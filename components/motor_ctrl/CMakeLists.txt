idf_component_register(SRCS "src/motor_ctrl.c"
                      INCLUDE_DIRS "include"
                      REQUIRES app_types board driver esp_timer esp_common)