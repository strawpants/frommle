# Hints from https://bloerg.net/2012/11/10/cmake-and-distutils.html
#The idea behind this CMakefile is that it essentially adds a makefile target to execute python setup.py which builds python module
find_program(PYTHON "python3")

if (PYTHON) 
   set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
   set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py") 
   set(TSTAMP "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")
   #set dependencies (python files and wrappers) 
   set(PYDEPS "${CMAKE_CURRENT_SOURCE_DIR}/frommle/__init__.py") 
   set(CPPDEPS "${CMAKE_CURRENT_SOURCE_DIR}/frommle/dims/dimBindings.cpp") 
   
   

    configure_file(${SETUP_PY_IN} ${SETUP_PY})

    add_custom_command(OUTPUT ${TSTAMP} COMMAND ${PYTHON} ${SETUP_PY} build
       COMMAND ${CMAKE_COMMAND} -E touch ${TSTAMP} DEPENDS ${PYDEPS} ${CPPDEPS}))

       add_custom_target(python ALL DEPENDS ${TSTAMP} ${FROMMLELIB} )

    install(CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install)")
 endif()

