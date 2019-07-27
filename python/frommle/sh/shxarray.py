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
import xarray as xr
import numpy as np
from frommle.sh import SHtmnGuide

def shxarray(nmax):
    shg=SHtmnGuide(nmax,"cnm")
    return xr.DataArray(np.zeros([shg.size()]),dims=[shg.name],coords={shg.name:np.array([x for x in shg],dtype=[("n",'i8'),("m",'i8'),("t","O")])})



def shxarray_load(shfiles,nmax=None,indexTime=False):
    """Read spherical harmonic coefficients from a list of files into an xarray"""
    nfiles=len(shfiles)

    #read the first shfile to find out maximum degree and format

    # if nmax:
    #     shg=SHtmnGuide(nmax)
    # else:
    #     #read the first file to find out the maximum degree



    # if(indexTime):
    #     #create a teimindex to be used as a dimension
    #
    # else:
    #     #use the filenames themselves as an index


