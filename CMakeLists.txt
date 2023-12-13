
# write cmakefile to build the project



# create a new project
project (UNIFIED_QUEUE)

# add a library
# add_library(unifiedqueue UnifiedQueue.hpp)

# add an executable
# add_executable(testingUnified testingunifiedqueue.cpp)

# add test
add_executable(unifiedgtest test_queue.cpp)
target_link_libraries(unifiedgtest gtest)
