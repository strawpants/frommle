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

from netCDF4 import Dataset

def writeNcGrids(outfile,lon,lat,grids, time=None):
   """Writes out grids in netcdf format in coards convention"""
   with Dataset(outfile, "w", format="NETCDF4") as ncid:
        ncid.history =" Created by writeNCGrids, Roelof Rietbroek"
        ncid.createDimension("lon",len(lon))
        ncid.createDimension("lat",len(lat))
        latv = ncid.createVariable("lat","f4",("lat",))
        lonv = ncid.createVariable("lon","f4",("lon",))
        zv= ncid.createVariable("z","f4",("lat","lon",))

        #assign data
        latv[:]=lat
        lonv[:]=lon
        zv[:]=grids


