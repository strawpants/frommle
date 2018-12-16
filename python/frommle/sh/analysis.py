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
from frommle.sh import Legendre_nm
from frommle.sh import nmax_from_sz
import math
from frommle.sh import SHtmnDim
i_from_mn=SHtmnDim.i_from_mn
import numpy as np
def SH2loc(loc,cnm,snm):
    """Performs a SH analysis on a set of points"""
    nmax=nmax_from_sz(cnm.shape[0])
    Legnm=Legendre_nm(nmax)
    out=np.zeros((len(loc)))
    for i,(lon,lat) in enumerate(loc):
       pnm=Legnm(math.sin(lat))
       for m in range(nmax+1):
           istart=i_from_mn(m,m,nmax)
           iend=i_from_mn(nmax,m,nmax)
           out[i]+=math.cos(m*lon)*np.dot(pnm[istart:iend+1],cnm[istart:iend+1])
           if m>0:
                out[i]+=math.sin(m*lon)*np.dot(pnm[istart:iend+1],snm[istart:iend+1])

    return out
