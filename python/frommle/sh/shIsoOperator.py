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

class shIsoOperator(Goperator):
    """Converts a spherical harmonic dataset into another using an isotropic kernel"""
    kernel=None
    scale=1.0
    fromunit=None
    tounit=None
    def __init__(self, nmax):
        shg=SHtmnGuide(nmax)
        shgar=np.array([x for x in shg],dtype=[("n",'i8'),("m",'i8'),("t","O")])
        super().__init__(coords={"nmout":shgar,"nmin":shgar})

    def __call__(self,shxin):
        """Apply kernel to input data"""
        pass

    def chainright(self,roperator):
        """Apply an operator from the right and return the resulting operator"""
        pass

    def inv(self):
        """returns the inverse operator"""
        pass

class Stokes2Stokes(shIsoOperator):
    """Isotropic operator which converts Stokes coefficients from one normalization (GM, a) to another"""
    def __init__(self, Earthmodel=None):
        pass

class Stokes2eqh(shIsoOperator):
    """Isotropic operator which converts Stokes coefficients to equivalent water height coefficients (in meter equivalent water height)"""
    def __init__(self, Earthmodel=None):
        pass

class Stokes2Geoid(shIsoOperator):
    """Isotropic operator which converts Stokes coefficients to geoid height"""
    def __init__(self):
        pass


class Stokes2Updef(shIsoOperator):
    """Isotropic operator which converts Stokes coefficients to vertical uplift from elastic loading"""
    def __init__(self):
        pass

class Stokes2Hordef(shIsoOperator):
    """Isotropic operator which converts Stokes coefficients to horizontal deformation component from elastic loading"""
    def __init__(self):
        pass
