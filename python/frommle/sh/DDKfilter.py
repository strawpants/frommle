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
from frommle.sh.shdata import shdata
import math

class DDKfilter():
    def __init__(self,filtername,transpose=False):
        """Reads  filter coefficients from file
        Transpose causes the filter to be applied in its transpose form (e.g. needed for filtering basin before
        basin averaging operations)"""


        W=readBIN(filtername)
        if W['type'] != "BDFULLV0":
            raise Exception("Not a appropriate filter matrix")
        self.nmax=int(W['meta']['Lmax'])
        self.blocks=[]
        self.mtindx=[]

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

            self.mtindx.append((m,trig))
            self.blocks.append(np.identity(W['meta']["Lmax"]+1-m))

            nminblk=max(int(W["meta"]["Lmin"]),m)

            shft=nminblk-m

            #note the blocks are in fact stored as Fortran style column major order
            #but we load them here pretending them to be row major so that the np.dot operation acts from __call()
            #acts correctly unless transpose is set to true
            if transpose:
                transp='F'
            else:
                transp='C'

            self.blocks[-1][shft:,shft:]=W['pack'][lastindex:lastindex+sz*sz].reshape([sz,sz],order=transp)

            lastblckind=W["blockind"][iblck]
            lastindex+=sz*sz

    def __call__(self,incoef):
        """Filter coefficients"""

        if incoef.nmax > self.nmax:
            raise ValueError("Maximum degree of filter matrix is smaller than the maximum input degree")
        shfilt=shdata(incoef.nmax)
        #loop over the filter blocks
        for (m,trig),block in zip(self.mtindx,self.blocks):
            if m> incoef.nmax:
                break
            st=shfilt.idx(m,m)
            nd=shfilt.idx(incoef.nmax,m)+1
            ndblck=incoef.nmax-m+1
            if trig == 0:
                np.dot(incoef.C[st:nd],block[:ndblck,:ndblck],out=shfilt.C[st:nd])
            else:
                np.dot(incoef.S[st:nd],block[:ndblck,:ndblck],out=shfilt.S[st:nd])

        return shfilt
