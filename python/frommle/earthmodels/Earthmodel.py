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


from frommle.earthmodels import LoadLove
from frommle.earthmodels import WGS84,rho_water,rho_earth
from frommle.sh import shIsoOperator
import numpy as np

class EarthModel():
    """Abstract class which models a spherical Earth model"""
    name_="EarthModel"
    def __init__(self,name):
        name_=name


class SNREI(EarthModel):
    lln=None
    #use asymptotic behavior of the Love numbers
    asymp=False
    def __init__(self, loadLovefile,nmax=None):
        super().__init__("SNREI")
        if not nmax:
            asymp=True
            lln=LoadLove(loadLovefile)
        else:
            lln=LoadLove(loadLovefile,nmax)
    
    def stokes2eqh(self):
        """returns a diagonal matrix with coefficients to convert from Stokes coefficients to equivalent water height"""
        kernel=np.ones([self.nmax+1])
        #create the isotropic kernel
        scale=WGS84.a*rho_earth/(3*rho_water)
        kernel=np.array([scale*(2*n+1)/(1+self.lln.kdat[n]) for n in range(self.nmax+1)])
        return shIsoOperator(kernel)
         

    def stokes2Geoid(self):
        """"Use Brun's Formula to convert from normalized Stokes coefficients to Geoid height"""
        kernel=WGS84.a*np.ones([self.nmax+1])
        return shIsoOperator(kernel)
    
    def stokes2Uplift(self):
        kernel=WGS84.a*np.array([self.lln.hdat/(1+self.lln.kdat)])
        return shIsoOperator(kernel)


