cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    aoc
    VERSION 0.1.0
    DESCRIPTION "Advent of Code"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    aoc_lib OBJECT
    source/aoc.cpp
    source/util/md5.cpp
    # 2015
    source/2015/01/solution.cpp
    source/2015/02/solution.cpp
    source/2015/03/solution.cpp
    source/2015/04/solution.cpp
    source/2015/05/solution.cpp
    source/2015/06/solution.cpp
    source/2015/07/solution.cpp
    source/2015/08/solution.cpp
    source/2015/09/solution.cpp
    source/2015/10/solution.cpp
    source/2015/11/solution.cpp
    source/2015/12/solution.cpp
    source/2015/13/solution.cpp
    source/2015/14/solution.cpp
    source/2015/15/solution.cpp
    source/2015/16/solution.cpp
    # 2017
    source/2017/01/solution.cpp
    source/2017/02/solution.cpp
    source/2017/03/solution.cpp
    source/2017/04/solution.cpp
    source/2017/05/solution.cpp
    source/2017/06/solution.cpp
    source/2017/07/solution.cpp
)

target_include_directories(
    aoc_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(aoc_lib PUBLIC cxx_std_20)

# ---- Find deps ----
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(fmt REQUIRED)
find_package(robin_hood REQUIRED)
find_package(scn REQUIRED)
find_package(cpp-lazy REQUIRED)
find_package(xxHash REQUIRED)
find_package(mdspan REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(unordered_dense REQUIRED)

target_link_libraries(aoc_lib PUBLIC Eigen3::Eigen cpp-lazy::cpp-lazy fmt::fmt scn::scn xxHash::xxhash nlohmann_json::nlohmann_json unordered_dense::unordered_dense)

# ---- Declare executable ----

add_executable(aoc_exe source/main.cpp)
add_executable(aoc::exe ALIAS aoc_exe)

set_property(TARGET aoc_exe PROPERTY OUTPUT_NAME aoc)

target_compile_features(aoc_exe PRIVATE cxx_std_20)

target_link_libraries(aoc_exe PRIVATE aoc_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT aoc_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of aoc"
  )
else()
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
    if(CMAKE_EXPORT_COMPILE_COMMANDS)
        set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
    endif()
endif()

include(cmake/dev-mode.cmake)
