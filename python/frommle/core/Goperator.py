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


from frommle.core import GOperatorBase

class Goperator(GOperatorBase):
    coords={}
    """A class which converts data from one dimension to the other"""
    def __init__(self,coords=None, dims=None, name="GOperator"):
        super().__init__(name)


        if coords:
            #assign coordinate dimensions
            if isinstance(coords,dict):
                #directly
                self.coords=coords
            else:
                for dname,coord in zip(dims,coords):
                    self.coords[dname]=coord

            if len(self.coords) != self.ndim:
                raise RuntimeError("Amount of coordinate values for GOperator must be %d"%self.ndim)
