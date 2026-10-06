cmake_minimum_required(VERSION 3.20)

project(Tank
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

add_executable(tank
    main.cpp
    ../src/hardware/ymotor.cpp
    ../src/drivers/motorDriver_25mm.cpp
)

target_include_directories(tank
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(tank
    PRIVATE
        PkgConfig::GPIOD
)

target_compile_options(tank
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
)