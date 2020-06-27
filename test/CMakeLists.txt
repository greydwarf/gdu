add_subdirectory("${PROJECT_SOURCE_DIR}/external/googletest" "external/googletest")
option(ASAN "Turn on address sanitizer" Off)
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
    target_compile_options(${TESTNAME} PRIVATE -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wold-style-cast -Wcast-align 
                                               -Wunused -Woverloaded-virtual -Wconversion -Wno-sign-conversion -Wmisleading-indentation 
                                               -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast 
                                               -Wdouble-promotion -Wformat=2 -Wstrict-null-sentinel -Wfloat-equal -Wcast-qual 
                                               -Wzero-as-null-pointer-constant -Wlogical-op -Wimplicit-fallthrough=5 -Wctor-dtor-privacy 
                                               -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wformat-overflow 
                                               -Wformat-security -Wformat-signedness -Wuseless-cast -Wredundant-decls)
    if (ASAN)
        target_compile_options(${TESTNAME} PRIVATE -fsanitize=address)
        target_link_libraries(${TESTNAME} "-lasan")
    endif()
endmacro()

package_add_test(test_flat_map test_flat_map.cpp)
package_add_test(test_simple_config test_scvalue.cpp test_scparse.cpp test_scobject.cpp)
