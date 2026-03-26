idf_component_register(SRCS "src/motor_ctrl.c"
                      INCLUDE_DIRS "include"
                      REQUIRES board driver esp_timer esp_common)