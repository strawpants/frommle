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
from io import StringIO
import yaml
import gzip as gz
from datetime import datetime
import os
import re
import logging
from frommle.sh import SHtmnGuide
i_from_mn=SHtmnGuide.i_from_mn
import numpy as np
from frommle.io.shArchive import SHArchive

class SHGSMv6Archive(SHArchive):
    def __init__(self,filen,mode='r',nmax=-1):
        super().__init__(filen,mode,nmax)

    def fload_impl(self):
        """Loads the data into variables"""

        witherrors=False
        with self.fid() as fid:
            ln = fid.readline()
            lnspl = ln.split()

            if len(lnspl) == 6:
                witherrors=True

            self.attr["nmaxfile"]=int(lnspl[1])
            if not "nmax" in self.attr:
                self.attr["nmax"]=self.attr["nmaxfile"]
            tags=["tstart","tcent","tend"]
            for ky,val in zip(tags,lnspl[2:5]):
                decyr=float(val)
                if decyr > 0:
                    self.attr[ky]=decyear2datetime(decyr)

            nmax=self.attr["nmax"]
            shg=self.shg_c(nmax)
            sz=shg.size()

            cnm=np.zeros([sz])
            sigcnm=np.zeros([sz])

            for ln in fid:
                lnspl = ln.split()

                n = int(lnspl[0])
                if n > nmax:
                    continue

                m = int(lnspl[1])

                idxc=shg.idx((n, m,trig.c))
                idxs=shg.idx((n,m,trig.s))

                # import pdb;pdb.set_trace()
                cnm[idxc]=float(lnspl[2])
                if m!=0:
                    cnm[idxs]=float(lnspl[3])


                if witherrors:
                    sigcnm[idxc]=float(lnspl[2])
                    if m!=0:
                        sigcnm[idxs]=float(lnspl[3])

            #set variables and assign ndarray's to them

            self["cnm"]=np_float64Var(cnm)
            self["shg"]=Variable(shg)
            if witherrors:
                self["sigcnm"]=np_float64Var(sigcnm)

    def fsave_impl(self):
        raise NotImplementedError("cannot (currently) save to GSMv6 format")



# def readGSMv06(filename,  nmax=None, headerOnly=False):
#     """Reads the header and optionally the Stokes coefficients from a GRACE file (release 06)"""
#     buf=StringIO()
#     fid=gz.open(filename,'rt')
#     for ln in fid:
#         if '# End of YAML header' in ln:
#             break
#         else:
#             buf.write(ln)
#     hdr=yaml.safe_load(buf.getvalue())["header"]
#     nonstand=hdr["non-standard_attributes"]
#
#
#     meta={"nmax":hdr["dimensions"]["degree"],
#           "omax":hdr["dimensions"]["order"],
#           "tstart":hdr["global_attributes"]["time_coverage_start"],
#           "tend":hdr["global_attributes"]["time_coverage_end"],
#           "lastupdate":datetime.fromtimestamp(os.path.getmtime(filename)),
#           "format":nonstand["format_id"]["short_name"],
#           "gm":nonstand["earth_gravity_param"]["value"],
#           "re":nonstand["mean_equator_radius"]["value"],
#           "type":nonstand["product_id"][0:3],
#           "data":{"description":hdr["global_attributes"]["title"]}
#           }
#
#     #add tide system
#     try:
#         tmp=nonstand["permanent_tide_flag"]
#         if re.search('inclusive',tmp):
#             meta["tidesystem"]="zero-tide"
#         elif re.search('exclusive'):
#             meta["tidesystem"]="tide-free"
#     except:
#         pass
#
#     if headerOnly:
#         return meta
#     if not nmax:
#         nmax=meta["nmax"]
#
#     if nmax > meta["nmax"]:
#         logging.warning("Warning: requested degree higher than supported, setting to zero")
#
#
#     sz=i_from_mn(nmax,nmax,nmax)+1
#     body={'nm':np.zeros([sz],dtype=(int,2)),'C':np.zeros([sz]),'S':np.zeros([sz]),'sigC':np.zeros([sz]),'sigS':np.zeros([sz])}
#     #continue reading body
#     dataregex=re.compile('^GRCOF2')
#     for ln in fid:
#         if dataregex.match(ln):
#             lnspl=ln.split()
#             n=int(lnspl[1])
#             if n> nmax:
#                 continue
#
#             m=int(lnspl[2])
#             idx=i_from_mn(n,m,nmax)
#             body['nm'][idx]=(n,m)
#             body['C'][idx]=float(lnspl[3])
#             body['S'][idx]=float(lnspl[4])
#             if len(tmp)> 6:
#                 body['sigC'][idx]=float(lnspl[5])
#                 body['sigS'][idx]=float(lnspl[6])
#
#     fid.close()
#
#     return meta,body
#
