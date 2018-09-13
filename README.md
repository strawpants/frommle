# Frommle
Frommle will be a Python/C++ software suite for geodesy and Earth Sciences. This will be the LGPL v3 version of the software

More detailed information can later be found [here (work in progress)](https://wobbly.earth/frommle) 

## Quick installation
**Clone this repository**

`
git@github.com:strawpants/frommle.git
`

**Create a build directory somewhere and change to it e.g.:**

`
mkdir /opt/frommle
cd /opt/frommle
`

**Let cmake prepare makefiles and a python setup.py**
Tune the options for cmake according to your needs and execute cmake:

`
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr /path/to/frommlegitclone
`

**Build the C++ library and tools**

`
make
`

**Optionally Build the python package**

`
make python
`

**Optionally install stuff**

`
make install
`