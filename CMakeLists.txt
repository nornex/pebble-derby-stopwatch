cmake_minimum_required(VERSION 3.3)
project(pebble_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    build/aplite/src/resource_ids.auto.h
    build/aplite/appinfo.auto.c
    build/basalt/src/resource_ids.auto.h
    build/basalt/appinfo.auto.c
    build/chalk/src/resource_ids.auto.h
    build/chalk/appinfo.auto.c
    src/basics.cpp
        src/pebble-cpp/pebble-api.hpp
        src/pebble-cpp/malloc.cpp
        src/pebble-cpp/pebble.cpp
        src/pebble-cpp/pebble.hpp)

add_executable(pebble_cpp ${SOURCE_FILES})