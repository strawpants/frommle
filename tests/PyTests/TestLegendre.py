from frommle.sh import Legendre_nm
from frommle.sh import i_from_mn, mn_from_i
from pyshtools import legendre
import time
import math
# input("press key to continue..")
nmax=2700
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


# def mn_from_idx(idx,nmax):
#     m=int((3+2*nmax)/2-math.sqrt(math.pow(3+2*nmax,2)/4-2*idx));
#     n=idx-int(((m+1)*(m+2))/2+m*(nmax-m))+m+1
#     return n,m

#test index functions

for m in range(0,nmax+1):
    for n in range(m,nmax+1):
        idx=i_from_mn(n,m,nmax)
        ni,mi=mn_from_i(idx,nmax)
        if ni != n or mi != m:
            print("error:",n,m,ni,mi)


# for n in range(nmax+1):
#     for m in range(n+1):
#         ind = legendre.PlmIndex(n, m)
#         ind2=pnm.indxnm(n,m)
#         print(n,m, ind,ind2,vcval[ind],vc[ind2])
