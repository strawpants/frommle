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

from datetime import datetime
from frommle.core.usersettings import usersettings
#some tools to help working with nectdf files whioch obey the CF convention


def setCFGlobalAttr(ncid,description=None,scriptname="Unknown"):
    ncid.Conventions="CF-1.7"
    
    if hasattr(ncid,"history"):
        ncid.history+="\n Adapted on %s by %s"%(datetime.now(),scriptname)
    else:
        ncid.history="Created on %s by %s"%(datetime.now(),scriptname)
    if description:
        ncid.description=description

    uset=usersettings()
    if "User" in uset:
        ncid.author=uset["User"]
    
    if "Institution" in uset:
        ncid.institution=uset["Institution"]
    
    if "Contact" in uset:
        ncid.contact=uset["Contact"]




    return ncid

def createCFlon(ncid,lon):
    ncid.createDimension("lon", len(lon))
    var=ncid.createVariable("lon","f8",("lon",))
    var.long_name="longitude"
    var.standard_name="longitude"
    var.units="degrees_east"
    var[:]=lon
    return var

def createCFlat(ncid,lat):
    ncid.createDimension("lat", len(lat))
    var=ncid.createVariable("lat","f8",("lat",))
    var.long_name="latitude"
    var.standard_name="latitude"
    var.units="degrees_north"
    var[:]=lat
    return var

def createCFtime(ncid,time):
    ncid.createDimension("time", len(time))
    var=ncid.createVariable("time","i8",("time",))
    var.long_name="time"
    var.standard_name="time"
    var.units="years since 0000-1-1 0:0:0"
    var.calendar="365_day"
    # import pdb;pdb.set_trace()
    var[:]=time
    return var

def createCFAuxVar(ncid,data,name,long_name=None, units=None):
    #find out existing dimensions
    lookup={}
    for ky,sz in ncid.dimensions.items():
        lookup[sz.size]=ky

    dimlist=[]
    for sz in data.shape:
        try:
            print(sz,lookup)
            dimlist.append(lookup[sz])
        except:
            raise RuntimeError("Dimension can not be matched with existing dimensions, please provide an explicit lookup")


    var=ncid.createVariable(name,"f8",tuple(dimlist))
    if long_name:
        var.long_name=long_name
    if units:
        var.units=units
    var[...]=data
    return var
    

