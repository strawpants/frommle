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
    def stokes2eqh(self,shg=None):
        """returns a diagonal matrix with coefficients to convert from Stokes coefficients to equivalent water height"""
        pass

    def stokes2Geoid(self,shg):
        pass

    def stokes2Uplift(selfself,shg):
        pass


