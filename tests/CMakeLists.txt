cmake_minimum_required(VERSION 3.11.0)

# Support standalone build (for CI unit-tests job)
if(NOT CMAKE_PROJECT_NAME OR CMAKE_PROJECT_NAME STREQUAL "alizams_tests")
  project(alizams_tests CXX)
  enable_testing()
endif()

set(TEST_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/test_structures.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_buffer_sizes.cpp)

add_executable(alizams_tests ${TEST_SRCS})

target_include_directories(alizams_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(alizams_tests PUBLIC cxx_std_14)

add_test(NAME alizams_tests COMMAND alizams_tests)
