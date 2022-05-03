cmake_minimum_required(VERSION 3.21)

project(project_X)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/inject)
include_directories(src/isa)
include_directories(src/isa/aarch64)
include_directories(src/isa/riscv64)
include_directories(src/isa/x64)

if(UNIX AND NOT APPLE)
    add_executable(
        project_X
        src/main.cpp
        src/inject/windows/inject_w.cpp
    )
endif()

if(APPLE)
    add_executable(
        project_X
        src/main.cpp
        src/inject/macos/inject_m.cpp
    )
endif()

if(WIN32)
    add_executable(
        project_X
        src/main.cpp
        src/inject/windows/inject_w.cpp
    )
endif()
