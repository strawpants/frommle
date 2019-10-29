#!/bin/bash
set -e -x

# Install a system package required by our library
#yum install -y boost-devel 


#buid the c++ part of the package library
export FROMMLEBUILD=${PWD}/frommle-build

mkdir -p ${FROMMLEBUILD}
cd ${FROMMLEBUILD}
cmake -DCMAKE_BUILD_TYPE=Release /io/

# build the python library
make python

cd ..

## Compile wheels
#for PYBIN in /opt/python/*/bin; do
    #"${PYBIN}/pip" install -r /io/dev-requirements.txt
    ##"${PYBIN}/pip" wheel /io/ -w wheelhouse/
#done

## Bundle external shared libraries into the wheels
#for whl in wheelhouse/*.whl; do
    #auditwheel repair "$whl" --plat $PLAT -w /io/wheelhouse/
#done

## Install packages and test
#for PYBIN in /opt/python/*/bin/; do
    #"${PYBIN}/pip" install python-manylinux-demo --no-index -f /io/wheelhouse
    #(cd "$HOME"; "${PYBIN}/nosetests" pymanylinuxdemo)
#done
