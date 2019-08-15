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
import xarray as xr
import numpy as np
from frommle.core import GuideBase
def newshxarray(shg,meta=None,auxcoords=None):
        dims=["SHGuide"]
        coords={"SHGuide":np.array([x for x in shg],dtype=[("n",'i8'),("m",'i8'),("t","O")])}
        coordattr={"guidepack":[shg]}
        shp=[shg.size()]
        # import pdb;pdb.set_trace()
        if meta:
            for ky,val in meta.items():
                coordattr[ky]=val
        if auxcoords:
            for ky,colist in auxcoords.items():
                dims.append(ky)
                coords[ky]=colist
                shp.append(len(colist))
                #the other non-Guide coordinates are tagged as GuideBase for the time being
                coordattr["guidepack"].append(GuideBase())

        return xr.DataArray(np.zeros(shp),dims=dims,coords=coords,attrs=coordattr)



