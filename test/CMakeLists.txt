# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(aocTests LANGUAGES CXX)

find_package(doctest REQUIRED)

# ---- Tests ----

add_executable(aoc_test source/aoc_test.cpp)
target_compile_features(aoc_test PRIVATE cxx_std_20)
target_link_libraries(aoc_test PRIVATE aoc_lib doctest::doctest)

include(doctest)
doctest_discover_tests(aoc_test COMMAND aoc_test WORKING_DIRECTORY ${aoc_SOURCE_DIR} ADD_LABELS 1)

# ---- End-of-file commands ----
# doctest_discover_tests(aoc_test)
# add_folders(test)
