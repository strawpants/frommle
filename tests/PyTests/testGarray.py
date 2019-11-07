from frommle.core.garray import makeGArray
from frommle.core import IndexGuide,monthlyRange

gar=makeGArray(monthlyRange(2011,4,2019,4),IndexGuide(5))

xr=gar.DataArray()


print(xr)
