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
import tarfile

class formats(Enum):
    standard=1
    icgem=2
    GSMv6=3
    unknown=4

class SHArchive(Group):
    """base type to read SH datasets from various text filetypes """
    loaded=False
    #Varnames holds valid variable names with variable classes which may be overloaded/extended in derived classes
    vars={"shg":Variable,"cnm":np_float64Var,"sigcnm":np_float64Var}
    shg_c=SHnmtGuide
    def __init__(self,filename,mode='r',nmax=-1):
        if type(filename) == str:
            #important: initialize Group class as well
            Group.__init__(self,filename)
            self.openfid=None
        else:
            #we consider filename as an open fid
            Group.__init__(self,"sharchive")
            self.openfid=filename
        
        self.setAmode(mode)
        if nmax == -1:
            nmax=0
        else:
            self.attr["nmax"]=nmax
        #also setup the variables (don't fill them with data yet)
        for varname,varclass in self.vars.items():
            if varname == "shg":
                self[varname]=varclass(self.shg_c(nmax,varname))
            else:
                self[varname]=varclass(name=varname);

    def __enter__(self):
        if self.readable():
            self.fload_impl()
        return self
    def __exit__(self, type, value, traceback):
        """This save the existing data to a file when the  archive is in write mode"""
        self.sync()

    def sync(self):
        if self.writable():
            self.fsave_impl()

    def __del__(self):
        self.sync()

    def fid(self):
        """Opens the text file (and possibly pass through a gzip filter) and returns a file descriptor"""
        if self.openfid:
            #quickly return if an openfid has been provided
            return self.openfid

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
