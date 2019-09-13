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

# Author Roelof Rietbroek (roelof@geod.uni-bonn.de), 2019

from frommle.core import Goperator
import numpy as np
from frommle.sh import SHtmnGuide
import copy
import scipy.sparse as sps
from frommle.sh.shxarray import newshxarray

class shIsoOperator(Goperator):
    """Converts a spherical harmonic dataset into another using an isotropic kernel"""
    kernel=None
    def __init__(self, kernel):
        """Initialize the operator with a list of degree dependent factors"""
        self.kernel=kernel

    def __call__(self,shxin):
        """Apply kernel to input xarrar.DataArray"""
        #expand the degree wise coefficients to a diagonal matrix with the same sorting as the input
        diag=sps.diags([self.kernel[n] for n,m,tr in shxin["SHGuide"].values])
        
        #create a new matrix to fill with new values
        otherdim=shxin.dims[1]
        shxout=newshxarray(shxin.guidepack[0],auxcoords={otherdim:shxin[otherdim].values})
        
        #apply the sparse matrix mulitplication
        shxout.values=diag.dot(shxin.values)
        return shxout

    def chain(self,roperator):
        """Apply an operator from the right and return the resulting operator"""
        if type(roperator) != type(self):
            return shIsoOperator(np.multiply(roperator.kernel,self.kernel))
        raise NotImplementedError("righthchaining with operator %s is not implemented"%(type(roperator)))

    def inv(self):
        """returns the inverse operator"""
        return shIsoOperator(np.reciprocal(self.kernel))


