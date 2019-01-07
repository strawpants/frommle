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
from frommle.sh import shdata
import math
import numpy as np

def SH2loc(loc,cnm,nmax=None):
    """Performs a SH analysis on a set of points"""
    if not nmax:
        #take the maximum degree from the  input if not set explicitly
        nmax=cnm.nmax

    Legnm=Legendre_nm(nmax)
    out=np.zeros((len(loc)))
    for i,(lon,lat) in enumerate(loc):
       pnm=Legnm(math.sin(lat))
       for m in range(nmax+1):
           istart=cnm.idx(m,m,nmax)
           iend=cnm.idx(nmax,m,nmax)
           out[i]+=math.cos(m*lon)*np.dot(pnm[istart:iend+1],cnm.C[istart:iend+1])
           if m>0:
                out[i]+=math.sin(m*lon)*np.dot(pnm[istart:iend+1],cnm.S[istart:iend+1])

    return out
