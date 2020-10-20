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

# Author Roelof Rietbroek (roelof@geod.uni-bonn.de), 2020


from frommle.io.fileConcatArchive import FileConcatArchive, AttrCoordMapper,CoordMapper
from frommle.io.GSM import SHGSMv6Archive
from frommle.io.icgem import SHicgemArchive
from frommle.io.shstandard import SHStandardArchive

from frommle.core import TimeGuide,makeGArray

class TcentCoordMapper(CoordMapper):
    def __init__(self):
        self.coord=TimeGuide("time")
    def append(self,grp):
        #compute the center time
        tcent=grp.attr["tstart"]+(grp.attr["tend"]-grp.attr["tstart"])/2
        self.coord.append(tcent)


class SHConcatArchive(FileConcatArchive):
    ArClass=None
    coordMapping={"tstart":AttrCoordMapper("tstart",TimeGuide),"time":TcentCoordMapper()}
    extendVars=["cnm"]
    noExtendVars=["shg"]
    def __init__(self, filelist,mode='r',type=None):
        #set the Archive type 
        if type  == "icgem":
            self.ArClass=SHicgemArchive
        elif type == "GSM":
            self.ArClass=SHGSMv6Archive
        else:
            self.ArClass=SHStandardArchive

        super().__init__(filelist,mode)
        

    def cnmArr(self):
        """returns a dense GArray with the Cnm coefficients from the archive"""
        return self.getGArr("cnm",["shg","time"])

    def getGArr(self,name,guides):
        gp=[ self[gname].value for gname in guides]
        garout=makeGArray(*gp,name=name)
        garout.load(self)
        return garout


