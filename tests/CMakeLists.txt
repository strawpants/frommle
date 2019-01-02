list(APPEND ALLTESTS GeometryTesting SHtesting coretesting )

#add all the tests to Ctest
foreach(tester ${ALLTESTS})
    add_executable(${tester} cppTests/${tester}.cpp )
    target_include_directories(${tester} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${tester} ${FROMMLELIB} ${GDAL_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE})
    add_test(NAME ${tester} COMMAND ${tester})
    set_target_properties(${tester} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testing)
endforeach(tester ${ALLTESTS})
