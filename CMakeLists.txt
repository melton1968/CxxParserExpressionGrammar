cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(cxx-peg VERSION 0.1 LANGUAGES CXX)

# Make including this file idemnpotent
#
if(TARGET cxx_peg)
  return()
endif()

# Configuration variables.
#
# Build the tests and set directories if this is the master project
#
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(PEG_TEST ON CACHE BOOL "Generate the tests.")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

  # Have cmake output the compilation commands
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# PEG_TEST -- Generate the tests
#
option(PEG_TEST "Generate the tests." OFF)

# Add our dependencies
#
add_subdirectory(extern/cxx-core)

find_package(Threads REQUIRED)

message("-- cxx-peg: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- cxx-peg: Included from: ${CMAKE_SOURCE_DIR}")
message("-- cxx-peg: PEG_TEST ${PEG_TEST}")
message("-- cxx-peg: DEPENDS_BUILD_DIR ${DEPENDS_BUILD_DIR}")
message("-- cxx-peg: DEPENDS_INSTALL_DIR ${DEPENDS_INSTALL_DIR}")

# Build this project
#
add_compile_options(-Wall)
add_compile_options(-std=c++2a)
add_compile_options(-O3)

add_library(cxx_peg src/calculator.cpp)
target_include_directories(cxx_peg PUBLIC include)
target_link_libraries(cxx_peg PUBLIC cxx_core)

set(TOOLS
  peg_t0
  peg_t1
  peg_t2)

foreach(NAME ${TOOLS})
  add_executable(${NAME} src/tools/${NAME}.cpp)
  target_link_libraries(${NAME} PUBLIC cxx_peg)
endforeach()

if(PEG_TEST)
  include_directories(include)
  enable_testing()
  add_subdirectory(test)
endif()
