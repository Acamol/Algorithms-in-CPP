# This file specifies how the project should be built, using CMake.
# If you are unfamiliar with CMake, don't worry about all the details.
# The sections you might want to edit are marked as such, and 
# the comments should hopefully make most of it clear.
#
# For many purposes, you may not need to change anything about this file.

cmake_minimum_required(VERSION 2.8)

# Set project name here.
project(ALGORITHMS_IN_CPP)
enable_language(CXX)

# Include stuff. No change needed.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)
include(Colors)


# --------------------------------------------------------------------------------
#                          Compile flags (change as needed).
# --------------------------------------------------------------------------------
# Set the C++ standard you wish to use (will apply to all files).
# If you do not use any features that limits the standard required,
# you could omit this line.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Things to always include as flags. Change as needed.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")  

# Build-type specific flags. Change as needed.
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

message(STATUS "Building with the following extra flags: ${CMAKE_CXX_FLAGS}")


# --------------------------------------------------------------------------------
#                         Locate files (no change needed).
# --------------------------------------------------------------------------------
# We make sure that CMake sees all the files.
include_directories(
    ${PROJECT_SOURCE_DIR}/Data\ Structures
    ${PROJECT_SOURCE_DIR}/Dynamic\ Programming
    ${PROJECT_SOURCE_DIR}/Graph
    ${PROJECT_SOURCE_DIR}/Greedy\ Algorithms
    ${PROJECT_SOURCE_DIR}/Sorting
    ${PROJECT_SOURCE_DIR}/Tests
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/include)

# Make variables referring to all the sources and test files.
set(HEADERS
        "Data\ Structures/DisjointSet.hpp"
        "Data\ Structures/Graph.hpp"
        "Data\ Structures/Heap.hpp"
        "Data\ Structures/WeightedGraph.hpp"
        "Dynamic\ Programming/KnapsackProblem.hpp"
        "Graph/_Utility.hpp"
        "Graph/BellmanFordGraph.hpp"
        "Graph/BFSGraph.hpp"
        "Graph/DFSGraph.hpp"
        "Graph/DijkstraGraph.hpp"
        "Graph/TopologicalSortGraph.hpp"
        "Greedy\ Algorithms/HuffmanCoding.hpp"
        "Greedy\ Algorithms/min_spanning_tree.hpp"
        "Sorting/heap_sort.hpp"
        "Sorting/insertion_sort.hpp"
        "Sorting/merge_sort.hpp"
        "Sorting/quick_sort.hpp"
        "Sorting/nth_element.hpp)
        
set(SOURCES
        "Data\ Structures/DisjointSet.cpp"
        "Dynamic\ Programming/KnapsackProblem.cpp"
        "Greedy\ Algorithms/HuffmanCoding.cpp")        
        
set(TESTFILES
        Tests/BellmanFordTest.cpp
        Tests/BFSTest.cpp
        Tests/DFSTest.cpp
        Tests/DijkstraTest.cpp
        Tests/GraphTest.cpp
        Tests/HeapsortTest.cpp
        Tests/HeapTest.cpp
        Tests/HuffmanCodeTest.cpp
        Tests/HuffmanCodeTest.cpp
        Tests/KnapsackTest.cpp
        Tests/MergesortTest.cpp
        Tests/MSTTest.cpp
        Tests/NthElementTest.cpp
        Tests/QuicksortTest.cpp
        Tests/TSortTest.cpp)
        
set(TEST_MAIN unit_tests.x)  # Default name for test executable (change if you wish).


# --------------------------------------------------------------------------------
#                            Build! (Change as needed)
# --------------------------------------------------------------------------------
# Compile all sources into a library. Called engine here (change if you wish).
add_library( engine "${SOURCES}" "${HEADERS}")


# --------------------------------------------------------------------------------
#                         Make Tests (no change needed).
# --------------------------------------------------------------------------------
# Add a make target 'gtest', that runs the tests (and builds all dependencies).
# The setup of Google Test is done at the very end of this file.
add_executable(${TEST_MAIN} ${TESTFILES})
add_dependencies(${TEST_MAIN} googletest engine)
target_link_libraries(${TEST_MAIN} googletest engine pthread)
add_custom_target(gtest 
    COMMAND "${PROJECT_BINARY_DIR}/${TEST_MAIN}"
    DEPENDS engine ${TEST_MAIN})

# Add a standard make target 'test' that runs the tests under CTest (only as an alt. to gtest).
include(CTest)
enable_testing()
add_test(unit_tests ${PROJECT_BINARY_DIR}/${TEST_MAIN})


# --------------------------------------------------------------------------------
#                         Code Coverage (no change needed).
# --------------------------------------------------------------------------------
# If CMake aborts due to missing dependencies for code coverage 
# (gcov, lcov, genhtml and supported compiler), comment this line.
include(CodeCoverage)


# --------------------------------------------------------------------------------
#                         Google Test (no change needed).
# --------------------------------------------------------------------------------
# The following makes sure that an up-to-date version of googletest is available,
# and built so that it may be used by your tests. 
add_custom_target( git_update
    COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
add_library( googletest
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/src/gtest_main.cc )
add_dependencies(googletest git_update)
set_source_files_properties(${PROJECT_SOURCE_DIR}/external/googletest/googletest/src/gtest-all.cc  PROPERTIES GENERATED 1)
set_source_files_properties(${PROJECT_SOURCE_DIR}/external/googletest/googletest/src/gtest_main.cc PROPERTIES GENERATED 1)
