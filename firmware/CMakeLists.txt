cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)

project(corneds)

pico_sdk_init()

if(NOT DEFINED KBD_LAYOUT)
  add_compile_definitions(KBD_LAYOUT_QWERTY=1)
  add_compile_definitions(KBD_LAYOUT_COLEMAK=0)
elseif(KBD_LAYOUT STREQUAL "QWERTY")
  add_compile_definitions(KBD_LAYOUT_QWERTY=1)
  add_compile_definitions(KBD_LAYOUT_COLEMAK=0)
elseif(KBD_LAYOUT STREQUAL "COLEMAK")
  add_compile_definitions(KBD_LAYOUT_QWERTY=0)
  add_compile_definitions(KBD_LAYOUT_COLEMAK=1)
endif()

add_executable(${PROJECT_NAME}
  src/main.c
)

# Tests
add_executable(debug_uart
  test/debug_uart.c
)
add_executable(keypress
  test/keypress.c
)
add_executable(led_blinky
  test/led_blinky.c
)
add_executable(print
  test/print.c
)
add_executable(xboard_comms
  test/xboard_comms.c
)

target_compile_options(${PROJECT_NAME} PUBLIC
  -g
  -O0
)

# Tests
target_compile_options(debug_uart PUBLIC
  -g
  -O0
)
target_compile_options(keypress PUBLIC
  -g
  -O0
)
target_compile_options(led_blinky PUBLIC
  -g
  -O0
)
target_compile_options(print PUBLIC
  -g
  -O0
)
target_compile_options(xboard_comms PUBLIC
  -g
  -O0
)

target_sources(${PROJECT_NAME} PUBLIC
  src/alarm_utils.c
  src/debug_uart.c
  src/hid_task.c
  src/keymap.c
  src/led_sm.c
  src/led_utils.c
  src/timing_arch.c
  src/usb_descriptors.c
  src/usb_detect.c
  src/watchdog.c
  src/xboard_comms.c
)

# Tests
target_sources(debug_uart PUBLIC
  src/alarm_utils.c
  src/debug_uart.c
  src/hid_task.c
  src/led_utils.c
  src/timing_arch.c
  src/usb_descriptors.c
  src/usb_detect.c
)
target_sources(keypress PUBLIC
  src/alarm_utils.c
  src/hid_task.c
  src/keymap.c
  src/led_utils.c
  src/timing_arch.c
  src/usb_descriptors.c
  src/usb_detect.c
)
target_sources(led_blinky PUBLIC
  src/alarm_utils.c
  src/led_utils.c
  src/timing_arch.c
)
target_sources(print PUBLIC
  src/alarm_utils.c
  src/hid_task.c
  src/keymap.c
  src/led_utils.c
  src/timing_arch.c
  src/usb_descriptors.c
  src/usb_detect.c
  src/xboard_comms.c
)
target_sources(xboard_comms PUBLIC
  src/alarm_utils.c
  src/hid_task.c
  src/keymap.c
  src/led_utils.c
  src/timing_arch.c
  src/usb_descriptors.c
  src/usb_detect.c
  src/xboard_comms.c
)

target_include_directories(${PROJECT_NAME} PUBLIC inc)

# Tests
target_include_directories(debug_uart PUBLIC inc)
target_include_directories(keypress PUBLIC inc)
target_include_directories(led_blinky PUBLIC inc)
target_include_directories(print PUBLIC inc)
target_include_directories(xboard_comms PUBLIC inc)

target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  tinyusb_device
  tinyusb_board
  hardware_regs_headers
  hardware_structs_headers
  hardware_timer
  hardware_irq
  hardware_watchdog
)

# Tests
target_link_libraries(debug_uart
  tinyusb_device
  tinyusb_board
  pico_stdlib
  hardware_regs_headers
  hardware_structs_headers
  hardware_timer
  hardware_irq
)
target_link_libraries(keypress
  tinyusb_device
  tinyusb_board
  pico_stdlib
  hardware_regs_headers
  hardware_structs_headers
  hardware_timer
  hardware_irq
)
target_link_libraries(led_blinky
  tinyusb_device
  tinyusb_board
  pico_stdlib
  hardware_regs_headers
  hardware_structs_headers
  hardware_timer
  hardware_irq
)
target_link_libraries(print
  pico_stdlib
  tinyusb_device
  tinyusb_board
  hardware_regs_headers
  hardware_structs_headers
  hardware_timer
  hardware_irq
)
target_link_libraries(xboard_comms
  tinyusb_device
  tinyusb_board
  pico_stdlib
  hardware_regs_headers
  hardware_structs_headers
  hardware_timer
  hardware_irq
)
