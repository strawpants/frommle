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

# Author Roelof Rietbroek (roelof@wobbly.earth), 2021



# a class which can be used as a netcdf file descriptor to read an write guided arrays

from frommle.io import Group
from frommle.io import Variable_obj

class NCVar(Variable_obj):
    def __init__(self,name):
        Variable_obj.__init__(self,name)

class NCArchive(Group):
    def __init__(self,filename,mode='r'):
        Group.__init__(self,filename)
        self.setAmode(mode)
        self.filename=filename
    def convVariable(self,chld):
        import pdb;pdb.set_trace()
    def createVariable(self,name):
        import pdb;pdb.set_trace()
        self[name]=NCVar(name)
        return self[name]
