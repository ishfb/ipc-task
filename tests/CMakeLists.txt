cmake_minimum_required(VERSION 3.12)

# Locate GTest package
find_package(GTest REQUIRED)



# Add all .cpp files in the directory to the source files
file(GLOB_RECURSE SOURCES "*.cpp")

# Create the executable
add_executable(tests ${SOURCES})

# Link against GTest
target_link_libraries(tests GTest::GTest GTest::Main ipc-task-lib)