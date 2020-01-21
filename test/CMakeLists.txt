add_subdirectory("${PROJECT_SOURCE_DIR}/external/googletest" "external/googletest")
macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} gdu)
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER test)
    set_target_properties(${TESTNAME} PROPERTIES FOLDER test)
    target_compile_options(${TESTNAME} PRIVATE -Werror -Wall -Wextra)
endmacro()

package_add_test(small_map_test small_map_test.cc)
package_add_test(simple_config scvalue_test.cc scparse_test.cc)
