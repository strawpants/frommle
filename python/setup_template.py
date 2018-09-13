#template for setup.py where cmake may substitute some project variables
import setuptools
from setuptools import Extension

with open("${CMAKE_SOURCE_DIR}/README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="frommle",
    author="Roelof Rietbroek",
    author_email="roelof@wobbly.earth",
    description="Python interface to the geodetic Earth Science toolbox frommle",
    long_description=long_description,
    url="https://wobbly.earth/frommle",
    packages=['core'],
    classifiers=(
        "Programming Language :: Python :: 3",
        "Programming Language :: C++",
        "License :: OSI Approved :: GNU Lesser General Public License v3 (LGPLv3)",
        "Operating System :: POSIX :: Linux",
        "Topic :: Scientific/Engineering",
        "Development Status :: 1 - Planning",
    ),
    ext_modules=[
        Extension("_core", sources=["core/dimBindings.cpp"],
                  library_dirs=["${Boost_LIBRARY_DIRS}"],
                  libraries = ["${Boost_python_LIBRARY}"],
                  include_dirs=["${Boost_INCLUDE}"])
    ],
)