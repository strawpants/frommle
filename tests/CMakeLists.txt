list(APPEND ALLTESTS GeometryTesting SHtesting GuideTesting coretesting IOtesting GOperatorTesting)

#add all the tests to Ctest
foreach(tester ${ALLTESTS})
    add_executable(${tester} cppTests/${tester}.cpp )
    target_include_directories(${tester} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${tester} ${FROMMLELIB} ${YAML_CPP_LIBRARY} ${GDAL_LIBRARY} ${LIBSECRET_LIBRARIES}
            ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE} ${Boost_LOG_LIBRARY_RELEASE}
            ${Boost_LOG_SETUP_LIBRARY_RELEASE} ${Boost_FILESYSTEM_LIBRARY_RELEASE} ${Boost_IOSTREAMS_LIBRARY_RELEASE}
            ${Boost_SYSTEM_LIBRARY_RELEASE} ${CMAKE_THREAD_LIBS_INIT})
        add_test(NAME ${tester} COMMAND ${tester} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
    #set_target_properties(${tester} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testing)
endforeach(tester ${ALLTESTS})

# add python tests

if (PYTHON)
    
    list(APPEND ALLPYTESTS PyTests/shio_n_conversion.py  PyTests/TestGravFunctionals.py )

    foreach(pytest ${ALLPYTESTS})
        get_filename_component(PYTESTNAME ${pytest} NAME_WE)
        add_test(NAME ${PYTESTNAME} COMMAND ${PYTHON} -m unittest ${pytest} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
        file(COPY ${pytest}  DESTINATION ${CMAKE_BINARY_DIR}/testing/)
        set_target_properties(${tester} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testing)
    endforeach()


endif()


#copy test datasets
list(APPEND TESTDATA testpolygonswkt.txt itsg2018_sample.db  itsg2018_sample_files.tgz )
foreach(dset ${TESTDATA})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/${dset}  DESTINATION ${CMAKE_BINARY_DIR}/testing/data)
endforeach(dset ${TESTDATA})
