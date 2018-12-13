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
import os
from frommle.gravity import Stokes2eqh
from frommle.sh import i_from_mn,mn_from_i,nmax_from_sz
from frommle.sh.analysis import SH2loc
import numpy as np
import math
loadlovef=os.path.join(os.environ['RLFTLBX_DATA'],'love','PREM.love')
nmax=100
conv=Stokes2eqh(loadlovef,nmax)

#prepare dummy input
sz=i_from_mn(nmax,nmax,nmax)+1
cnm=np.ones((sz,))
snm=np.ones((sz,))

nmaxout=nmax_from_sz(cnm.shape[0])
#convert coefficients
cnmout,snmout=conv(cnm,snm)

d2r=math.pi/180


locations=[(10,53),(20,89.8)]
locations=[(d2r*lon,d2r*lat) for lon,lat in locations]

out=SH2loc(locations,cnm,snm)

for (lon,lat),val in zip(locations,out):
    print(lon/d2r,lat/d2r,val)

# for idx,(c,s) in enumerate(zip(cnmout,snmout)):
#     n,m=mn_from_i(idx,nmax)
#     print(n,m,c,s)



