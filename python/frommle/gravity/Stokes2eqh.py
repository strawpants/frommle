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
from frommle.sh import i_from_mn
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

    def __call__(self,cnm,snm,sigcnm=None,sigsnm=None):
        """Convert a given set of Stokes coefficients"""

        #check whether the input sizes agree
        if cnm.shape[0]-1 != i_from_mn(self.nmax_,self.nmax_,self.nmax_):
            raise Exception("maximum degree does not agree with input")

        cnmout=np.zeros(cnm.shape)
        snmout=np.zeros(snm.shape)

        if sigcnm and sigsnm:
            sigcnmout=np.zeros(sigcnm.shape)
            sigsnmout=np.zeros(sigsnm.shape)


        for m in range(self.nmax_):
            istart=i_from_mn(m,m,self.nmax_)
            iend=i_from_mn(self.nmax_,m,self.nmax_)
            # multiply order wise
            np.multiply(self.kernel[m:self.nmax_+1],cnm[istart:iend+1],out=cnmout[istart:iend+1])
            np.multiply(self.kernel[m:self.nmax_+1],snm[istart:iend+1],out=snmout[istart:iend+1])
            if sigcnm and sigsnm:
                np.multiply(self.kernel[m:self.nmax_+1],sigcnm[istart:iend+1],out=sigcnmout[istart:iend+1])
                np.multiply(self.kernel[m:self.nmax_+1],sigsnm[istart:iend+1],out=sigsnmout[istart:iend+1])

        if sigcnm and sigsnm:
            return cnmout,snmout,sigcnmout,sigsnmout
        else:
            return cnmout,snmout

