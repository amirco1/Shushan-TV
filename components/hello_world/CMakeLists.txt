idf_component_register(SRCS "src/hello_world.c"
                      INCLUDE_DIRS "include"
                      REQUIRES board driver freertos esp_common)
