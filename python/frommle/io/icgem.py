# read and write icgem files
import gzip as gz
from datetime import datetime
import os
import logging
import re
import numpy as np
from frommle.sh import SHnmtGuide,trig
from frommle.io.shArchive import SHArchive
from frommle.io import Variable
from frommle.io.numpyVariable import np_float64Var

from datetime import datetime
import math

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
            return None,None,None,None,None,None,None

        lnspl=ln.split()
        n=int(lnspl[1])
        if n> self.nmax:
            return None,None,None,None,None,None,None

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
            return "gfc",n,m,C,S,sigC,sigS

        #otherwise consider this as a time variable component
        elif lnspl[0] =='gfct':
            # also read/set the reference time from the last element
            self.t0=datetime.strptime(lnspl[-1],'%Y%m%d')
            return "gfct",n,m,C,S,sigC,sigS

        if not self.epoch:
            #quick return if no time variable component is requested
            return None,None,None,None,None,None

        if lnspl[0] =='trnd':
            #add a linear trend
            scale=(self.epoch-self.t0).days/self.diyr
            return "trnd",n,m,scale*C,scale*S,scale*sigC,scale*sigS

        elif lnspl[0] =='acos':
            #add a cosine term
            dt=(self.epoch-self.t0).days/self.diyr
            omega=2*math.pi*float(lnspl[-1])/self.diyr
            scale=math.cos(omega*dt)
            return "acos",n,m,scale*C,scale*S,scale*sigC,scale*sigS

        elif lnspl[0] =='asin':
            #add a cosine term
            dt=(self.epoch-self.t0).days/self.diyr
            omega=2*math.pi*float(lnspl[-1])/self.diyr
            scale=math.sin(omega*dt)
            return "asin",n,m,scale*C,scale*S,scale*sigC,scale*sigS

