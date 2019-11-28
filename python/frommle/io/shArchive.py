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



from frommle.io import Archive,Variable_float64

from enum import Enum

class formats(Enum):
    standard=1
    icgem=2
    GSMv6=3
    unknown=4

class SHVar(Variable):
        """contains a variable object which allows reading/writing to an SHarchive"""
        def __init__(self,name="sh"):
            pass



class SHArchive(Archive):
    """Reads sh datasets from various files"""
    gzip=False
    def __init__(self,filename,mode='r',format=formats.unknown):
        super().__init__(name,mode)

        if filen.endswith(".gz"):
            gzip=True

        if mode == 'r':
            if format == formats.unknown:
                self.format=queryformat()
        elif mode == "w" and format == formats.unknown:
            raise RunTimeError("When writing an sh file the format option is obligatory")
        else:
            raise RunTimeError("Unknown mode %s supplied",%mode)


    def getVariable(self,name):
        if name in self:
            #already exists
            return self[name]
        else:
            return self[name]=SHVar(name);


    def queryformat(self):

        ftest=self.name.strip(".gz")

        if ftest.endswith("gfc"):
            #quick return
            self.format=formats.icgem

        #no we open the file to search for format hints
        if gzip:
            fid=gz.open(self.name,'rt')
        else:
            fid=open(self.name,'rt')
        try:
            ln = fid.readline()
        except Exception as excep:
            raise IOError("Reading error for %s"%self.name)

        fid.close()

        if re.search("META",ln):
            self.format=formats.standard

        #fall back by means of exclusion
        self.format=formats.GSMv6


