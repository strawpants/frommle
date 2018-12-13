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

# set of Earth constants which can be used as defaults

#mean Earth radius in m
RE = 0.6378136460e+07
# gravitational constant of the earth
GM = 0.3986004415e+15

#gravitational universal constant
GU = 6.6738480e-11

#density of sea water in kg/m^3
rho_w =1.025e+3

#mean density of the Earth
rho_e = 5517.0e+0

#density of (compacted) ice
rho_ice = 931.e0 #kg/m^3 taken from G. Spada and friends

#mean acceleration of the Earth
g=9.80665e0

#ratio of the oceans surface to the total surface (taken from ocean.sh)
A_oce=7.0802102076271e-1

#mean earth rotation (in radians per second) relative to the stars
OHM_E=7.292115467064e-5
#Dynamical flattening from GRS80
GRS80_C20=-4.84166854896120e-04

#Mean moments of inertia from the earth ( kg m^2)
Ixx_A=8.0102e+37  # ~= (I_xx+Iyy)/2
Izz_C=8.0365e+37

#Chandler frequency ( radians per second) from iers constants
Ohm_chandler=1.679064144e-7
