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

from frommle.earthmodels import LoadLove
from frommle.gravity.earth import rho_w,rho_e,RE
from frommle.sh.shdata import shdata
import numpy as np

class Stokes2eqh():
    """Operator which converts Stokes coefficients to equivalent water height"""
    nmax_=0
    def __init__(self,loadlovefile, nmax=None,refsys='CF'):
        """Initializes Load love numbers and kernel (i.e. read from file)"""
        ll = LoadLove(loadlovefile, nmax)
        ll.setRefSystem(refsys)
        self.nmax_=len(ll.hdat)-1
        #compute the isotropic kernel
        self.kernel=np.zeros([self.nmax_+1])
        fact=RE*rho_e/(3*rho_w)
        for n in range(self.nmax_+1):
            self.kernel[n]=fact*(2*n+1)/(1+ll.kdat[n])

    def __call__(self,cnm,sigcnm=None):
        """Convert a given set of Stokes coefficients"""

        #check whether the input sizes agree
        if cnm.nmax != self.nmax_:
            raise Exception("maximum degree does not agree with input")
        cnmout=shdata(self.nmax_)

        if sigcnm :
            sigcnmout=shdata(self.nmax_)


        for m in range(self.nmax_):
            istart=cnm.idx(m,m)
            iend=cnm.idx(self.nmax_,m)
            # multiply order wise
            np.multiply(self.kernel[m:self.nmax_+1],cnm.C[istart:iend+1],out=cnmout.C[istart:iend+1])
            np.multiply(self.kernel[m:self.nmax_+1],cnm.S[istart:iend+1],out=cnmout.S[istart:iend+1])
            if sigcnm:
                np.multiply(self.kernel[m:self.nmax_+1],sigcnm.C[istart:iend+1],out=sigcnmout.C[istart:iend+1])
                np.multiply(self.kernel[m:self.nmax_+1],sigcnm.S[istart:iend+1],out=sigcnmout.S[istart:iend+1])

        if sigcnm and sigsnm:
            return cnmout,sigcnmout
        else:
            return cnmout

