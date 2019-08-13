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
from sys import maxsize
import re

class LoadLove:
    def __init__(self, filename, nmax=None):
        if not nmax:
            #unlimited maximum degree for a start
            nmax=maxsize

        commentreg = re.compile('^#')
        fid = open(filename, 'r')
        self.hdat = []
        self.ldat = []
        self.kdat = []
        for line in fid:
            if commentreg.match(line):
                continue
            else:
                # parse line
                linespl = line.split()
                deg = int(linespl[0])
                if deg > nmax:
                    break
                if deg >= len(self.hdat):
                    #exctend the lists so the requested degree can be inserted
                    self.hdat.extend([0 for x in range(len(self.hdat), deg + 1)])
                    self.ldat.extend([0 for x in range(len(self.ldat), deg + 1)])
                    self.kdat.extend([0 for x in range(len(self.kdat), deg + 1)])

                # the input is a fortran file which has a double floating point notation with e.g.
                # 'D+11' notation which can not be parsed by python
                self.hdat[deg] = float(linespl[1].replace('D', 'E'))
                self.ldat[deg] = float(linespl[2].replace('D', 'E'))
                self.kdat[deg] = float(linespl[3].replace('D', 'E'))
        # make sure the right degree 1 coefficients are set
        self.setRefSystem('CF')

    # set the reference system of the degree 1 loading Love numbers
    def setRefSystem(self, refsys):
        self.refsys = refsys
        # compute isomorphic frame parameter
        if refsys == 'CF':
            alpha = (self.hdat[1] + 2 * self.ldat[1]) / 3
        elif refsys == 'CM':
            alpha = 1 + self.kdat[1]
        elif refsys == 'CE':
            alpha = self.kdat[1]
        elif refsys == 'CH':
            alpha = self.hdat[1]
        elif refsys == 'CL':
            alpha = self.ldat[1]

        # apply isomorphic frame parameter
        self.hdat[1] -= alpha
        self.ldat[1] -= alpha
        self.kdat[1] -= alpha
