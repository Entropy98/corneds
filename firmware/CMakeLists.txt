cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)

project(corneds)

pico_sdk_init()

add_executable(${PROJECT_NAME}_left
  src/main.c
)
add_executable(${PROJECT_NAME}_right
  src/main.c
)

target_compile_options(${PROJECT_NAME}_right PUBLIC
  -g
  -O0
  -DKBDSIDE_RIGHT
)
target_compile_options(${PROJECT_NAME}_left PUBLIC
  -g
  -O0
)

target_sources(${PROJECT_NAME}_right PUBLIC
  src/hid_task.c
  src/keymap.c
  src/timeslice.c
  src/usb_descriptors.c
)
target_sources(${PROJECT_NAME}_left PUBLIC
  src/hid_task.c
  src/keymap.c
  src/timeslice.c
  src/usb_descriptors.c
)

target_include_directories(${PROJECT_NAME}_right PUBLIC inc)
target_include_directories(${PROJECT_NAME}_left PUBLIC inc)

target_link_libraries(${PROJECT_NAME}_right pico_stdlib tinyusb_device tinyusb_board)
target_link_libraries(${PROJECT_NAME}_left pico_stdlib tinyusb_device tinyusb_board)
