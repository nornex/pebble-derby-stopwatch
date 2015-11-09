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
        src/pebble-cpp/pebble-sdk.hpp
    src/pebble-cpp/malloc.cpp
    src/pebble-cpp/pebble.cpp
    src/pebble-cpp/pebble.hpp src/pebble-cpp/window.hpp src/pebble-cpp/layer.hpp)

include_directories(
    /home/matt/code/pebble-basic/PebbleSDK-3.6/Pebble/chalk/include
    /home/matt/code/PebbleSDK-3.6/Pebble/chalk/include
    /home/matt/code/pebble-basic/build
    /home/matt/code/pebble-basic
    /home/matt/code/pebble-basic/build/src
    /home/matt/code/pebble-basic/src
    /home/matt/code/pebble-basic/build/chalk
    /home/matt/code/pebble-basic/chalk)

add_executable(pebble_cpp ${SOURCE_FILES})