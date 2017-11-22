cmake_minimum_required(VERSION 3.8)
project(ass-spl-1)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

set(SOURCE_FILES
        src/Main.cpp
        src/Commands.cpp
        src/Environment.cpp
        src/Files.cpp
        src/FileSystem.cpp)

add_executable(ass-spl-1 ${SOURCE_FILES})