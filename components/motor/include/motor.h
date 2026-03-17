#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MOTOR_DIRECTION_FORWARD = 0,
    MOTOR_DIRECTION_REVERSE = 1,
} motor_direction_t;

typedef struct {
    gpio_num_t pwm_gpio;
    gpio_num_t dir_gpio;
    ledc_timer_t ledc_timer;
    ledc_channel_t ledc_channel;
    uint32_t pwm_freq_hz;
    ledc_timer_bit_t pwm_resolution;
} motor_init_config_t;

typedef struct {
    motor_init_config_t cfg;
    bool initialized;
} motor_t;

esp_err_t motor_init(motor_t *motor, const motor_init_config_t *cfg);
esp_err_t motor_set_open_loop(motor_t *motor, motor_direction_t direction, uint8_t duty_percent);
esp_err_t motor_stop(motor_t *motor);

#ifdef __cplusplus
}
#endif