class SHicgemArchive(SHArchive):
    def __init__(self,filen,mode='r',nmax=-1):
        super().__init__(filen,mode,nmax)

    def fload_impl(self):
        """Loads the data into variables"""
        with self.fid() as fid:
            #first extract the icgem header
            hdr={}
            for ln in fid:
                if 'end_of_head' in ln:
                    break

                spl=ln.split()
                if len(spl) == 2:
                    #insert name value pairs in the hdr dict
                    hdr[spl[0]]=spl[1]

            #extract relevant parameters from the headers
            try:
                nmaxsupp=int(hdr["max_degree"])
                self.attr["nmaxfile"]=nmaxsupp
                if "nmax" in self.attr:
                    self.attr["nmax"]=nmaxsupp
                nmax=self.attr["nmax"]

                if nmax > nmaxsupp:
                    logging.warning("warning nmax requested larger than supported, higher degree coefficients will be set to zero")


                if 'format' in hdr:
                    self.attr["format"]=hdr['format']
                else:
                    self.attr["format"]="icgem"

                self.attr["gm"]=float(hdr["earth_gravity_constant"]),
                self.attr["re"]=float(hdr["radius"])
                self.attr["modelname"]=hdr["modelname"]
            except KeyError:
            #some values may not be present but that is ok
                pass

            #add tide system
            try:
                tmp=hdr["tide_system"]
                if re.search('zero_tide',tmp):
                    self.attr["tidesystem"]="zero-tide"
                elif re.search('tide_free',tmp):
                    self.attr["tidesystem"]="tide-free"
            except KeyError:
                pass

            shg=self.shg_c(nmax)
            sz=shg.size()
            for ln in fid:

            cnm=np.zeros([sz])
            sigcnm=np.zeros([sz])

            self["cnm"]=np_float64Var(cnm)
            self["shg"]=Variable(shg)

            if self.attr["format"] == "icgem":
                lParser=icgemLineParser(nmax,epoch)
            else:
                raise RuntimeError("The icgem format '%s' is currently unsupported"%(self.attr["format"]))

            witherrors=False

            if witherrors:
                self["sigcnm"]=np_float64Var(sigcnm)



    # #extract coefficients
    # if not shg:
    #     shg=SHtmnGuide(nmax)
    #     #also put the sorting of the sh coefficients in the meta information
    #     meta["shguide"]=shg
    # else:
    #     #take the nmax from the provided guide
    #     if shg.nmax > nmax:
    #         logging.warning("warning nmax requested larger than supported, setting to zero")
    #     nmax=shg.nmax
    #
    # shout=np.zeros([shg.size()])
    # if error:
    #     sherr=np.zeros([shg.size()])
    # else:
    #     sherr=None
    #
    # if format == "icgem":
    #     lParser=icgemLineParser(nmax,epoch)
    # else:
    #     raise RuntimeError("The icgem format '%s' is currently unsupported"%(format))
    #
    # # loop over remaining lines
    #
    # for ln in fid:
    #     n,m,c,s,sigc,sigs=lParser(ln)
    #     if not n :
    #         continue
    #     idxc=shg.idx(n,m,0)
    #     shout[idxc]+=c
    #     if m != 0:
    #         idxs=shg.idx(n,m,1)
    #         shout[idxs]+=s
    #     if error:
    #         sherr[idxc]=math.sqrt(math.pow(sherr[idxc],2)+math.pow(sigc,2))
    #         if m != 0:
    #             sherr[idxs]=math.sqrt(math.pow(sherr[idxs],2)+math.pow(sigs,2))
    #
    # return meta,shout
    #     witherrors=False
    #     with self.fid() as fid:
    #         ln = fid.readline()
    #         lnspl = ln.split()
    #
    #         if len(lnspl) == 6:
    #             witherrors=True
    #
    #         self.attr["nmaxfile"]=int(lnspl[1])
    #         if not "nmax" in self.attr:
    #             self.attr["nmax"]=self.attr["nmaxfile"]
    #         tags=["tstart","tcent","tend"]
    # try:
    #     nmaxsupp=int(hdr["max_degree"])
    #     if nmax:
    #         if nmax > nmaxsupp:
    #             logging.warning("warning nmax requested larger than supported, setting to zero")
    #     else:
    #         nmax=nmaxsupp
    #
    #     if 'format' in hdr:
    #         self.attr["format"]=hdr['format']
    #     else:
    #         self.attr["format"]="icgem"
    #
    #     self.attr["gm"]=float(hdr["earth_gravity_constant"]),
    #     self.attr["re"]=float(hdr["radius"])
    #     self.attr["modelname"]=hdr["modelname"]
    # except KeyError:
    #     #some values may not be present but that is ok
    #     pass
    #
    # #add tide system
    # try:
    #     tmp=hdr["tide_system"]
    #     if re.search('zero_tide',tmp):
    #         self.attr["tidesystem"]="zero-tide"
    #     elif re.search('tide_free',tmp):
    #         self.attr["tidesystem"]="tide-free"
    # except KeyError:
    #     pass
    #
    #         for ky,val in zip(tags,lnspl[2:5]):
    #             decyr=float(val)
    #             if decyr > 0:
    #                 self.attr[ky]=decyear2datetime(decyr)
    #
    #         nmax=self.attr["nmax"]
    #         shg=self.shg_c(nmax)
    #         sz=shg.size()
    #
    #         cnm=np.zeros([sz])
    #         sigcnm=np.zeros([sz])
    #
    #         for ln in fid:
    #             lnspl = ln.split()
    #
    #             n = int(lnspl[0])
    #             if n > nmax:
    #                 continue
    #
    #             m = int(lnspl[1])
    #
    #             idxc=shg.idx((n, m,trig.c))
    #             idxs=shg.idx((n,m,trig.s))
    #
    #             # import pdb;pdb.set_trace()
    #             cnm[idxc]=float(lnspl[2])
    #             if m!=0:
    #                 cnm[idxs]=float(lnspl[3])
    #
    #
    #             if witherrors:
    #                 sigcnm[idxc]=float(lnspl[2])
    #                 if m!=0:
    #                     sigcnm[idxs]=float(lnspl[3])
    #
    #         #set variables and assign ndarray's to them
    #
    #         self["cnm"]=np_float64Var(cnm)
    #         self["shg"]=Variable(shg)
    #         if witherrors:
    #             self["sigcnm"]=np_float64Var(sigcnm)

    def fsave_impl(self):
        pass
        # cnm=self["cnm"][:]
        # sigcnm=self["sigcnm"][:]
        # shg=self["shg"].value
        #
        # tstamps=[0.0,0.0,0.0]
        # for i,tag in enumerate(["tstart","tcent","tend"]):
        #     if tag in self.attr:
        #         tstamps[i]=datetime2decyear(self.attr[tag])
        #
        # with self.fid() as fid:
        #     fid.write("META   %d %f %f %f\n"%(shg.nmax,tstamps[0],tstamps[1],tstamps[2]))
        #
        #
        #     # for val in enumerate(shg):
        #     #     print(val)
        #     idxsorted=sorted(enumerate(shg),key=lambda val:val[1])
        #     if sigcnm:
        #         lineval=[0.0,0.0,0.0,0.0]
        #         for i,(n,m,t) in idxsorted:
        #             lineval[t]=cnm[i]
        #             lineval[2+t]=sigcnm[i]
        #             if t == trig.s:
        #                 if m ==0:
        #                     lineval[t]=0.0
        #                     lineval[2+t]=0.0
        #                 #print out line
        #                 fid.write("%5d %5d %14.10e %14.10e %14.10e %14.10e\n"%(n,m,*lineval))
        #     else:
        #         lineval=[0.0,0.0]
        #         for i,(n,m,t) in idxsorted:
        #             lineval[t]=cnm[i]
        #             if t == trig.s:
        #                 if m ==0:
        #                     lineval[t]=0.0
        #                 #print out line
        #                 fid.write("%5d %5d %14.10e %14.10e\n"%(n,m,*lineval[0:2]))

