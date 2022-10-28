# This file is part of Frommle
# Frommle is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.

# Frommle is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with Frommle; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# Author Roelof Rietbroek (roelof@wobbly.earth), 2020
from frommle.io import Group
from frommle.io.numpyVariable import np_Var
import os
from sqlalchemy import create_engine, func,select
import tarfile
import pandas as pd
import geopandas as gpd 
import re
from io import BytesIO,TextIOWrapper
import gzip
import shapely.wkb
from rasterio.io import MemoryFile
import copy

cache=".geoslurpcache"
class PandasGroup(Group):
    """Wraps a pandas dataframe in a Group object. Also allows a recursive loading of uri's"""
    def __init__(self,name,eng=None,qry=None,df=None):
        Group.__init__(self,name)

        if eng:
            self.eng=eng
        if qry:
            #read from the database engine as a query
            self.df=pd.read_sql(qry,eng,parse_dates=["time","tstart","tend"])
        elif type(df) != None:
            #directly assign from a dataframe
            self.df=df

        if not os.path.exists(cache):
            os.mkdir(cache)
        if "uri" in self.df:
            #possibly refer to local tgz filestore
            self.df["uri"]=self.df["uri"].str.replace("^.+.tgz:",cache)
        if "geom" in self.df:
            #convert normal data frame to geopandas dataframe
            self.df["geom"]=self.df["geom"].apply(PandasGroup.wkb2shapely)
            self.df=gpd.GeoDataFrame(self.df,geometry="geom")
        
        if "rast" in self.df:
            #create an additional column which contains rasterio objects
            self.df["rast"]=self.df["rast"].apply(PandasGroup.gdal2rasterio)

    def __getitem__(self,ky):
        """retrieve a variable"""
        if ky in self:
            #already present, quickly return a variable
            return super(Group,self).__getitem__(ky)
        elif ky in self.df:
            #create a new numpy like variable containing the column data
            self[ky]=np_Var(arr=self.df[ky].to_numpy(),name=ky)
            return super(Group,self).__getitem__(ky)
        else:
            raise KeyError("Variable/Column %s not found in PandasGroup"%ky)
   
    @staticmethod
    def openfile(fname):
        if fname.endswith(".gz"):
            return gzip.open(fname,"rt")
        else:
            return open(fname,"rt")
    
    @staticmethod
    def wkb2shapely(entry):
        return shapely.wkb.loads(str(entry),hex=True)

    @staticmethod
    def gdal2rasterio(entry):
        return MemoryFile(entry.tobytes())

    def openuris(self):
        """Generator which opens uris as bufferlike objects"""
        selfregex=re.compile("self:([^\s]+)")
        for row in self.df.itertuples():
            selfmatch=selfregex.search(row.uri)
            if selfmatch:
                #load from other column
                yield BytesIO(getattr(row,selfmatch.groups()[0]).encode("utf-8"))
            elif os.path.exists(row.uri):
                yield PandasGroup.openfile(row.uri)
            else:
                #directly read the file through the database
                # and put it in the local cache
                cachefile=os.path.join(cache,os.path.basename(row.uri))
                if os.path.exists(cachefile):
                    yield PandasGroup.openfile(cachefile)
                else:
                    qry=select([func.readfile(row.uri).label('fid')])
                    res=self.eng.execute(qry).first()
                    buff=BytesIO(res.fid)
                    with open(cachefile,'wb') as fid:
                        fid.write(buff.getbuffer())

                    if row.uri.endswith(".gz"):
                        yield TextIOWrapper(gzip.GzipFile(fileobj=buff,mode="r"))
                    else:
                        yield buff

class GeoslurpArchive(Group):
    """Opens a geoslurp like database, and acts like an archive"""
    def __init__(self,connection,mode='r',scheme="public"):
        self.scheme=scheme
        if type(connection)  == str:

            Group.__init__(self,connection)
            
            if connection.endswith(".sql"):
                self.loadsqlite()
                if scheme == "public":
                    scheme=None
            else:
                #try loading this as  database
                self.eng=create_engine(self.name)


        else:
            #use an existing db connection
            Group.__init__(self,"dbconnection")
            self.eng=connection
        self.tnames=self.eng.table_names(schema=scheme)
        
        self.setAmode(mode)
        if self.writable():
            raise RuntimeError("Sorry no writing to geoslurpArchive is currently supported")

         
    def __enter__(self):
        return self


    def __exit__(self,type,value,traceback):
        """Close database connections and possible opened tarball"""
        
        if self.eng:
            self.eng.dispose()

    def __getitem__(self,kyqry):
        """retrieves a group (i.e. table)"""
        if type(kyqry) == tuple:
            ky=kyqry[0]
            qry=kyqry[1]
            #also delete the current group when it exists already (will be overwritten with the results of the new query)
            if ky in self:
                del self[ky]
        else:
            #select everything
            ky=kyqry
            qry="SELECT * from %s.%s"%(self.scheme,ky)
        #check if the group already exists
        if ky in self:
            return super(Group,self).__getitem__(ky)
        else:
            #create a new group which wraps a panda dataframe
            super(Group,self).__setitem__(ky,PandasGroup(ky,self.eng,qry))
            return super(Group,self).__getitem__(ky)
    
    def __setitem__(self,ky,df:pd.DataFrame):
            """Assign a table from a data frame"""
            super(Group,self).__setitem__(ky,PandasGroup(ky,eng=self.eng,df=df))

    def loadsqlite(self):
        """setups reading from an sqlite file and possibly an accompanying file archive"""
        if os.path.exists(self.name):
            #Treat it as a local sqlite file
            self.eng=create_engine("sqlite:///"+self.name)
            #try to see if a tarball is associated with this sqlite db
            arfile=os.path.splitext(self.name)[0]+"_files.tgz"
            if os.path.exists(arfile):
                if not os.path.exists(cache):
                    with tarfile.open(arfile,"r:gz") as tid:
                        def is_within_directory(directory, target):
                            
                            abs_directory = os.path.abspath(directory)
                            abs_target = os.path.abspath(target)
                        
                            prefix = os.path.commonprefix([abs_directory, abs_target])
                            
                            return prefix == abs_directory
                        
                        def safe_extract(tar, path=".", members=None, *, numeric_owner=False):
                        
                            for member in tar.getmembers():
                                member_path = os.path.join(path, member.name)
                                if not is_within_directory(path, member_path):
                                    raise Exception("Attempted Path Traversal in Tar File")
                        
                            tar.extractall(path, members, numeric_owner=numeric_owner) 
                            
                        
                        safe_extract(tid, cache)
        else:
            raise RuntimeError("Archive is not an existing sqlite database")


