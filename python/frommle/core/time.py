from datetime import datetime
def decyear2datetime(decyr):
    year=int(decyr)
    jan1=datetime(year,1,1)
    return jan1+(decyr-year)*(datetime(year+1,1,1)-jan1)
