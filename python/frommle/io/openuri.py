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
import re
import os

class openuri():
    def __init__(self,uri,mode='r'):
        """Opens a resource (file, tarball component,...) as an fid-like object"""
        self.uri=uri
        self.tarar=None
        if mode == "r":
    

    def openforReading(self):
        if os.path.exists(uri):
            #open as a normal file
            if self.name.endswith(".gz"):
                self.fid=gz.open(self.name,mod)
            else:
                self.fid=open(self.name,mod)
        else:
           #maybe the file is part of a tarball
           tgzmatch=re.search("(.+\.tgz):(.+)",uri)
           if tgzmatch:
               #open a tarfile and open the resource in it
               tarf=tgzmatch.groups()[0]
               if os.path.exists(tarf):
                   self.tarar=tarfile.open(tarf,"r:gz")

                    

               else:
                    raise FileNotFoundError(tarf)


    def __enter__(self):
        pass

    def __exit__(self,type,value,traceback):
        pass
