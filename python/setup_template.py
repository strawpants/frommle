#template for setup.py where cmake may substitute some project variables
import setuptools
from setuptools import Extension,find_packages
import os
# def splitWrappers(instr):
#     #returns a list of all *cpp from the given cmake string
#     cppfiles=[]
#     for cpp in instr.split(";"):
#         cppfiles.append(cpp)
#     return cppfiles

def getLibraryAlias(libpath):
    """Extracts the library name without the preceding lib and suffixes"""
    return os.path.basename(libpath).split('.')[0].replace('lib','')

with open("${CMAKE_SOURCE_DIR}/README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="frommle",
    author="Roelof Rietbroek",
    author_email="roelof@wobbly.earth",
    description="Python interface to the geodetic Earth Science toolbox frommle",
    long_description=long_description,
    url="https://wobbly.earth/frommle",
    packages=find_packages("."),
    package_dir={"":"."},
    classifiers=["Programming Language :: Python :: 3",
        "Programming Language :: C++",
        "License :: OSI Approved :: GNU Lesser General Public License v3 (LGPLv3)",
        "Operating System :: POSIX :: Linux",
        "Topic :: Scientific/Engineering",
        "Development Status :: 1 - Planning"],
    ext_modules=[
        Extension("frommle._cpp", sources=["${CXXPYCOLLECTOR}"],
                  library_dirs=["${Boost_LIBRARY_DIR_RELEASE}","${PROJECT_BINARY_DIR}/lib" ],
                  libraries = ["${FROMMLEPYLIB}","${FROMMLELIB}",getLibraryAlias("${Boost_PYTHON3_LIBRARY}"),getLibraryAlias("${Boost_NUMPY3_LIBRARY}"),getLibraryAlias("${Boost_SYSTEM_LIBRARY}"),getLibraryAlias("${GDAL_LIBRARY}")],
                  include_dirs=["${Boost_INCLUDE_DIR}","${CMAKE_SOURCE_DIR}"],
                  runtime_library_dirs = ["${PROJECT_BINARY_DIR}/lib"])
    ],
)
