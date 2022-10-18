# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(aocTests LANGUAGES CXX)

# ---- Tests ----

add_executable(aoc_test source/aoc_test.cpp)
target_link_libraries(aoc_test PRIVATE aoc_lib)
target_compile_features(aoc_test PRIVATE cxx_std_20)

add_test(NAME aoc_test COMMAND aoc_test)

# ---- End-of-file commands ----

add_folders(Test)
