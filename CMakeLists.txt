cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

add_subdirectory(./extern/googletest)
add_subdirectory(./extern/cxx-core)
enable_testing()

include_directories(BEFORE SYSTEM /opt/local/include)
include_directories(include)

link_directories(BEFORE /opt/local/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_compile_options(-Wall)
add_compile_options(-std=c++2a)
add_compile_options(-fconcepts)
add_compile_options(-O3)

set(TESTS
  character_parser
  control
  peg_repetition
  peg_sequence
  peg_whitespace
  range_parser
  tree
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
