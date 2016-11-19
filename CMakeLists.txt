cmake_minimum_required (VERSION 2.8)
project (bvh-parser CXX)

# enable C and C++14 language
enable_language (C CXX)
set (CMAKE_CXX_STANDARD 14)

# set project version
set (BVH_PARSER_MAJOR_VERSION 0)
set (BVH_PARSER_MINOR_VERSION 1)
set (BVH_PARSER_PATCH_VERSION 0)
set (BVH_PARSER_VERSION
    "${BVH_PARSER_MAJOR_VERSION}.\
     ${BVH_PARSER_MINOR_VERSION}.\
     ${BVH_PARSER_PATCH_VERSION}")

# loaction of logger configuration file
set (EASYLOGGING_CONFIG_FILE_PATH
    "CMAKE_CURRENT_SOURCE_DIR/easylogging++.config")

# location of additional cmake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# get git commit hash
include(git-commit-hash)

get_directory_property(HAS_BVH_PARSER_PARENT PARENT_DIRECTORY)

# if bvh-parser is standalone project or is sub-project
if(HAS_BVH_PARSER_PARENT)
  # detect operating system
  message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
      add_definitions(-DSYSTEM_LINUX)
  endif()
  if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      add_definitions(-DSYSTEM_WINDOWS)
  endif()

  # detect host processor
  message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# configure header file
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/config.h.in
    ${PROJECT_BINARY_DIR}/config.h)

set (BVH_PARSER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (BVH_PARSER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (BVH_PARSER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/bvh-parser.cc)

add_library (bvhParser SHARED ${BVH_PARSER_SOURCES} ${BVH_PARSER_INCLUDE_DIR})

target_include_directories (bvhParser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)