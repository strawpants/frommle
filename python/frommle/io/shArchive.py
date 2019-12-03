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


from frommle.core import typehash
from frommle.io import Group,Variable,Variable_float64
from frommle.sh import SHnmtGuide
import gzip as gz
from enum import Enum

class formats(Enum):
    standard=1
    icgem=2
    GSMv6=3
    unknown=4

class SHGVar(Variable):
    def __init__(self,shg=None,name=None):
        if name:
            super().__init__(name)
        else:
            super().__init__()
        
        if shg:
            self.attr["typehash"]=shg.hash()

# class SHVar(Variable_float64):
        # """contains a variable object which allows reading/writing to an SHarchive"""
        # def __init__(self,name="cnm"):
            # # TreeNode.__init__(self)
            # super().__init__(name)

        # def __getitem__(self,slc):
            # """return (slices of) the data"""
            # return self.parent.vars[self.name][slc];



class SHArchive(Group):
    """base type to read SH datasets from various text filetypes """
    loaded=False
    #Varnames holds valid variable names which can be overloaded/extended in derived classes 
    # vars={"shg":None,"cnm":None}
    shg_c=SHnmtGuide
    def __init__(self,filename,mode='r',nmax=None):
        #important: initialize Group class as well
        Group.__init__(self,filename)
        self.setAmode(mode)
        self.fload()
        #also sets the variables
        # for varname,val in self.vars.items():
            # self[varname]= SHVar(varname);
            # if varname == "shg":
                # #set a dedicated typehash (needed to load SHGuides from C++)
                # self[varname].attr["typehash"]=val.hash()

    def fid(self):
        """Opens the text file (and possibly pass through a gzip filter) and returns a file descriptor"""
        if self.readable():
            mod="rt"
        elif self.writeable():
            mod="wt"

        if self.name.endswith(".gz"):
            return gz.open(self.name,mod)
        else:
            return open(self.name,mod)
    
    # def getVariable(self,varname):
        # """This overloads the c++ routine getVariable (needed if more complexed object will be read from C++)"""
        
        # self.fload()
        
        # if varname in self.vars.keys():
            # # self[varname]= SHVar(varname);
            # if varname == "shg":
                # #set a dedicated typehash (needed to load SHGuides from C++)
                # self[varname].attr["typehash"]=self.vars[varname].hash()
                
        # else:
            # raise RunTimeError("Variable %s not supported for this archive"%varname)
    
        # return self[varname]

    # def shg(self):
        # """Convenience function to retrieve the stored SH guide"""
        # self.fload()
        # return self.vars["shg"]

    def fload(self):
        if self.loaded:
            return

        if self.attr["mode"] == 'w':
            raise RunTimeError("Cannot load sh data in write mode")

        #call abstract method of the derived class
        self.fload_impl()
        self.loaded=True

    def fload_impl(self):
        pass
