LIST(APPEND COREHEADERS core/DimensionBase.hpp core/TimeDimension.hpp core/AnnotedArrayBase.hpp core/OperatorBase.hpp core/InputArchiveBase.hpp
        core/OutputArchiveBase.hpp)
LIST(APPEND COREOBJS core/TimeDimension.cpp)

LIST(APPEND SHHEADERS sh/SHDimension.hpp sh/Legendre_nm.hpp)
LIST(APPEND SHOBJS sh/Legendre_nm.cpp)

if (PYTHON)
    #add python wrapper routines
    list(APPEND CXXPYWRAPS pybindings/coreBindings.cpp)
    list(APPEND CXXPYWRAPS pybindings/SHBindings.cpp )
    add_library(${FROMMLEPYLIB} SHARED ${CXXPYWRAPS}  )
endif()

add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS} ${SHHEADERS} ${SHOBJS} )
target_include_directories(${FROMMLELIB} PUBLIC ${Boost_INCLUDE_DIR} )
#set_target_properties(${FROMMLELIB} PROPERTIES PUBLIC_HEADER ${COREHEADERS})

