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

from frommle.io import Variable_float64,Variable_int,Variable_obj
import numpy as np



class np_VarBase():
    """metaClass which wraps a numpy array and acts as frommle Variable """
    array=None
    def __init__(self,arr=None,name:str=None):
        if name:
            super().__init__(name)
        else:
            super().__init__()

        if type(arr) != type(None):
            #this also allows the conversion of a list to a numpy array if needed
            self.array=np.array(arr)
   
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
        if not self.isSet():
            if type(slc) == tuple:
                #ndim slicing
                maxshape=np.zeros([s.stop for s in slc])
            elif type(slc) == slice:
                maxshape=[slc.stop]
            self.array=np.zeros(maxshape)

        self.array[slc]=ArrValue

    def shape(self):
        if self.isSet():
            return self.array.shape
        else:
            return ()

np_intVar=type("np_intVar",(np_VarBase,Variable_int,),{})
np_float64Var=type("np_float64Var",(np_VarBase,Variable_float64,),{})
np_objVar=type("np_objVar",(np_VarBase,Variable_obj,),{})

def np_Var(arr=None,name:str=None):
    if type(arr) == type(None):
        #default to a float
        return np_float64Var(name=name)
    else:
        #check dtype of the array
        if arr.dtype == np.float64:
            return np_float64Var(name=name,arr=arr)
        elif arr.dtype == np.int32:
            return np_floatIntVar(name=name,arr=arr)
        else:
            #return object type (not very useful to interact within c++ though
            return np_objVar(name=name,arr=arr)
