list(APPEND ALLTESTS GeometryTesting SHtesting coretesting )

#add all the tests to Ctest
foreach(tester ${ALLTESTS})
    add_executable(${tester} cppTests/${tester}.cpp )
    target_include_directories(${tester} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${tester} ${FROMMLELIB} ${YAML_CPP_LIBRARY} ${GDAL_LIBRARY} ${LIBSECRET_LIBRARIES}
            ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE} ${Boost_LOG_LIBRARY_RELEASE}
            ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE} ${Boost_LOG_SETUP_LIBRARY_RELEASE}
            ${Boost_SYSTEM_LIBRARY_RELEASE} ${CMAKE_THREAD_LIBS_INIT})
    add_test(NAME ${tester} COMMAND ${tester})
    set_target_properties(${tester} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testing)
endforeach(tester ${ALLTESTS})
