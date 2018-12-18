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
import numpy as np
import math

class DDKfilter():
    def __init__(self,filtername):
        """Reads  filter coefficients from file"""
        self.coblocks=[]
        self.soblocks=[]
        W=readBIN(filtername)
        if W['type'] != "BDFULLV0":
            raise Exception("Not a appropriate filter matrix")
        lastblckind=0
        lastindex=0
        #unpack matrix in diagonal blocks
        for iblck in range(W['nblocks']):
            #order of the block
            m=int((iblck+1)/2)
            # get the trigonometic part of the block (cosine=0, sin=1)
            if iblck == 0:
                trig=0
            else:
                (dum,trig)=divmod(iblck-1,2)
        #size of the block
            sz=W['blockind'][iblck]-lastblckind

            blockn=np.identity(W['meta']["Lmax"]+1-m)

            nminblk=max(int(W["meta"]["Lmin"]),m)

            shft=nminblk-m

            blockn[shft:,shft:]=W['pack'][lastindex:lastindex+sz*sz].reshape([sz,sz],order='F')
            if trig ==0:
                self.coblocks.append(blockn)
            else:
                self.soblocks.append(blockn)
            lastblckind=W["blockind"][iblck]
            lastindex+=sz*sz

    def __call__(self,incoef):
        """Filter coefficients"""
        pass
