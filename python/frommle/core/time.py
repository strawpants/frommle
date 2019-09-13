from datetime import datetime
import numpy as np

def decyear2datetime(decyr):
    year=int(decyr)
    if year == 0:
        return datetime.min
    jan1=datetime(year,1,1)
    return jan1+(decyr-year)*(datetime(year+1,1,1)-jan1)

def datetime2decyear(dtime):
    jan1=datetime(dtime.year,1,1)
    yrlen=datetime(dtime.year+1,1,1)-jan1
    return dtime.year+(dtime-jan1).total_seconds()/yrlen.total_seconds()

def npdt64_2_decyear(dt64):
    return datetime2decyear(npdt64_2_datetime(dt64))

def monthrange(dtime):
    """return the beginning, end and center of a month"""
    yr=dtime.year
    mn=dtime.month
    if mn == 12:
        daysinmon=(datetime(yr+1,1,1)-datetime(yr,mn,1)).days
    else:
        daysinmon=(datetime(yr,mn+1,1)-datetime(yr,mn,1)).days

    if daysinmon % 2 == 0:
        hrs=0
    else:
        hrs=12

    return datetime(yr,mn,1,0,0,0),datetime(yr,mn,int(daysinmon/2),hrs,0,0),datetime(yr,mn,daysinmon,23,59,59)

def npdt64_2_datetime(dt64):
    return datetime.utcfromtimestamp((dt64 - np.datetime64('1970-01-01T00:00:00Z')) / np.timedelta64(1, 's'))
