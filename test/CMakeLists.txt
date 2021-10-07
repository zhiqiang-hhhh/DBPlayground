
file(GLOB MINIKV_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")

######################################################################################################################
# MAKE TARGETS
######################################################################################################################

##########################################
# "make check-tests"
##########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

##########################################
# "make XYZ_test"
##########################################
foreach (minikv_test_source ${MINIKV_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(bustub_test_filename ${minikv_test_source} NAME)
    string(REPLACE ".cpp" "" minikv_test_name ${bustub_test_filename})

    # Add the test target separately and as part of "make check-tests".
    # EXCLUDE_FROM_ALL 表示不直接编译当前的可执行文件，需要后续通过 make XXX 来手动指定编译
    # 最后的参数表示不将 EXCLUDE_FROM_ALL 应用在该文件上，即 xxx.cpp 一定会被编译
    # add_executable(${minikv_test_name} EXCLUDE_FROM_ALL ${minikv_test_source} storage/b_plus_tree_concurrent_2_test.cpp )
    #    add_executable(${minikv_test_name} EXCLUDE_FROM_ALL ${minikv_test_source})
    add_executable(${minikv_test_name} ${minikv_test_source})
    add_dependencies(build-tests ${minikv_test_name})
    add_dependencies(check-tests ${minikv_test_name})

    target_link_libraries(${minikv_test_name} miniKV_lib gtest gmock_main)

    # Set test target properties and dependencies.
    set_target_properties(${minikv_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${minikv_test_name}
            )

    # Add the test under CTest.
    add_test(${minikv_test_name} ${VALGRIND_BIN} ${VALGRIND_OPTIONS}
            --suppressions=${VALGRIND_SUPPRESSIONS_FILE} ${CMAKE_BINARY_DIR}/test/${minikv_test_name}
            --gtest_color=yes --gtest_output=xml:${CMAKE_BINARY_DIR}/test/unit_${test_name}.xml)

    add_test(${minikv_test_name} ${CMAKE_BINARY_DIR}/test/${minikv_test_name} --gtest_color=yes
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${minikv_test_name}.xml)

endforeach (minikv_test_source ${BUSTUB_TEST_SOURCES})