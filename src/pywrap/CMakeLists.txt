#CMake file which builds a wrapper library which can be imported in python
LIST(APPEND PYWRAPOBJS TimeDimension.cpp)
add_library(${FROMMLEPYLIB} SHARED ${PYWRAPOBJS})
set_target_properties(${FROMMLEPYLIB} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/pyfrommle LIBRARY_OUTPUT_NAME frommle.so)