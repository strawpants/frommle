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

# Author Roelof Rietbroek (roelof@geod.uni-bonn.de), 2018

import numpy as np
from frommle.sh import SHtmnGuide,trig
from frommle.sh.shxarray import newshxarray

from frommle.core.time import decyear2datetime

def read_shstandard(file, nmax=None, headerOnly=False,error=False):
    """Reads the standard SH format as produce by the RLFTLBX (first line has a META tag, with the maximum degree and start,center and end time)"""


    with open(file, 'rt') as fid:
        ln = fid.readline()
        lnspl = ln.split()
        meta = {"nmax": int(lnspl[1]), "tstart": decyear2datetime(float(lnspl[2])),
                "tcent": decyear2datetime(float(lnspl[3])), "tend": decyear2datetime(float(lnspl[4]))}

        if headerOnly:
            return meta,None,None,None

        if not nmax:
            nmax=meta["nmax"]

        shcoef=[]
        if error:
            sherr=[]
        else:
            sherr=None
        
        # import pdb;pdb.set_trace()
        idx=[]
        
        for ln in fid:
            lnspl = ln.split()
            n = int(lnspl[0])
            if n > nmax:
                continue

            m = int(lnspl[1])
            idx.append((n, m,trig.c))

            # import pdb;pdb.set_trace()
            shcoef.append(float(lnspl[2]))
            if m != 0:
                idx.append( (n, m, trig.s))
                shcoef.append(float(lnspl[3]))
            if error and len(lnspl) > 5:
                sherr.append(float(lnspl[4]))
                if m != 0:
                    sherr.append(float(lnspl[5]))
    # pdb.set_trace()
    return meta,idx,shcoef,sherr

def write_shstandard(file,idx, shcoef,sherr=None,meta=None):
    """Write a dataset of spherical harmonic coeficients to a 'standard' sh file"""
    
    #create an index vector which is used to fix the 
    nmax=clm.shape[0]-1
    with open(file,'wt') as fid:
        fid.write("META   %d %f %f %f\n"%(nmax,tstamps[0],tstamps[1],tstamps[2]))
        for n in range(nmax+1):
            for m in range(n+1):
                fid.write("%d %d %e %e\n"%(n,m,clm[n,m],slm[n,m]))
