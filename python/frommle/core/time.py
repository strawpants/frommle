from datetime import datetime
def decyear2datetime(decyr):
    year=int(decyr)
    jan1=datetime(year,1,1)
    return jan1+(decyr-year)*(datetime(year+1,1,1)-jan1)

def datetime2decyear(dtime):
    jan1=datetime(dtime.year,1,1)
    yrlen=datetime(dtime.year+1,1,1)-jan1
    return dtime.year+(dtime-jan1).total_seconds()/yrlen.total_seconds()
