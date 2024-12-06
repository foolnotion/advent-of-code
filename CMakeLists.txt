cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    aoc
    VERSION 0.1.0
    DESCRIPTION "Advent of Code"
    HOMEPAGE_URL "https://adventofcode.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

include(CheckIPOSupported)

check_ipo_supported(RESULT ipo_supported OUTPUT error)

# ---- Declare library ----
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS source/solution.cpp source/aoc.cpp source/util/md5.cpp)
add_library(
    aoc_lib OBJECT
    ${SOURCE_FILES}
)

target_include_directories(
    aoc_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(aoc_lib PUBLIC cxx_std_20)

# ---- Find deps ----
find_package(Boost CONFIG REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(date REQUIRED)
#find_package(gtl REQUIRED)
find_package(fmt REQUIRED)
find_package(scn REQUIRED)
find_package(sul-dynamic_bitset REQUIRED)
find_package(cpp-lazy REQUIRED)
find_package(cpp-sort REQUIRED)
find_package(libassert REQUIRED)
find_package(mdspan REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(unordered_dense REQUIRED)
find_package(pratt-parser REQUIRED)
find_package(xxhash_cpp REQUIRED)

target_link_libraries(aoc_lib PUBLIC
    Boost::boost
    Eigen3::Eigen
    cpp-lazy::cpp-lazy
    cpp-sort::cpp-sort
    date::date
    fmt::fmt
    #gtl
    libassert::assert
    nlohmann_json::nlohmann_json
    pratt-parser::pratt-parser
    scn::scn
    sul::dynamic_bitset
    unordered_dense::unordered_dense
    z3
    xxhash_cpp::xxhash_cpp
)

# ---- Declare executable ----

add_executable(aoc_exe source/main.cpp)
add_executable(aoc::exe ALIAS aoc_exe)

set_property(TARGET aoc_exe PROPERTY OUTPUT_NAME aoc)

target_compile_features(aoc_exe PRIVATE cxx_std_20)

target_link_libraries(aoc_exe PRIVATE aoc_lib)

if(ipo_supported)
    set_property(TARGET aoc_lib PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET aoc_exe PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

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
