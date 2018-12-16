#Top level Cmake configuration for the Frommle library
cmake_minimum_required(VERSION 2.8)

project(Frommle)

#set some options
option(USE_MPI "Build with MPI enabled" OFF)


#make sure that we're using c++11 (unfortunately this variable is not supported with cmake <3 .1)
##set(CMAKE_CXX_STANDARD 11)

#check gnu compiler version (should be larger pr equal than 4.9
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.9
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        message(FATAL_ERROR "GCC version must be at least 4.9!")
    endif()
endif()

#basic compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -std=c++11")
#set(CMAKE_CXX_FLAGS_DEBUG "-pg  -g -O3") #to use when the gprof profiler is used for performance testing
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Winline -ansi -pedantic -Wextra -Weffc++ -Wno-long-long -std=c++11")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++11")

#add make info
set(CMAKE_VERBOSE_MAKEFILE on)


#set the name of the library to be build
set(FROMMLELIB "Frommle")
set(FROMMLEPYLIB "Frommle_pybind")

set(FROMMLELIBMAJORVERSION 0)
set(FROMMLELIBMINORVERSION 1)

#where to put build executables
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#where to put archives and the libraries
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)



#make some choices depending on whether this is going to be a MPI enabled build
if( ${USE_MPI} )
    message("searching for MPI toolchain")
    find_package(MPI REQUIRED)
    set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_CXX_COMPILE_FLAGS})
    set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_CXX_LINK_FLAGS} )
    include_directories(${MPI_C_INCLUDE_PATH})
    set(WITH_BOOST_MPI mpi)
    #set a preprocessor macro to be passed 
    add_definitions(-DUSE_MPI)
endif( ${USE_MPI} )


# see if python3 is installed
find_program(PYTHON "python3")
if (PYTHON)
   execute_process(COMMAND python3 -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())" OUTPUT_VARIABLE PYTHON_INCLUDE_DIR)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)
#find prerequisites (BLAS & LAPACK)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

#try finding an optimizedBLAS library
#find_package(OPTBLAS REQUIRED)

#We need the boost library for various things (e.g. timing and dates)
#find_package(Boost REQUIRED COMPONENTS system chrono program_options date_time serialization ${WITH_BOOST_MPI} iostreams)
find_package(Boost REQUIRED COMPONENTS system python3 numpy3 unit_test_framework date_time ${WITH_BOOST_MPI})

#add Boost include path
include_directories(${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})

#find the hdf5 library
#find_package(HDF5 REQUIRED COMPONENTS CXX)
##set a preprocessor FLAG depending on the version
#if(${HDF5_VERSION} VERSION_LESS 1.10.0 )
   #add_definitions(-DHDF5OLD)
#endif(${HDF5_VERSION} VERSION_LESS 1.10.0 )



#find the netcdf library
#find_package(NetCDF REQUIRED )

#find GDAL library (for reading shapefiles etc)
#find_package(GDAL REQUIRED)
#include_directories(${GDAL_INCLUDE_DIR})

#where to copy public library header files
set(FROMMLE_HEADER_DIRECTORY ${PROJECT_BINARY_DIR}/include)
#define some PATHS to essential components of the package
set(FROMMLE_LIB_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/lib)



#setup testing
enable_testing()
add_subdirectory(tests)

#directory with c++ source code
add_subdirectory(src)

#add directory with python and C++ wrappers
add_subdirectory(python)

#Directory with documentation (Using Doxygen and Sphinx)
add_subdirectory(docsrc)



