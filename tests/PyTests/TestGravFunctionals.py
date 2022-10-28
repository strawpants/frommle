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
import unittest
from frommle.core import logInfo
# from frommle.earthmodels.Earthmodel import  SNREI
import numpy as np
import math
import io 
from sqlalchemy import create_engine
import pandas as pd
import tarfile
from frommle.io import SHConcatArchive
import os

# def getStokes(filename):
    # shcoef=io.StringIO("META    5    0.000000    0.000000    0.000000"
    # "0    0  0.11443310195063E+08  0.00000000000000E+00"
    # "1    0  0.33459521883773E+08  0.00000000000000E+00"
    # "2    0  0.82373621262374E+08  0.00000000000000E+00"
    # "3    0  0.99634454013549E+08  0.00000000000000E+00"
    # "4    0  0.11887990641308E+09  0.00000000000000E+00"
    # "5    0  0.14059794911887E+09  0.00000000000000E+00"
    # "1    1  0.33459521883773E+08  0.33459521883773E+08"
    # "2    1  0.82373621262374E+08  0.82373621262374E+08"
    # "3    1  0.99634454013549E+08  0.99634454013549E+08"
    # "4    1  0.11887990641308E+09  0.11887990641308E+09"
    # "5    1  0.14059794911887E+09  0.14059794911887E+09"
    # "2    2  0.82373621262374E+08  0.82373621262374E+08"
    # "3    2  0.99634454013549E+08  0.99634454013549E+08"
    # "4    2  0.11887990641308E+09  0.11887990641308E+09"
    # "5    2  0.14059794911887E+09  0.14059794911887E+09"
    # "3    3  0.99634454013549E+08  0.99634454013549E+08"
    # "4    3  0.11887990641308E+09  0.11887990641308E+09"
    # "5    3  0.14059794911887E+09  0.14059794911887E+09"
    # "4    4  0.11887990641308E+09  0.11887990641308E+09"
    # "5    4  0.14059794911887E+09  0.14059794911887E+09"
    # "5    5  0.14059794911887E+09  0.14059794911887E+09"
    # )


class testGrav(unittest.TestCase):
    def testeqh(self):
        dbeng=create_engine("sqlite:///data/itsg2018_sample.db")
        #modify urls so they point to the correct (extracted) files
        gsmfiles=["tgzextract"+uri.split(":")[1] for uri in pd.read_sql("SELECT * FROM obp;",dbeng)["uri"]]
        
        #extract files
        if not os.path.exists("tgzextract"):
            with tarfile.open("data/itsg2018_sample_files.tgz","r:gz") as tid:
                def is_within_directory(directory, target):
                    
                    abs_directory = os.path.abspath(directory)
                    abs_target = os.path.abspath(target)
                
                    prefix = os.path.commonprefix([abs_directory, abs_target])
                    
                    return prefix == abs_directory
                
                def safe_extract(tar, path=".", members=None, *, numeric_owner=False):
                
                    for member in tar.getmembers():
                        member_path = os.path.join(path, member.name)
                        if not is_within_directory(path, member_path):
                            raise Exception("Attempted Path Traversal in Tar File")
                
                    tar.extractall(path, members, numeric_owner=numeric_owner) 
                    
                
                safe_extract(tid, "tgzextract")

        # #load GSM files
        gsmarchive=SHConcatArchive(gsmfiles,type="icgem")

        # with tarfile.open(tarball,"r:gz") as tid:
            # for uri in gsm["uri"]:
                # print(uri)

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

# from geoslurp.db import geoslurpConnect
# from geoslurp.discover.gravity import queryStatic,queryGRACE
# from frommle.io import GSMConcatArchive
# import os

# class testHybridAr(unittest.TestCase):
    # def test_shhybrid(self):
        # pgcon=geoslurpConnect()
        # gsmfiles=[x.gsm for x in queryGRACE(pgcon,"gracel2_gfz_rl06")]
        # GSMArchive=GSMConcatArchive(gsmfiles[0:10])
        # cnm=GSMArchive.cnmArr()
        # # tguide=GSMArchive["tstart"]
        # print(tguide)

if __name__ == '__main__':
    logInfo()
    unittest.main()
