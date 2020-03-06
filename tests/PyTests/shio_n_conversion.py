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
from frommle.sh import SHnmtGuide,trig
from frommle.core import logInfo
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
        """Test the GSMv6 format (reading only)"""
        gsmfile='../data/GSM-2_2008001-2008031_GRAC_UTCSR_BA01_0600.gz'
        #read from file
        cnmload=makeGArray(SHnmtGuide(),name="cnm")
        with shopen(gsmfile,mode='r',format=formats.GSMv6) as sharin:
            cnmload.load(sharin)

        #check maximum degree
        shg=cnmload.gp[0]
        self.assertEqual(shg.nmax,60)

        #also check a few handchecked values
        valdat=[(shg.idx((13, 1,trig.c)),-.514368595279E-07),
                (shg.idx((13,1,trig.s)),0.386779437418E-07),
                (shg.idx((60,60,trig.c)), 0.378141240210E-08),
                (shg.idx((60,60,trig.s)), 0.206527643361E-10)
                ]

        for idx,val in valdat:
            self.assertEqual(val,cnmload.mat[idx])

    def test_icgem(self):
        """Test reading SH data from the icgem format"""
        icgemfile="../data/ITSG-Grace2018_n60_2010-04.gfc.gz"

        #read from file
        cnmload=makeGArray(SHnmtGuide(),name="cnm")
        with shopen(icgemfile,mode='r',format=formats.icgem) as sharin:
            cnmload.load(sharin)

        #check maximum degree
        shg=cnmload.gp[0]
        self.assertEqual(shg.nmax,60)

        #also check a few handchecked value
        # gfc    13   11 -4.452325661643e-08 -4.845607967346e-09  1.163458670286e-12  1.151077435222e-12
        # gfc    60   60  3.784696252231e-09  2.318360408875e-11  6.406286577683e-12  6.277124447653e-12
        valdat=[(shg.idx((13, 11,trig.c)),-4.452325661643e-08),
                (shg.idx((13,11,trig.s)),0-4.845607967346e-09),
                (shg.idx((60,60,trig.c)), 3.784696252231e-09),
                (shg.idx((60,60,trig.s)), 2.318360408875e-11)
                ]

        for idx,val in valdat:
            self.assertEqual(val,cnmload.mat[idx])



    def checkmat(self,mat1,mat2):
        for i,o in zip(mat1,mat2):
            self.assertEqual(i,o)

if __name__ == '__main__':
    logInfo()
    unittest.main()

