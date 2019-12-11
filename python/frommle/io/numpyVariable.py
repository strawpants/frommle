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

# Author Roelof Rietbroek (roelof@wobbly.earth), 2019

from frommle.io import Variable_float64,Variable_int
import numpy as np

# classmapper={np.dtype(np.float64):Variable_float64,np.dtype(np.int)}


class np_float64Var(Variable_float64):
    """Class which implements the C++ Variable_float64, so it can be used both from C++ and python """
    array=None
    def __init__(self,arr=None,name=None):
        if name:
            super().__init__(name)
        else:
            super().__init__()

        if type(arr) != type(None):
            self.array=arr
   
    def __getitem__(self,slc=None):
        """returns (a slice of) the internal array"""
        if type(self.array) == type(None):
            return None
        if slc == slice(None,None):
            #return the whole thing
            return self.array
        else:
            return self.array[slc]

    def isSet(self):
        return type(self.array) != type(None)

    def __setitem__(self,slc,ArrValue):
        """sets the internal array"""
        if type(slc) == tuple:
            #ndim slicing
            selectall=all(map(lambda x: x==slice(None),slc))
        elif type(slc) == slice:
            selectall = slc == slice(None)

        if selectall:
            self.array=ArrValue
        else:
            if  not self.isSet():
                raise RuntimeError("Cannot set slice of non existing array")
            else:
                self.array[slc]=ArrValue
