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
    source/2015/17/solution.cpp
    source/2015/18/solution.cpp
    source/2015/19/solution.cpp
    source/2015/20/solution.cpp
    source/2015/21/solution.cpp
    source/2015/22/solution.cpp
    source/2015/23/solution.cpp
    source/2015/24/solution.cpp
    source/2015/25/solution.cpp
    # 2016
    source/2016/01/solution.cpp
    source/2016/02/solution.cpp
    source/2016/03/solution.cpp
    source/2016/04/solution.cpp
    source/2016/05/solution.cpp
    source/2016/06/solution.cpp
    source/2016/07/solution.cpp
    source/2016/08/solution.cpp
    source/2016/09/solution.cpp
    source/2016/10/solution.cpp
    source/2016/11/solution.cpp
    source/2016/12/solution.cpp
    source/2016/13/solution.cpp
    source/2016/14/solution.cpp
    source/2016/15/solution.cpp
    source/2016/16/solution.cpp
    source/2016/17/solution.cpp
    source/2016/18/solution.cpp
    source/2016/19/solution.cpp
    source/2016/20/solution.cpp
    source/2016/21/solution.cpp
    source/2016/22/solution.cpp
    source/2016/23/solution.cpp
    source/2016/24/solution.cpp
    source/2016/25/solution.cpp
    # 2017
    source/2017/01/solution.cpp
    source/2017/02/solution.cpp
    source/2017/03/solution.cpp
    source/2017/04/solution.cpp
    source/2017/05/solution.cpp
    source/2017/06/solution.cpp
    source/2017/07/solution.cpp
    source/2017/08/solution.cpp
    source/2017/09/solution.cpp
    source/2017/10/solution.cpp
    source/2017/11/solution.cpp
    source/2017/12/solution.cpp
    source/2017/13/solution.cpp
    source/2017/14/solution.cpp
    source/2017/15/solution.cpp
    source/2017/16/solution.cpp
    source/2017/17/solution.cpp
    source/2017/18/solution.cpp
    source/2017/19/solution.cpp
    source/2017/20/solution.cpp
    source/2017/21/solution.cpp
    source/2017/22/solution.cpp
    source/2017/23/solution.cpp
    source/2017/24/solution.cpp
    source/2017/25/solution.cpp
    # 2020
    source/2020/01/solution.cpp
    source/2020/02/solution.cpp
    source/2020/03/solution.cpp
    source/2020/04/solution.cpp
    source/2020/05/solution.cpp
    source/2020/06/solution.cpp
    source/2020/07/solution.cpp
    source/2020/08/solution.cpp
    source/2020/09/solution.cpp
    source/2020/10/solution.cpp
    source/2020/11/solution.cpp
    source/2020/12/solution.cpp
    source/2020/13/solution.cpp
    source/2020/14/solution.cpp
    source/2020/15/solution.cpp
    source/2020/16/solution.cpp
    source/2020/17/solution.cpp
    source/2020/18/solution.cpp
    source/2020/19/solution.cpp
    source/2020/20/solution.cpp
    source/2020/21/solution.cpp
    source/2020/22/solution.cpp
    source/2020/23/solution.cpp
    source/2020/24/solution.cpp
    source/2020/25/solution.cpp
    # 2022
    source/2022/01/solution.cpp
    source/2022/02/solution.cpp
    source/2022/03/solution.cpp
    source/2022/04/solution.cpp
    source/2022/05/solution.cpp
    source/2022/06/solution.cpp
    source/2022/07/solution.cpp
    source/2022/08/solution.cpp
    source/2022/09/solution.cpp
    source/2022/10/solution.cpp
    source/2022/11/solution.cpp
    source/2022/12/solution.cpp
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
find_package(cpp-sort REQUIRED)
find_package(mdspan REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(unordered_dense REQUIRED)
find_package(pratt-parser REQUIRED)

target_link_libraries(aoc_lib PUBLIC
    Eigen3::Eigen
    cpp-lazy::cpp-lazy
    cpp-sort::cpp-sort
    fmt::fmt
    nlohmann_json::nlohmann_json
    pratt-parser::pratt-parser
    scn::scn
    unordered_dense::unordered_dense
)

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
