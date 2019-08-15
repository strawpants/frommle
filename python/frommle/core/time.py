from datetime import datetime
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