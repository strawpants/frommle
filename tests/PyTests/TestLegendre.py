from frommle.sh import Legendre_nm
from frommle.sh import i_from_mn, mn_from_i
from pyshtools import legendre
import time
import math
# input("press key to continue..")
nmax=100
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


def mn_from_idx(idx,nmax):
    tot=i_from_mn(nmax,nmax,nmax)
    m= math.sqrt(math.pow(nmax,2)+2*(nmax-idx-1)+1)+nmax;
    n=0
    return n,m

#test index functions

for m in range(0,nmax+1):
    for n in range(m,nmax+1):
        idx=i_from_mn(n,m,nmax)
        ni,mi=mn_from_idx(idx,nmax)
        print(n,m,ni,mi,idx)


# for n in range(nmax+1):
#     for m in range(n+1):
#         ind = legendre.PlmIndex(n, m)
#         ind2=pnm.indxnm(n,m)
#         print(n,m, ind,ind2,vcval[ind],vc[ind2])
