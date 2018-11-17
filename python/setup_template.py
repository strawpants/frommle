#template for setup.py where cmake may substitute some project variables
import setuptools
from setuptools import Extension,find_packages

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
        Extension("frommle._core", sources=["${CXXPYWRAPS}"],
                  library_dirs=["${Boost_LIBRARY_DIR_RELEASE}"],
                  libraries = ["boost_python3","boost_system"],
                  include_dirs=["${Boost_INCLUDE_DIR}"])
    ],
)