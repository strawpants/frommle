# read and write icgem files
import gzip as gz
from datetime import datetime
import os
import logging
import re
import numpy as np
from frommle.sh import i_from_mn

def read_icgem(filename,headerOnly=False):
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
        meta={"nmax":int(hdr["max_degree"]),
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
    sz=
    body=np.zeros([10,10])

    return meta,body