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

# Author Roelof Rietbroek (roelof@geod.uni-bonn.de), 2018

import unittest
from frommle.io.shio import shopen,formats
from frommle.sh import SHnmtGuide
from frommle.core import logInfo
from frommle.core import TreeNode
from frommle.core.garray import makeGArray
from datetime import datetime
import gzip
import io
import numpy as np
import os

def makeTestSh(shg):
    """Make a test garray from a Spherical harmonic guide"""
    shgar=makeGArray(shg,name="cnm")
    shgar.mat[:]=np.zeros([shg.size()])
    for i,(n,m,t) in enumerate(shg):
        if m==0 and t == 1:
            continue
        shgar.mat[i]=n+m+t
    return shgar


def writeSH(shoarchive, gar):
    """Sets up some attributes and writes a garray to an archive"""

    shoarchive.attr["tstart"]=datetime(2000,1,1)
    shoarchive.attr["tend"]=datetime(2002,6,30)
    shoarchive.attr["tcent"]=(shoarchive.attr["tend"]-shoarchive.attr["tstart"])/2+shoarchive.attr["tstart"]
    gar.save(shoarchive)

class SHIO(unittest.TestCase):
    def test_standard(self):
        nmax=5
        shgar=makeTestSh(SHnmtGuide(nmax))

        #write to file
        fileout='tmpout.sh.gz'
        with shopen(fileout,mode='w',format=formats.standard) as sharout:
            writeSH(sharout,shgar)

        #read stuff in again
        shgarload=makeGArray(SHnmtGuide(),name="cnm")
        with shopen(fileout,mode='r',format=formats.standard) as sharin:
            shgarload.load(sharin)

        self.checkmat(shgar.mat,shgarload.mat)

        os.remove(fileout)

    def test_GSM(self):
        """Test the GSMv6 format"""
        nmax=6
        shgar=makeTestSh(SHnmtGuide(nmax))

        #write to file
        fileout='tmpout.sh.gz'
        with shopen(fileout,mode='w',format=formats.GSMv6) as sharout:
            writeSH(sharout,shgar)

    def checkmat(self,mat1,mat2):
        for i,o in zip(mat1,mat2):
            self.assertEqual(i,o)

if __name__ == '__main__':
    logInfo()
    unittest.main()

