LIST(APPEND COREHEADERS core/DimensionBase.hpp core/TimeDimension.hpp core/AnnotedArrayBase.hpp core/OperatorBase.hpp core/InputArchiveBase.hpp
        core/OutputArchiveBase.hpp)
LIST(APPEND COREOBJS core/TimeDimension.cpp)

LIST(APPEND SHHEADERS sh/SHDimension.hpp )

add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS} ${SHHEADERS} )
#set_target_properties(${FROMMLELIB} PROPERTIES PUBLIC_HEADER ${COREHEADERS})
