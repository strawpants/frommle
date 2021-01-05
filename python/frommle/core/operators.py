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

# Author Roelof Rietbroek (roelof@wobbly.earth), 2020


# an Y=aX+Y operator which updates a vector/matrix by a scaled vector



from frommle.core import GOperator_float64


class GarrSub():
    """Binary operator which substracts to garray's from each other while respecting the guides sequence"""
    def __init__(self):
        pass


    def __call__(self,gar1,gar2):
        # get common part of the guides
        gs1,gs2=getCommonGuides(gar1.gp(0),gar2.gp(1))

        return None
