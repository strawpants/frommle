set(TESTBASE SHtesting)
add_executable(${TESTBASE} cppTests/${TESTBASE}.cpp )
target_include_directories(${TESTBASE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TESTBASE} ${FROMMLELIB} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE})
list(APPEND ALLTESTS ${TESTBASE})

#add all the tests to Ctest
foreach(tester ${ALLTESTS})
    add_test(NAME ${tester} COMMAND ${tester})
    set_target_properties(${tester} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testing)
endforeach(tester ${ALLTESTS})
