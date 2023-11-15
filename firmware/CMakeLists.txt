cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)

project(corneds)

pico_sdk_init()

add_executable(${PROJECT_NAME}
  src/main.c
)

# Tests
add_executable(${PROJECT_NAME}_test_print
  test/test_print.c
)
add_executable(${PROJECT_NAME}_test_debug_uart
  test/test_debug_uart.c
)
add_executable(${PROJECT_NAME}_test_keypress
  test/test_keypress.c
)


target_compile_options(${PROJECT_NAME} PUBLIC
  -g
  -O0
)

# Tests
target_compile_options(${PROJECT_NAME}_test_print PUBLIC
  -g
  -O0
)
target_compile_options(${PROJECT_NAME}_test_debug_uart PUBLIC
  -g
  -O0
)
target_compile_options(${PROJECT_NAME}_test_keypress PUBLIC
  -g
  -O0
)

target_sources(${PROJECT_NAME} PUBLIC
  src/debug_uart.c
  src/hid_task.c
  src/keymap.c
  src/led_utils.c
  src/timeslice.c
  src/usb_descriptors.c
  src/usb_detect.c
  src/xboard_comms.c
)

# Tests
target_sources(${PROJECT_NAME}_test_print PUBLIC
  src/hid_task.c
  src/keymap.c
  src/led_utils.c
  src/timeslice.c
  src/usb_descriptors.c
  src/usb_detect.c
  src/xboard_comms.c
)
target_sources(${PROJECT_NAME}_test_debug_uart PUBLIC
  src/debug_uart.c
  src/hid_task.c
  src/led_utils.c
  src/timeslice.c
  src/usb_descriptors.c
  src/usb_detect.c
)
target_sources(${PROJECT_NAME}_test_keypress PUBLIC
  src/hid_task.c
  src/keymap.c
  src/led_utils.c
  src/timeslice.c
  src/usb_descriptors.c
  src/usb_detect.c
)


target_include_directories(${PROJECT_NAME} PUBLIC inc)

# Tests
target_include_directories(${PROJECT_NAME}_test_print PUBLIC inc)
target_include_directories(${PROJECT_NAME}_test_debug_uart PUBLIC inc)
target_include_directories(${PROJECT_NAME}_test_keypress PUBLIC inc)

target_link_libraries(${PROJECT_NAME} pico_stdlib tinyusb_device tinyusb_board hardware_regs_headers hardware_structs_headers)

# Tests
target_link_libraries(${PROJECT_NAME}_test_print pico_stdlib tinyusb_device tinyusb_board hardware_regs_headers hardware_structs_headers)
target_link_libraries(${PROJECT_NAME}_test_debug_uart tinyusb_device tinyusb_board pico_stdlib hardware_regs_headers hardware_structs_headers)
target_link_libraries(${PROJECT_NAME}_test_keypress tinyusb_device tinyusb_board pico_stdlib hardware_regs_headers hardware_structs_headers)
