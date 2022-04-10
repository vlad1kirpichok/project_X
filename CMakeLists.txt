cmake_minimum_required(VERSION 3.21)

project(project_X)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/inject)
include_directories(src/isa)
include_directories(src/isa/aarch64)
include_directories(src/isa/riscv64)
include_directories(src/isa/x64)

add_executable(
        project_X
        src/isa/aarch64/AArch64.h
        src/isa/riscv64/RiscV64.h
        src/isa/x64/X64.h
        src/inject/Inject.h
        src/Main.cpp
)
