LIST(APPEND COREHEADERS DimensionBase.hpp TimeDimension.hpp)
LIST(APPEND COREOBJS TimeDimension.cpp)


add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS})