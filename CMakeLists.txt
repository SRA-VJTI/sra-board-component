set(srcs "src/accelerometer.c" "src/adc.c" "src/bar_graph.c" "src/switches.c" "src/lsa.c" "src/motor_driver.c" "src/servo.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES )