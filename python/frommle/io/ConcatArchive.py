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

from frommle.core import TimeGuide,IndexGuide
from frommle.core.logger import logger
from frommle.io import Group,Variable
from frommle.io import np_float64Var
import numpy as np
from abc import ABC,abstractmethod


class CoordMapper(ABC):
    """"This abstracts class defines the abstract methods needed for"""
    coord=None
    @abstractmethod
    def append(self,grp):
        """Append an object from the archive group"""
        pass

class AttrCoordMapper(CoordMapper):
    def __init__(self,attrName,guideClass):
        self.attr=attrName
        self.gClass=guideClass
    def append(self,grp):
        """appends a value to the internal guide """
        if not self.coord:
            self.coord=self.gClass()
        self.coord.append(grp.attr[self.attr])


class IdMapper(CoordMapper):
    """simple id mapper which holds a sequence"""
    def __init__(self):
        self.coord=IndexGuide()

    def append(self,grp):
        #this just increments the index by 1
        self.coord.append()

class TcentCoordMapper(CoordMapper):
    def __init__(self):
        self.coord=TimeGuide("time")
    def append(self,grp):
        #compute the center time
        tcent=grp.attr["tstart"]+(grp.attr["tend"]-grp.attr["tstart"])/2
        self.coord.append(tcent)

class ConcatArchive(Group):
    """An Archive which combines a generator of archives and adds a corresponding dimension the the outputvariables"""
    coordMapping=None # How a coodinate guide is created along the stacked dimension 
    extendVars=None #these variables will be extended with an additional dimension
    noExtendVars=None #these variables will be taken from the first archive and will be assumed to remain the same for all archives
    globAttr=[] #global attribute names to copy from the first Archive
    def __init__(self, archives,mode='r',nar=None):
        """Initialize a list of files which have the same archive type"""
        Group.__init__(self,"root")
        self.archives=archives

        #find out the stacking dimension
        if type(nar) == None:
            if hasattr(archives,"__len__"):
                self.nar=len(archives)
            else:
                raise IOError("Cannot determine stacking dimension from generator, please explicitly specify nar") 
        else:
            self.nar=nar

        self.setAmode(mode)
        self.readAll()

    def readAll(self):
        """read all data"""


        if not self.readable():
            raise IOError("File is not opened for reading")


        iar=0

        for ar in self.archives:
            logger.info("Appending data from archive %d"%iar)
            #construct the requested auxiliary coordinates(guides)
            for name,cmapper in self.coordMapping.items():
                cmapper.append(ar)

            if iar == 0:
                #on first loop initialize the extended numpy arrays
                for varn in self.extendVars:
                    shape=[ n for n in ar[varn].shape()]
                    shape.append(len(self.flist))
                    self[varn]=np_float64Var(name=varn,arr=np.zeros(shape))
                #and copy the specified variables from the firts archive
                for varn in self.noExtendVars:
                    #just take the variable from the first archive and assume it doesn't change for the others
                    self[varn]=ar[varn]
                
                #also extract global attributes from the first archive
                for attname in self.globAttr:
                    self.attr[attname]=ar.attr[attname]

            for varn  in self.extendVars:
                #copy data into the extended numpy array
                self[varn][...,iar]=ar[varn][:]


            iar+=1

        #add the completed coord mappers
        for name,cmapper in self.coordMapping.items():
            #add a variable containing the guide
            self[name]=Variable(cmapper.coord)
    

# def createConcatClass(ArClass,extendVars,noExtendVars,coordMapping={"id":IdMapper()}):
#     return type("ConcatArchive"+ArClass.__class__,FileConcatArchive,{"ArClass":ArClass,"coordMapping":coordMapping,"extendVars":extendVars,"noExtendVars":noExtendVars})