# def read_icgem(filename, nmax=None, headerOnly=False, epoch=None,shg=None,error=False):
#     """Extract meta information from a (possibly gzipped) icgem file"""
#
#     modtime=datetime.fromtimestamp(os.path.getmtime(filename))
#
#     if filename.endswith('.gz'):
#         fid=gz.open(filename,'rt')
#     else:
#         fid=open(filename,'rt')
#
#     #first extract the icgem header
#     hdr={}
#     logging.info("Extracting header info from %s"%(filename))
#     for ln in fid:
#         if 'end_of_head' in ln:
#             break
#
#         spl=ln.split()
#         if len(spl) == 2:
#             #insert name value pairs in the hdr dict
#             hdr[spl[0]]=spl[1]
#

    # #extract coefficients
    # if not shg:
    #     shg=SHtmnGuide(nmax)
    #     #also put the sorting of the sh coefficients in the meta information
    #     meta["shguide"]=shg
    # else:
    #     #take the nmax from the provided guide
    #     if shg.nmax > nmax:
    #         logging.warning("warning nmax requested larger than supported, setting to zero")
    #     nmax=shg.nmax
    #
    # shout=np.zeros([shg.size()])
    # if error:
    #     sherr=np.zeros([shg.size()])
    # else:
    #     sherr=None
    #
    # if format == "icgem":
    #     lParser=icgemLineParser(nmax,epoch)
    # else:
    #     raise RuntimeError("The icgem format '%s' is currently unsupported"%(format))
    #
    # # loop over remaining lines
    #
    # for ln in fid:
    #     n,m,c,s,sigc,sigs=lParser(ln)
    #     if not n :
    #         continue
    #     idxc=shg.idx(n,m,0)
    #     shout[idxc]+=c
    #     if m != 0:
    #         idxs=shg.idx(n,m,1)
    #         shout[idxs]+=s
    #     if error:
    #         sherr[idxc]=math.sqrt(math.pow(sherr[idxc],2)+math.pow(sigc,2))
    #         if m != 0:
    #             sherr[idxs]=math.sqrt(math.pow(sherr[idxs],2)+math.pow(sigs,2))
    #
    # return meta,shout