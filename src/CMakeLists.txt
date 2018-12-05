LIST(APPEND COREHEADERS core/DimensionBase.hpp core/TimeDimension.hpp core/AnnotedArrayBase.hpp core/OperatorBase.hpp core/InputArchiveBase.hpp
        core/OutputArchiveBase.hpp)
LIST(APPEND COREOBJS core/TimeDimension.cpp)

LIST(APPEND SHHEADERS sh/SHDimension.hpp sh/Legendre_nm_naive.hpp)
LIST(APPEND SHOBJS sh/Legendre_nm_naive.cpp)

if (PYTHON)
    #add python wrapper routines
    list(APPEND CXXPYWRAPS pybindings/dimBindings.cpp)
    list(APPEND CXXPYWRAPS pybindings/SHDimensionBindings.cpp)
endif()

add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS} ${SHHEADERS} ${SHOBJS} ${CXXPYWRAPS}  )
target_include_directories(${FROMMLELIB} PUBLIC ${Boost_INCLUDE_DIR} )
#set_target_properties(${FROMMLELIB} PROPERTIES PUBLIC_HEADER ${COREHEADERS})

