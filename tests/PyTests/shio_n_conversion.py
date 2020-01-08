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
    """Make a test array from a Spherical harmonic guide"""
    arr=np.zeros([shg.size()])
    for i,(n,m,t) in enumerate(shg):
        if m==0 and t == 1:
            continue
        arr[i]=n+m+t
    return arr

class SHIO(unittest.TestCase):
    def test_standard(self):
        nmax=5
        shgar=makeGArray(SHnmtGuide(nmax),name="cnm")
        #put s perdictive set of values in the matrix
        shgar.mat[:]=makeTestSh(shgar.gp()[0])
        #write to file
        fileout='tmpout.sh.gz'
        with shopen(fileout,mode='w',format=formats.standard) as sharout:
            sharout.attr["tstart"]=datetime(2000,1,1)
            sharout.attr["tend"]=datetime(2002,6,30)
            sharout.attr["tcent"]=(sharout.attr["tend"]-sharout.attr["tstart"])/2+sharout.attr["tstart"]
            shgar.save(sharout)

        #read stuff in again
        shgarload=makeGArray(SHnmtGuide(),name="cnm")
        with shopen(fileout,mode='r',format=formats.standard) as sharin:
            shgarload.load(sharin)

        for i,o in zip(shgar.mat,shgarload.mat):
            self.assertEqual(i,o)

        os.remove(fileout)

if __name__ == '__main__':
    logInfo()
    unittest.main()

