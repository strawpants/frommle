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
from datetime import datetime
import sys
import copy
from collections import namedtuple


class GeoTimeEnvelope():
    """Class which holds a geographical bounding box, a vertical depth range and a datetime range"""

    w=0.0
    e=360.0
    n=90.0
    s=-90.0
    tmin=datetime.min
    tmax=datetime.max
    zmin=sys.float_info.min
    zmax=sys.float_info.max

    def __init__(self,wsne=None, tspan=None,zspan=None):
        if wsne:
            self.w=wsne[0]
            self.s=wsne[1]
            self.n=wsne[2]
            self.e=wsne[3]
        if tspan:
            self.tmin=tspan[0]
            self.tmax=tspan[1]
        if zspan:
            self.zmin=zspan[0]
            self.zmax=zspan[1]

        self.check()

    def toGreenwhich(self):
        """returns an instance with the longitude coordinates to span -180 .. 180"""
        if self.e > 180:
            self.e-=360
        if self.w > 180:
            self.w-=360
        self.check()

    def to0_360(self):
        """Change the longitude coordinates to span 0 .. 360"""
        if self.e <= 0:
            self.e+=360
        if self.w < 0:
            self.w+=360
        self.check()

    def polygon(self):
        """Retrieves an array with the boundingbox ccordinates as a closed polygon"""
        return [[self.w,self.s],[self.w,self.n],[self.e,self.n],[self.e,self.s],[self.w,self.s]]

    def toWKT(self):
        """Export boundary to well known text format"""
        return 'POLYGON (('+",".join(["%f %f"%(x,y) for (x,y) in self.polygon()])+'))'

    def tspan(self):
        return [self.tmin,self.tmax]

    def check(self):
        """Check if the ranges are valid """
        if not (self.s < self.n and self.w < self.e and self.tmin < self.tmax and self.zmin < self.zmax):
            raise ValueError("invalid Bounding box, time range or , z-range")

        if ( self.w < 0 and self.e > 180):
            raise ValueError("Inconsistent longitudes, mixing 0..360 with -180..180 bounds")

    def lonSplit(self,lon=0):
        """returns 2 bounding boxes from the current one split at a longitude
        """
        if lon < self.w or self.e < lon:
            raise RuntimeError("Splitting longitude not within the bounding box")
        left=copy.deepcopy(self)
        left.e=lon
        right=copy.deepcopy(self)
        right.w=lon
        return left,right

    def timeSplit(self,t=None):
        """return 2 bounding boxes split up at a certain time point"""
        if not t:
            t=self.ts+(self.te-self.ts)/2
        if t < self.ts or self.te < t:
            raise RuntimeError("Splitting Time dimension not within timerange")
        before=copy.deepcopy(self)
        before.tmax=t
        after=copy.deepcopy(self)
        after.tmin=t
        return before,after

    def isGMTCentered(self):
        if ( self.w < 0 ) or (self.e < 0):
            return True
        else:
            return False

    def crop(self,btdbox):
        """Crops the bounding box based on the limits available in an other"""

        if self.s < btdbox.s:
            self.s=btdbox.s

        if btdbox.n < self.n:
            self.n=btdbox.n

        if self.w < btdbox.w:
            self.w=btdbox.w

        if btdbox.e < self.e:
            self.e=btdbox.e

        if self.zmin < btdbox.zmin:
            self.zmin=btdbox.zmin

        if btdbox.zmax < self.zmax:
            self.zmin=btdbox.zmax

        if self.tmin < btdbox.tmin:
            self.tmin=btdbox.tmin

        if btdbox.tmax < self.tmax:
            self.tmax=btdbox.tmax