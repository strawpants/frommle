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

# Author Roelof Rietbroek (roelof@geod.uni-bonn.de), 2019

import re
import gzip as gz
from frommle.io.shstandard import read_shstandard,write_shstandard
from frommle.io.icgem import read_icgem
from frommle.io.GSM import readGSMv06
from frommle.sh.shxarray import newshxarray
from frommle.core.frlogger import logger
from frommle.sh import SHtmnGuide
import numpy as np
from frommle.core.time import npdt64_2_datetime

class formats:
    standard=1
    icgem=2
    GSMv6=3

def queryformat(filename):
    gzip=False

    ftest=filename

    if filename.endswith(".gz"):
        gzip=True
        ftest=filename.strip(".gz")

    if ftest.endswith("gfc"):
        #quick return
        return formats.icgem
    
    #no we open the file to search for format hints
    if gzip:
        fid=gz.open(filename,'rt')
    else:
        fid=open(filename,'rt')
    try:
        ln = fid.readline()
    except Exception as excep:
        raise IOError("Reading error for %s"%filename)

    fid.close()

    if re.search("META",ln):
        return formats.standard
    
    #fall back by means of exclusion
    return formats.GSMv6

def sh_read_asxar(shfiles,nmax=None):
    """Reads a list of Spherical harmonic files into a 2D xarray.DataArray.
    The 2nd dimension will be set to the time dimension"""
    
    #start with the first file to find out nmax
    shformat=queryformat(shfiles[0])
    meta,idx,shc,_=sh_read(shfiles[0],nmax=nmax)

    if not nmax:
        #take te nmax supported by the file if not provided explicitly
        nmax=meta["nmax"]

    shg=SHtmnGuide(nmax)

    dtlist=[meta["tcent"]]
    #allocate space for DataArray
    shxout=newshxarray(shg,auxcoords={"time":np.array([x for x in range(len(shfiles))])})
    #copy data in first column
    shxout.loc[idx,0]=shc
    ith=1
    for shfile in shfiles[1:]:
        logger.info("reading %s"%shfile)
        meta,idx,shc,_=sh_read(shfile,nmax=nmax,format=shformat)
        shxout.loc[idx,ith]=shc
        dtlist.append(meta["tcent"])
        ith+=1
    #change the coordinate system to the time from the files and return  
    return shxout.assign_coords(time=dtlist)


def sh_writexarray(shxin,shfiles):
    """Write shxarray to files"""
     
    for i in range(len(shfiles)):
        meta={"tcent":npdt64_2_datetime(shxin["time"].values[i])}
        sh_write(shfiles[i],shxin.guidepack[0],shxin[:,i].values,meta=meta)
    
def sh_read(shfile,nmax=None,format=None,epoch=None,headerOnly=False,error=False):
    """Reads meta informaation and spherical harmonic coefficients and their errors into a list"""
    if not format:
        format=queryformat(shfile)

     
    if format == formats.standard:
        return read_shstandard(shfile,nmax=nmax,headerOnly=headerOnly,error=error)

    if format == formats.icgem:
        return read_icgem(shfile,nmax=nmax,epoch=epoch,headerOnly=headerOnly,error=error)

    if format == formats.GSMv6:
        return readGSMv06(shfile,nmax=nmax,headerOnly=headerOnly,error=error)

def sh_write(shfile,idx,shcoef,sherr=None,format=None,meta=None):
    """Write out a spherical harmonic dataset in a file"""
    if not format:
        format=formats.standard

    if format == formats.standard:
        write_shstandard(shfile,idx,shcoef,sherr=sherr,meta=meta)
        return

    raise NotImplementedError("Output sh format not implemented")
