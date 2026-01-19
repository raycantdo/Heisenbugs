cmake_minimum_required(VERSION 3.10)
project(StudyChron)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure this path is correct relative to this file
include_directories(include)

# Main Application
add_executable(StudyChronApp src/main.cpp)

# Link the tests folder
add_subdirectory(tests)