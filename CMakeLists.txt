cmake_minimum_required(VERSION 3.20)
project(study_helper)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE src
        "src/*.cpp")

include_directories(include)

add_executable(study_helper ${src})
