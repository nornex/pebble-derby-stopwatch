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
    src/pebble-cpp/util/malloc.cpp
    src/pebble-cpp/window.hpp
    src/pebble-cpp/layer.hpp
    src/pebble-cpp/rect.hpp
    src/pebble-cpp/util/optional.hpp
    src/pebble-cpp/tick_handler.hpp
    src/pebble-cpp/util/types.hpp
    src/pebble-cpp/util/bit_mask.hpp
    src/pebble-cpp/util/calendar_time.hpp
    src/pebble-cpp/util/fixed_string.hpp
    src/pebble-cpp/click_handler.hpp
    src/pebble-cpp/color.hpp
    src/pebble-cpp/font.hpp
    src/pebble-cpp/time_handler.hpp
    src/pebble-cpp/application.hpp
    src/countdown_timer.hpp
    src/pebble-cpp/util/duration.hpp
    src/pebble-cpp/vibration.hpp src/pebble-cpp/text_layer.hpp)

include_directories(
    /home/matt/code/PebbleSDK-3.6/Pebble/chalk/include
    /home/matt/code/pebble-basic/build
    /home/matt/code/pebble-basic
    /home/matt/code/pebble-basic/build/src
    /home/matt/code/pebble-basic/src
    /home/matt/code/pebble-basic/build/chalk
    /home/matt/code/pebble-basic/chalk)

add_executable(pebble_cpp ${SOURCE_FILES})