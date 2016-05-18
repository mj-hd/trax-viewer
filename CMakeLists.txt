cmake_minimum_required(VERSION 2.4)

project(trax-viewer CXX)

set(BINARY_NAME trax-viewer)

set(OUT_DIR out)
set(SRC_DIR src)

set(CMAKE_CXX_FLAGS_RELEASE "-std=c++14 -Wall -O2")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++14 -g3")
set(CMAKE_BUILD_TYPE Release)

set(PROJECT_BINARY_DIR ${OUT_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

###########################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost COMPONENTS thread system REQUIRED)
find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

foreach(path IN LISTS SOURCES)
    message("source file found: ${path}")
endforeach()

add_executable(${BINARY_NAME} ${SOURCES})

target_link_libraries(${BINARY_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
