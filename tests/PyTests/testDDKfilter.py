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

from frommle.io.BINV import readBIN

from frommle.sh.DDKfilter import DDKfilter
from frommle.io.shstandard import read_shstandard
import os
import urllib

#download necessary files from github
direc='ddktest'

ddkfile=os.path.join(direc,'DDK1.bin')

try:
    os.mkdir(direc)
except FileExistsError:
    pass

if not os.path.exists(ddkfile):
    urllib.request.urlretrieve("https://github.com/strawpants/GRACE-filter/raw/master/data/DDK/Wbd_2-120.a_1d10p_4",ddkfile)


inputf=os.path.join(direc,'inGSM.sh')
if not os.path.exists(inputf):
    urllib.request.urlretrieve("https://github.com/strawpants/GRACE-filter/raw/master/tests/GSM-2_2008122-2008153_0030_EIGEN_G---_0004in",inputf)


outputf=os.path.join(direc,'outGSM.sh')
if not os.path.exists(outputf):
    urllib.request.urlretrieve("https://github.com/strawpants/GRACE-filter/raw/master/tests/GSM-2_2008122-2008153_0030_EIGEN_G---_0004out",outputf)

inputfn60=os.path.join(direc,'inGSMn60.sh')
if not os.path.exists(inputfn60):
    urllib.request.urlretrieve("https://github.com/strawpants/GRACE-filter/raw/master/tests/GSM-2_2008122-2008153_0030_EIGEN_G---_0004lmax60in",inputfn60)


outputfn60=os.path.join(direc,'outGSMn60.sh')
if not os.path.exists(outputfn60):
    urllib.request.urlretrieve("https://github.com/strawpants/GRACE-filter/raw/master/tests/GSM-2_2008122-2008153_0030_EIGEN_G---_0004lmax60out",outputfn60)

hdr,incoef=read_shstandard(inputf)



filt=DDKfilter(ddkfile)




cnmout,snmout=filt(incoef)
# dat=readBIN(file)

