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


# from frommle.sh import SHtmnGuide
# from frommle.sh import shxarray
#
# nmax=5
# shx=shxarray(nmax)
#
# print(shx)
# for (n,m,t) in shg:
#     print(n,m,t)


from frommle.core import IndexGuide,DateGuide,makeGuidePack
from frommle.sh import SHnmtGuide
from datetime import datetime


gp=makeGuidePack(IndexGuide(12),DateGuide(),SHnmtGuide(3))


gp[1].append(datetime(2002,2,17))
coords=gp.coords(0)
print(coords)
coords=gp.coords(1)
print(coords)
