LIST(APPEND COREHEADERS core/DimensionBase.hpp core/TimeDimension.hpp)
LIST(APPEND COREOBJS core/TimeDimension.cpp)


add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS})
#set_target_properties(${FROMMLELIB} PROPERTIES PUBLIC_HEADER ${COREHEADERS})
