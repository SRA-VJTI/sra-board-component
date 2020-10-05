set(srcs "accelerometer.c" "adc.c" "bar_graph.c" "buttons.c" "lsa.c" "motor_driver.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES )