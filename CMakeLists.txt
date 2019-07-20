cmake_minimum_required(VERSION 3.5)
project(WebAssemblyBoy)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s WASM=0 -s ENVIRONMENT=web -s MODULARIZE_INSTANCE=1 -s EXPORT_NAME='cmodule'")


# include_directories(src/)
add_executable(build
    src/cpu.c
    src/mmu.c
    src/ppu.c
    src/board.c
)