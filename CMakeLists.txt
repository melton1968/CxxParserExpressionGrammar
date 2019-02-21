cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(cxx-peg VERSION 0.1 LANGUAGES CXX)

set(MASTER_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MASTER_PROJECT ON)
endif()

option(PEG_TEST "Generate the tests." ${MASTER_PROJECT})
option(PEG_FMT "Build fmt subproject." ${MASTER_PROJECT})
option(PEG_GTEST "Build googletest subproject." ${MASTER_PROJECT})
option(PEG_CORE "Build core subproject." ${MASTER_PROJECT})

if(PEG_FMT)
  add_subdirectory(./extern/fmt)
endif()

if(PEG_GTEST)
  add_subdirectory(./extern/googletest)
endif()

if(PEG_CORE)
  add_subdirectory(./extern/cxx-core)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_compile_options(-Wall)
add_compile_options(-std=c++2a)
add_compile_options(-fconcepts)
add_compile_options(-O3)

add_executable(calculator src/calculator.cpp)
target_link_libraries(calculator core fmt)

set(TOOLS
  t0
  t1
  t2)

foreach(NAME ${TOOLS})
  add_executable(${NAME} src/tools/${NAME}.cpp)
  target_link_libraries(${NAME} core stl2 fmt)
endforeach()

if(PEG_TEST)
  include_directories(include)
  enable_testing()
  add_subdirectory(test)
endif()
