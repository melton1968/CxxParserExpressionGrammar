cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

add_dependencies(cxx_depends cxx_googletest)
include(${DEPENDS_SOURCE_DIR}/googletest.cmake)

set(TESTS
  at
  block
  character
  clang/comment
  clang/constant
  clang/expression
  clang/floating_constant
  clang/header
  clang/identifier
  clang/keyword
  clang/string
  # cst/calc1
  cst/node
  # cst/transform
  identifier
  keyword
  must
  number
  number_generative
  expr/apply
  expr/apply_rt
  expr/get_children
  quote
  range
  repetition
  sequence
  string
  until
  whitespace
  )

foreach(NAME ${TESTS})
  get_filename_component(DIRNAME ${NAME} DIRECTORY)
  get_filename_component(BASENAME ${NAME} NAME_WE)
  string(REPLACE / _ CATNAME ${NAME})
  
  set(TEST_NAME test_${CATNAME})
  set(TEST_FILE1 ${PROJECT_SOURCE_DIR}/test/src/${DIRNAME}/test_${BASENAME}.cpp)
  set(TEST_FILE2 ${PROJECT_SOURCE_DIR}/test/src/${DIRNAME}/test_${CATNAME}.cpp)
  if(EXISTS ${TEST_FILE1})
    set(TEST_FILE ${TEST_FILE1})
  elseif(EXISTS ${TEST_FILE2})
    set(TEST_FILE ${TEST_FILE2})
  else()
    message(FATAL "Cannot find test file for ${NAME}")
  endif()

  list(APPEND TEST_TARGETS ${TEST_NAME})
  add_executable(${TEST_NAME} ${TEST_FILE})
  add_test(${TEST_NAME} bin/${TEST_NAME})
  target_link_libraries(${TEST_NAME} PUBLIC cxx_peg googletest Threads::Threads c++)
  target_include_directories(${TEST_NAME} PUBLIC ../include/core)
endforeach(NAME)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TEST_TARGETS})
