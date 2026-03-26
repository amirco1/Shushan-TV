idf_component_register(SRCS "src/safety.c"
                      INCLUDE_DIRS "include"
                      REQUIRES motor_ctrl limit_inputs esp_common)
