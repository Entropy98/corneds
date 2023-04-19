cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)

project(corneds)

pico_sdk_init()

add_executable(corneds
  src/main.c
)

target_compile_options(corneds PUBLIC
  -g
)

target_sources(corneds PUBLIC
  src/timeslice.c
)

target_include_directories(corneds PUBLIC inc)

target_link_libraries(corneds pico_stdlib)
