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

# Author Roelof Rietbroek (roelof@wobbly.earth), 2019


from frommle.core import makeGArray_float64,makeGArray_uint64
from frommle.core.guidepack import makeGuidePack
import numpy as np



def makeGArray(*guides,**kwargs):
    if "dtype" in kwargs:
         dt=kwargs["dtype"]
    else:
        dt=np.float64

    if "name" in kwargs:
        name=kwargs["name"]
    else:
        name="data"

    if dt == np.float64:
        return makeGArray_float64(makeGuidePack(*guides),name)

    if dt == np.dtype('uint64'):
        return makeGArray_uint64(makeGuidePack(*guides),name)

    raise NotImplementedError("cannot (yet) create a GArray with dtype %s"%str(dt))




