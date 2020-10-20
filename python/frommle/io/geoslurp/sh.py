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
from frommle.core import TimeGuide
from frommle.io.icgem import SHicgemArchive
from frommle.io.json import JSONArchive
from frommle.core.garray import makeGArray


def SHArfromFormat(formatstr):
    if formatstr == "icgem" :
        return SHicgemArchive
    elif formatstr == "JSONB":
        return JSONArchive
    else:
        raise RuntimeError("format %s not recognized"%formatstr)

def loadSHGuide(Agroup,name="shg"):
    """Loads a shguide from an archive group"""
    if hasattr(Agroup[name],"value"):
        return Agroup[name].value
    else:
        #try building a shguide from an array of tuples
        for nmt in Agroup[name][:]:
            pass


def loadSHGarray(pdAr):
    """Load a garray spanned by a shguide and a time guide from a pandasGroup""" 
    
    #load the time guide
    tg=TimeGuide("time")
    for dt in pdAr.df["time"]:
        tg.append(dt.to_pydatetime())

        
    sharclass=SHArfromFormat(pdAr.df["format"][0]) 
    #load data into array
    for i,fid in enumerate(pdAr.openuris()):
        with sharclass(fid) as sha:
            if i == 0:
                shg=sha["shg"].value
                gar=makeGArray(shg,tg,name="cnm")
                
            gar.mat[:,i]=sha["cnm"].array
    return gar
