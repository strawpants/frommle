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

try:
    os.mkdir(direc)
except FileExistsError:
    pass

shfilebase='GSM-2_2008122-2008153_0030_EIGEN_G---_0004'
shvariations=['in','out','lmax60out','lmax60in']

for shvar in shvariations:
    fout=os.path.join(direc,shfilebase+shvar)
    if not os.path.exists(fout):
        urllib.request.urlretrieve("https://github.com/strawpants/GRACE-filter/raw/master/tests/"+shfilebase+shvar,fout)

ddkfilt='Wbd_2-120.a_1d13p_4'
filtfile=os.path.join(direc,ddkfilt)
if not os.path.exists(filtfile):
    urllib.request.urlretrieve("https://github.com/strawpants/GRACE-filter/raw/master/data/DDK/"+ddkfilt,filtfile)



# hdr,incoef=read_shstandard(inputf)



filt=DDKfilter(filtfile)



fin=os.path.join(direc,shfilebase+"lmax60in")
fcheck=os.path.join(direc,shfilebase+"lmax60out")
hdr,coefin=read_shstandard(fin)
hdr,coefchk=read_shstandard(fcheck)
coef=filt(coefin)
tol=1e-22
for i,(cf,cck,sf,sck) in enumerate(zip (coef.C,coefchk.C,coef.S,coefchk.S)):
    if (abs(cf-cck)> tol or abs(sf-sck) >tol):
        raise Exception("Comparison not within tolerance")
    # print(coef.nm(i),cf,cck,sf,sck)
# dat=readBIN(file)

