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

import numpy as np
from frommle.sh import SHtmnDim

class shdata():
    """Contains a class which holds a SH dataset and some index functions"""
    nmax=0
    def __init__(self,nmax):
        self.nmax=nmax
        sz = SHtmnDim.i_from_mn(nmax, nmax, nmax) + 1
        self.C=np.zeros([sz])
        self.S=np.zeros([sz])
        self.sigC=np.zeros([sz])
        self.sigS=np.zeros([sz])

    def idx(self,n,m):
        return SHtmnDim.i_from_mn(n,m,self.nmax)

    def nm(self,idx):
        return SHtmnDim.mn_from_i(idx,self.nmax)