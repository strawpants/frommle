## Adds applications which are part of the default installation

LIST(APPEND COMMANDLINEHELPERHEADERS cppApps/commandLineParsing.hpp )
LIST(APPEND COMMANDLINEHELPEROBJS cppApps/commandLineParsing.cpp )

LIST(APPEND CPPAPPS frommleinfo )



#add all the executables to the installation
foreach(app ${CPPAPPS})
    add_executable(${app} cppApps/${app}.cpp ${COMMANDLINEHELPEROBJS} )
    target_include_directories(${app} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${app} ${FROMMLELIB} ${YAML_CPP_LIBRARY} ${GDAL_LIBRARY} ${LIBSECRET_LIBRARIES}
            ${Boost_LOG_LIBRARY_RELEASE} ${Boost_LOG_SETUP_LIBRARY_RELEASE} ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE} ${Boost_SYSTEM_LIBRARY_RELEASE}
            ${CMAKE_THREAD_LIBS_INIT})
    set_target_properties(${app} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endforeach(app ${CPPAPPS})

