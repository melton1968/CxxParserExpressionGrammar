
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

macro(package_add_test TESTNAME)
  add_executable(${TESTNAME} ${ARGN})
  target_link_libraries(${TESTNAME} gtest gmock gtest_main)
  target_include_directories(${TESTNAME} PUBLIC "../include")
  target_compile_features(${TESTNAME} PUBLIC cxx_std_17)
  add_test(${TESTNAME} ${TESTNAME})
  set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(test_character_parser test_character_parser.cpp)
package_add_test(test_control test_control.cpp)
package_add_test(test_tree test_tree.cpp)
