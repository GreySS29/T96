cmake_minimum_required(VERSION 3.16)
project(camera_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(camera_test test_camera.cpp)

target_include_directories(camera_test PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(camera_test PRIVATE ${OpenCV_LIBS})
