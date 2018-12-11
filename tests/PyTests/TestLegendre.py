from frommle.sh import Legendre_nm
from pyshtools import legendre
import time
import math
# input("press key to continue..")
nmax=2000
z=[math.cos(x*math.pi/180) for x in range(180)]
t1=time.time()
pnm=Legendre_nm(nmax)
for zz in z:
    vc=pnm(zz)
t2=time.time()
print("elapsed time c++ implementation",t2-t1)

t1=time.time()
for zz in z:
    vcval=legendre.PlmBar(nmax, zz)
t2=time.time()

print("elapsed time fortran implementation",t2-t1)

# for n in range(nmax+1):
#     for m in range(n+1):
#         ind = legendre.PlmIndex(n, m)
#         ind2=pnm.indxnm(n,m)
#         print(n,m, ind,ind2,vcval[ind],vc[ind2])
