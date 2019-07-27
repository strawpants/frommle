# read and write icgem files
import gzip as gz
from datetime import datetime
import os
import logging
import re
import numpy as np
from frommle.sh.shdata import shdata
from datetime import datetime
import math
from frommle.sh import SHtmnGuide

class icgemLineParser():
    def __init__(self,nmax,epoch):
        self.epoch=epoch
        self.nmax=nmax
        self.t0=None
        self.diyr=365.25
        self.regex=re.compile('(^gfc .*)|(^gfct .*)|(^trnd .*)|(^acos .*)|(^asin .*)')
    def __call__(self,ln):
        """Parse a icgem line and return an update to the coefficient"""
        if not self.regex.match(ln):
            return None,None,None,None,None,None

        lnspl=ln.split()
        n=int(lnspl[1])
        if n> self.nmax:
            return None,None,None,None,None,None

        m=int(lnspl[2])
        C=float(lnspl[3])
        S=float(lnspl[4])
        if len(lnspl)> 6:
            sigC=float(lnspl[5])
            sigS=float(lnspl[6])
        else:
            sigC=0
            sigS=0

        if lnspl[0] =='gfc':
            return n,m,C,S,sigC,sigS

        #otherwise consider this as a time variable component
        elif lnspl[0] =='gfct':
            # also read/set the reference time from the last element
            self.t0=datetime.strptime(lnspl[-1],'%Y%m%d')
            return n,m,C,S,sigC,sigS

        if not self.epoch:
            #quick return if no time variable component is requested
            return None,None,None,None,None,None

        if lnspl[0] =='trnd':
            #add a linear trend
            scale=(self.epoch-self.t0).days/self.diyr
            return n,m,scale*C,scale*S,scale*sigC,scale*sigS

        elif lnspl[0] =='acos':
            #add a cosine term
            dt=(self.epoch-self.t0).days/self.diyr
            omega=2*math.pi*float(lnspl[-1])/self.diyr
            scale=math.cos(omega*dt)
            return n,m,scale*C,scale*S,scale*sigC,scale*sigS

        elif lnspl[0] =='asin':
            #add a cosine term
            dt=(self.epoch-self.t0).days/self.diyr
            omega=2*math.pi*float(lnspl[-1])/self.diyr
            scale=math.sin(omega*dt)
            return n,m,scale*C,scale*S,scale*sigC,scale*sigS

def read_icgem(filename, nmax=None, headerOnly=False, epoch=None,shg=None,error=False):
    """Extract meta information from a (possibly gzipped) icgem file"""

    modtime=datetime.fromtimestamp(os.path.getmtime(filename))

    if filename.endswith('.gz'):
        fid=gz.open(filename,'rt')
    else:
        fid=open(filename,'rt')

    #first extract the icgem header
    hdr={}
    logging.info("Extracting header info from %s"%(filename))
    for ln in fid:
        if 'end_of_head' in ln:
            break

        spl=ln.split()
        if len(spl) == 2:
            #insert name value pairs in the hdr dict
            hdr[spl[0]]=spl[1]

    try:
        nmaxsupp=int(hdr["max_degree"])
        if nmax:
            if nmax > nmaxsupp:
                logging.warning("warning nmax requested larger than supported, setting to zero")
        else:
            nmax=nmaxsupp
        if 'format' in hdr:
            format=hdr['format']
        else:
            format='icgem'

        meta={"nmax":nmaxsupp,
              "lastupdate":modtime,
              "format":format,
              "gm":float(hdr["earth_gravity_constant"]),
              "re":float(hdr["radius"]),
              "uri":filename,
              "type":"GSM",
              "data":{"name":hdr["modelname"]}
              }
    except KeyError:
        #some values may not be present but that is ok
        pass

    #add tide system
    try:
        tmp=hdr["tide_system"]
        if re.search('zero_tide',tmp):
            meta["tidesystem"]="zero-tide"
        elif re.search('tide_free',tmp):
            meta["tidesystem"]="tide-free"
    except KeyError:
        pass

    if headerOnly:
        fid.close()
        return meta

    #extract coefficients
    if not shg:
        shg=SHtmnGuide(nmax)
        #also put the sorting of the sh coefficients in the meta information
        meta["shguide"]=shg
    else:
        #take the nmax from the provided guide
        if shg.nmax > nmax:
            logging.warning("warning nmax requested larger than supported, setting to zero")
        nmax=shg.nmax

    shout=np.zeros([shg.size()])
    if error:
        sherr=np.zeros([shg.size()])
    else:
        sherr=None

    if format == "icgem":
        lParser=icgemLineParser(nmax,epoch)
    else:
        raise RuntimeError("The icgem format '%s' is currently unsupported"%(format))

    # loop over remaining lines

    for ln in fid:
        n,m,c,s,sigc,sigs=lParser(ln)
        if not n :
            continue
        idxc=shg.idx(n,m,0)
        shout[idxc]+=c
        if m != 0:
            idxs=shg.idx(n,m,1)
            shout[idxs]+=s
        if error:
            sherr[idxc]=math.sqrt(math.pow(sherr[idxc],2)+math.pow(sigc,2))
            if m != 0:
                sherr[idxs]=math.sqrt(math.pow(sherr[idxs],2)+math.pow(sigs,2))

    return meta,shout