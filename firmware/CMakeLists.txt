cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)

project(corneds)

pico_sdk_init()

set(EXEC_NAME ${PROJECT_NAME}_${KBDSIDE})

add_executable(${EXEC_NAME}
  src/main.c
)

target_compile_options(${EXEC_NAME} PUBLIC
  -g
  -O0
)

target_sources(${EXEC_NAME} PUBLIC
  src/hid_task.c
  src/keymap.c
  src/timeslice.c
  src/usb_descriptors.c
)

target_include_directories(${EXEC_NAME} PUBLIC inc)

target_link_libraries(${EXEC_NAME} pico_stdlib tinyusb_device tinyusb_board)
