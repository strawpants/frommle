# Hints from https://bloerg.net/2012/11/10/cmake-and-distutils.html
#The idea behind this CMakefile is that it essentially adds a makefile target to execute python setup.py which builds python module



if (PYTHON)


    #note: we're going to make an in source build for the python part as it will be easier to develop
   set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup_template.py")
   set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
   set(TSTAMP "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

   #set dependencies (python files and wrappers)
   list(APPEND CXXPYCOLLECTOR ${CMAKE_SOURCE_DIR}/src/pybindings/PYCPPCollect.cpp)
   set(PYDEPS ${CXXPYCOLLECTOR} ${CMAKE_CURRENT_SOURCE_DIR}/frommle/__init__.py ${CMAKE_CURRENT_SOURCE_DIR}/frommle/core/__init__.py)

    add_custom_target(linkpython COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/frommle ${CMAKE_CURRENT_BINARY_DIR}/frommle)

    configure_file(${SETUP_PY_IN} ${SETUP_PY})
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")

        add_custom_command(OUTPUT ${TSTAMP} COMMAND ${PYTHON} ./setup.py develop
                COMMAND ${CMAKE_COMMAND} -E touch ${TSTAMP} DEPENDS linkpython ${PYDEPS} ${FROMMLELIB} ${FROMMLEPYLIB}  )
    else ()
        add_custom_command(OUTPUT ${TSTAMP} COMMAND ${PYTHON} ${SETUP_PY} develop
                COMMAND ${CMAKE_COMMAND} -E touch ${TSTAMP} DEPENDS linkpython ${PYDEPS} ${FROMMLELIB} ${FROMMLEPYLIB} )
    endif ()


    add_custom_target(python DEPENDS ${TSTAMP} ${PYDEPS} ${FROMMLELIB} ${FROMMLEPYLIB} )

    install(CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install )")

   #also make sure the correct folders are clean up upon make clean
   set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "build" "dist" )
 endif()

