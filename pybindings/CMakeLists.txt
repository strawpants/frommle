#add python wrapper routines

#core wrappers 
list(APPEND CXXPYWRAPS core/FrommleBindings.cpp)
list(APPEND CXXPYWRAPS core/TreeNodeBindings.cpp)
list(APPEND CXXPYWRAPS core/GuideBaseBindings.cpp
        core/coreGuides.cpp
        core/datetimeconversion.cpp)

list(APPEND CXXPYWRAPS core/coreBindings.cpp 
        core/tupleconversion.cpp
         core/numpyConverting.cpp)

list(APPEND CXXPYHEADWRAPS core/coreBindings.hpp
        core/tupleconversion.hpp core/numpyConverting.hpp)


list(APPEND CXXPYWRAPS core/LoggerBindings.cpp)

list(APPEND CXXPYWRAPS core/GuidePackBindings.cpp)
list(APPEND CXXPYWRAPS core/GArrayBindings.cpp)
list(APPEND CXXPYWRAPS core/GOperatorBindings.cpp)
list(APPEND CXXPYWRAPS core/ExceptionBindings.cpp)

#geometry wrappers

# SH wrappers
list(APPEND CXXPYWRAPS sh/SHBindings.cpp)
list(APPEND CXXPYWRAPS geometry/geometryBindings.cpp)

#earthmodel wrappers
LIST(APPEND CXXPYWRAPS earthmodels/earthBindings.cpp)

#IO wrappers
list(APPEND CXXPYHEADWRAPS io/IOBindings.hpp)
list(APPEND CXXPYWRAPS io/IOBindings.cpp
        io/ArchiveBindings.cpp)



add_library(${FROMMLEPYLIB} SHARED ${CXXPYWRAPS} )
