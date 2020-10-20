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

from frommle.io import Group,Variable
from frommle.io.numpyVariable import np_Var
import os
import json
import numpy as np

class JSONArchive(Group):
    def __init__(self,name,mode='r'):
        if type(name) == str:
            super().__init__(name)
            with open(name,'rb') as fid:
                values=json.loads(fid)
        else:
            super().__init__("json")
            values=json.loads(name.read())
        
        for ky,val in values["attr"].items():
            self.attr[ky]=val

        for ky,val in values["vars"].items():
            if type(val) == list:
                self[ky]=np_Var(arr=np.array(val))
            else:
                self[ky]=Variable(val)

    def __enter__(self):
        return self


    def __exit__(self,type,value,traceback):
        pass


