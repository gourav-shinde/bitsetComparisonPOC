
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
add_custom_command(TARGET unifiedgtest
                    POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E echo "Running your_script.sh"
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
