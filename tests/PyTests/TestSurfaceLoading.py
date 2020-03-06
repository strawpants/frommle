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
import unittest

import os
# from frommle.earthmodels.Earthmodel import  SNREI
import numpy as np
import math
import io 


def getStokes(filename):
    shcoef=io.StringIO("META    5    0.000000    0.000000    0.000000"
    "0    0  0.11443310195063E+08  0.00000000000000E+00"
    "1    0  0.33459521883773E+08  0.00000000000000E+00"
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


# class testSurfaceLoading(unittest.TestCase):
#     def test_eqh2Stokes(self):
#         loadlovef=os.path.join(os.environ['RLFTLBX_DATA'],'love','PREM.love')
#         loadlovef=findresource('PREM.love',table='love',scheme='earthmodels')
#         nmax=5
#         #feed an elastic Earth model some love numbers
#         snrei=SNREI(loadlovef,nmax)
#
#         #retrieve the stokes2eqqh operator
#         s2eqh=snrei.stokes2eqh();
#         self.assertEqual("test","test")

from geoslurp.db import geoslurpConnect
from geoslurp.discover.gravity import queryStatic,queryGRACE
from frommle.io import GSMConcatArchive
import os

class testHybridAr(unittest.TestCase):
    def test_shhybrid(self):
        pgcon=geoslurpConnect()
        gsmfiles=[x.gsm for x in queryGRACE(pgcon,"gracel2_gfz_rl06")]
        GSMArchive=GSMConcatArchive(gsmfiles[0:10])
        tguide=GSMArchive["tstart"]
        print(tguide)