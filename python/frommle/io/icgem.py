# read and write icgem files
import gzip as gz
from datetime import datetime
import os
import logging
import re
import numpy as np
from frommle.sh import SHtmnDim
i_from_mn=SHtmnDim.i_from_mn

def read_icgem(filename, nmax=None, headerOnly=False):
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
        nmax=nmaxsupp

        meta={"nmax":nmaxsupp,
              "lastupdate":modtime,
              "format":"icgem",
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

    # also extract the coefficients
    sz=i_from_mn(nmax,nmax,nmax)+1
    body={'nm':np.zeros([sz],dtype=(int,2)),'C':np.zeros([sz]),'S':np.zeros([sz]),'sigC':np.zeros([sz]),'sigS':np.zeros([sz])}
    # loop over remaining lines
    gfcregex=re.compile('^gfc .*')
    for ln in fid:
        if gfcregex.match(ln):
            lnspl=ln.split()
            n=int(lnspl[1])
            if n> nmax:
                continue

            m=int(lnspl[2])
            idx=i_from_mn(n,m,nmax)
            body['nm'][idx]=(n,m)
            body['C'][idx]=float(lnspl[3])
            body['S'][idx]=float(lnspl[4])
            if len(lnspl)> 6:
                body['sigC'][idx]=float(lnspl[5])
                body['sigS'][idx]=float(lnspl[6])


    return meta,body