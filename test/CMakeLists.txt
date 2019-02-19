cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

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
  identifier
  must
  number
  number_generative
  quote
  range
  repetition
  sequence
  until
  whitespace
  )

foreach(NAME ${TESTS})
  get_filename_component(DIRNAME ${NAME} DIRECTORY)
  get_filename_component(BASENAME ${NAME} NAME_WE)
  string(REPLACE / _ CATNAME ${NAME})
  
  set(TEST_NAME test_${CATNAME})
  set(TEST_FILE ${PROJECT_SOURCE_DIR}/test/src/${DIRNAME}/test_${BASENAME}.cpp)
  if(EXISTS ${TEST_FILE})
  else()
    set(TEST_FILE ${PROJECT_SOURCE_DIR}/test/src/${DIRNAME}/test_${CATNAME}.cpp)    
  endif()
  
  add_executable(${TEST_NAME} ${TEST_FILE})
  add_test(${TEST_NAME} bin/${TEST_NAME})
  target_link_libraries(${TEST_NAME} core fmt gtest)
endforeach(NAME)
