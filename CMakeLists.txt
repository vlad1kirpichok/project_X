cmake_minimum_required(VERSION 3.15)

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
        src/inject/inject.cpp
        src/inject/windows/inject_w.cpp
        src/isa/utils.cpp
    )
endif()

if(APPLE)
    add_executable(
        project_X
        src/main.cpp
        src/inject/inject.cpp
        src/inject/macos/inject_m.cpp
        src/isa/utils.cpp
    )
endif()

if(WIN32)
    add_executable(
        project_X
        src/main.cpp
        src/inject/inject.cpp
        src/inject/windows/inject_w.cpp
        src/isa/utils.cpp
    )
endif()
