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
     ${BVH_PARSER_PATCH_VERSION}"
     )

# loaction of logger configuration file
set (EASYLOGGING_CONFIG_FILE_PATH
    "CMAKE_CURRENT_SOURCE_DIR/easylogging++.config"
    )

# location of additional cmake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )

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
    ${PROJECT_BINARY_DIR}/config.h
    )
#-------------------------------------------------------------------------------
# BOOST
#-------------------------------------------------------------------------------

find_package( Boost REQUIRED COMPONENTS filesystem )

include_directories(${Boost_INCLUDE_DIRS})

#-------------------------------------------------------------------------------
# LIBRARY SOURCES SETTING
#-------------------------------------------------------------------------------

set (BVH_PARSER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (BVH_PARSER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (BVH_PARSER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/bvh-parser.cc)

add_library (bvhParser SHARED ${BVH_PARSER_SOURCES} ${BVH_PARSER_INCLUDE_DIR})

target_include_directories (bvhParser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

# target to update git submodules
add_custom_target(
    update_submodules
    COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

#-------------------------------------------------------------------------------
# GOOGLETEST MODULE AND ADD TESTS
#-------------------------------------------------------------------------------

# set googletest module and add tests
set(GOOGLETEST_ROOT modules/googletest/googletest CACHE STRING "Google Test source root")

message(STATUS "GOOGLETEST_ROOT set to ${GOOGLETEST_ROOT}")

# google test includes
include_directories(
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
    )

# google test sources
set(GOOGLETEST_SOURCES
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
    )

# mark these files as generated (they may not be present at configure time)
foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletest ${GOOGLETEST_SOURCES})

# update git submodules before building google test
add_dependencies(googletest update_submodules)

# unit test executable
add_executable(
    unit_tests
    test/main.cc
    )

# unit test executable depends on google test
add_dependencies(unit_tests googletest)

# link unit test executable against google test
target_link_libraries(
    unit_tests
    googletest
    bvhParser
    pthread
    ${Boost_LIBRARIES}
    )

# enable tests
enable_testing()

# include Dart that generate default DartConfiguration.tcl
include(Dart)

# add tests
add_test(bvhParserTests unit_tests)