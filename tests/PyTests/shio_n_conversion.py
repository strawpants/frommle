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
from frommle.core import makeGArray
from datetime import datetime
import gzip
import io
import numpy as np

def makeTestStokes():
    """Writes a gzipped test file with independently computed degree 5 expansion of ones convolved with PREM
      and parameter rhowater 1025kg/m3 and rho_earth 5517 kg/m3"""
    shcoef=io.StringIO("META    5    2003.0    2003.5    2004.0\n"
                       "0    0  0.11443310195063E+08  0.00000000000000E+00\n"
                       "1    0  0.33459521883773E+08  0.00000000000000E+00\n"
                       "2    0  0.82373621262374E+08  0.00000000000000E+00\n"
                       "3    0  0.99634454013549E+08  0.00000000000000E+00\n"
                       "4    0  0.11887990641308E+09  0.00000000000000E+00\n"
                       "5    0  0.14059794911887E+09  0.00000000000000E+00\n"
                       "1    1  0.33459521883773E+08  0.33459521883773E+08\n"
                       "2    1  0.82373621262374E+08  0.82373621262374E+08\n"
                       "3    1  0.99634454013549E+08  0.99634454013549E+08\n"
                       "4    1  0.11887990641308E+09  0.11887990641308E+09\n"
                       "5    1  0.14059794911887E+09  0.14059794911887E+09\n"
                       "2    2  0.82373621262374E+08  0.82373621262374E+08\n"
                       "3    2  0.99634454013549E+08  0.99634454013549E+08\n"
                       "4    2  0.11887990641308E+09  0.11887990641308E+09\n"
                       "5    2  0.14059794911887E+09  0.14059794911887E+09\n"
                       "3    3  0.99634454013549E+08  0.99634454013549E+08\n"
                       "4    3  0.11887990641308E+09  0.11887990641308E+09\n"
                       "5    3  0.14059794911887E+09  0.14059794911887E+09\n"
                       "4    4  0.11887990641308E+09  0.11887990641308E+09\n"
                       "5    4  0.14059794911887E+09  0.14059794911887E+09\n"
                       "5    5  0.14059794911887E+09  0.14059794911887E+09\n"
                       )
    testfile="tmp.sh.gz"

    with gzip.open(testfile,'wt') as gzid:
        gzid.write(shcoef.read())
    return testfile

class SHIO(unittest.TestCase):
    def test_standard(self):

        nmax=5
        shgar=makeGArray(SHnmtGuide(nmax),name="cnm")
        #put random stuff in there
        shgar.mat[:]=np.random.random_sample(shgar.mat.shape)
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

        print(shgar.mat)
        print(shgarload.mat)
        # self.assertEqual(shgar.gp ","test")

if __name__ == '__main__':
    logInfo()
    unittest.main()

