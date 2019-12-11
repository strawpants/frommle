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
from frommle.io import Group,Variable
from frommle.io.numpyVariable import np_float64Var
from frommle.sh import SHnmtGuide
import gzip as gz
from enum import Enum

class formats(Enum):
    standard=1
    icgem=2
    GSMv6=3
    unknown=4

class SHGVar(Variable):
    shg=None
    def __init__(self,shg=None,name=None):
        if name:
            super().__init__(name)
        else:
            super().__init__()
        
        if shg:
            self.attr["typehash"]=shg.hash()
            self.shg=shg
    def __getitem__(self,slc):
        if type(slc) == slice:
            selectall = slc == slice(None)
        else:
            raise RuntimeError("cannot get item without slicing")

        if selectall:
            return self.shg
        else:
            return self.shg.__array__()[slc]

class SHArchive(Group):
    """base type to read SH datasets from various text filetypes """
    loaded=False
    #Varnames holds valid variable names with variable classes which may be overloaded/extended in derived classes
    vars={"shg":SHGVar,"cnm":np_float64Var,"sigcnm":np_float64Var}
    shg_c=SHnmtGuide
    def __init__(self,filename,mode='r',nmax=0):
        #important: initialize Group class as well
        Group.__init__(self,filename)
        self.setAmode(mode)
        #also setup the variables (don't fill them with data yet)
        for varname,varclass in self.vars.items():
            if varclass == SHGVar:
                if nmax:
                    self[varname]=varclass(shg=self.shg_c(nmax))
                else:
                    self[varname]=varclass(shg=self.shg_c())
            else:
                self[varname]=varclass(varname);

    def __enter__(self):
        if self.readable():
            self.fload_impl()
        return self
    def __exit__(self, type, value, traceback):
        """This save the existing data to a file when the  archive is in write mode"""
        if self.writable():
            self.fsave_impl()

    def fid(self):
        """Opens the text file (and possibly pass through a gzip filter) and returns a file descriptor"""
        if self.readable():
            mod="rt"
        elif self.writable():
            mod="wt"

        if self.name.endswith(".gz"):
            return gz.open(self.name,mod)
        else:
            return open(self.name,mod)
    
    def fload(self):
        if self.loaded:
            return

        if self.attr["mode"] == 'w':
            #no need to load file as it is in write mode
            return

        #call abstract method of the derived class
        self.fload_impl()
        self.loaded=True

    def fload_impl(self):
        pass

    def fsave_impl(self):
        pass
