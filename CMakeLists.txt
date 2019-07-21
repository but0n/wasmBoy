cmake_minimum_required(VERSION 3.5)
project(WebAssemblyBoy)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s WASM=0 -s ENVIRONMENT=web -s MODULARIZE_INSTANCE=1 -s EXPORT_NAME='wasmBoy'")

file(GLOB_RECURSE src src/*.c)
# add_executable(wasmBoy ${src})
add_executable(wasmBoy-debug ${src})

set_target_properties(wasmBoy-debug PROPERTIES COMPILE_DEFINITIONS "DEBUG_LOG")