cmake_minimum_required(VERSION 3.22)

set(srcs "src/i2cdev.c" "src/adc.c" "src/bar_graph.c" "src/switches.c" "src/lsa.c" "src/mcpwm_interface.c" "src/mpu6050.c" "src/servo.c" "src/utils.c" "src/motor_driver.c")
set(include_dirs include)

if(CONFIG_ENABLE_OLED)
    list(APPEND include_dirs lvgl)
    list(APPEND srcs "src/oled.c" "src/lvgl_helpers.c")
    file(GLOB_RECURSE LVGL_DRIVER_FILES lvgl/src/*.c)
    list(APPEND srcs ${LVGL_DRIVER_FILES})
endif()

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES driver efuse esp_adc esp_common esp_event 
                                esp_rom esp_timer freertos heap log soc spiffs)