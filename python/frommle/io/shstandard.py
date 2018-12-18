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
from frommle.core.time import decyear2datetime
from frommle.sh import SHtmnDim
from frommle.sh.shdata import shdata


def read_shstandard(file, nmax=None, headerOnly=False):
    """Reads the standard SH format as produce by the RLFTLBX (first line has a META tag)"""
    with open(file, 'rt') as fid:
        ln = fid.readline()
        lnspl = ln.split()
        meta = {"nmax": int(lnspl[1]), "tstart": decyear2datetime(float(lnspl[2])),
                "tcent": decyear2datetime(float(lnspl[3])), "tend": decyear2datetime(float(lnspl[4]))}

        if headerOnly:
            return meta

        # also extract the body
        nmax=meta['nmax']
        shout=shdata(nmax)
        for ln in fid:
            lnspl = ln.split()
            n = int(lnspl[0])
            if n > nmax:
                continue

            m = int(lnspl[1])
            idx = shout.idx(n, m)

            shout.C[idx] = float(lnspl[2])
            shout.S[idx] = float(lnspl[3])
            if len(lnspl) > 5:
                shout.sigC[idx] = float(lnspl[4])
                shout.sigS[idx] = float(lnspl[5])

    return meta,shout
