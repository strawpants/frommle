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
from frommle.io.shstandard import write_shstandard, read_shstandard
from datetime import datetime
import gzip
import io


def makeTestStokes():
    """Writes a gzipped test file with independently computed degree 5 expansion of ones convolved with PREM
      and parameter rhowater 1025kg/m3 and rho_earth 5517 kg/m3"""
    shcoef=io.StringIO("META    5    0.000000    0.000000    0.000000\n"
                       "0    0  0.11443310195063E+08  0.00000000000000E+00\n"
                       "1    0  0.33459521883773E+08  0.00000000000000E+00\n"
                       "2    0  0.82373621262374E+08  0.00000000000000E+00"
                       "3    0  0.99634454013549E+08  0.00000000000000E+00"
                       "4    0  0.11887990641308E+09  0.00000000000000E+00"
                       "5    0  0.14059794911887E+09  0.00000000000000E+00"
                       "1    1  0.33459521883773E+08  0.33459521883773E+08"
                       "2    1  0.82373621262374E+08  0.82373621262374E+08"
                       "3    1  0.99634454013549E+08  0.99634454013549E+08"
                       "4    1  0.11887990641308E+09  0.11887990641308E+09"
                       "5    1  0.14059794911887E+09  0.14059794911887E+09"
                       "2    2  0.82373621262374E+08  0.82373621262374E+08"
                       "3    2  0.99634454013549E+08  0.99634454013549E+08"
                       "4    2  0.11887990641308E+09  0.11887990641308E+09"
                       "5    2  0.14059794911887E+09  0.14059794911887E+09"
                       "3    3  0.99634454013549E+08  0.99634454013549E+08"
                       "4    3  0.11887990641308E+09  0.11887990641308E+09"
                       "5    3  0.14059794911887E+09  0.14059794911887E+09"
                       "4    4  0.11887990641308E+09  0.11887990641308E+09"
                       "5    4  0.14059794911887E+09  0.14059794911887E+09"
                       "5    5  0.14059794911887E+09  0.14059794911887E+09"
                       )
    testfile="tmp.sh.gz"

    with gzip.open(testfile,'wt') as gzid:
        gzid.write(shcoef.read())
    return testfile

class SHIOTests(unittest.TestCase):
    def test_shiostandard(self):
        filen=makeTestStokes()
        shdata=read_shstandard(filen)

        # epoch=datetime(2009,1,1)
        # hdr,sh=read_icgem(file,nmax=2,epoch=epoch)
        #
        # for i,(c,s) in enumerate(zip(sh.C,sh.S)):
        #     print(sh.nm(i),c,s)

        self.assertEqual("test","test")

if __name__ == '__main__':
    unittest.main()

