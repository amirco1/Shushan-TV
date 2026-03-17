#pragma once

#include <stdint.h>

#include "esp_err.h"
#include "motor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    motor_direction_t direction;
    int speed_percent;
} control_command_t;

typedef struct {
    motor_t *motor;
    // Placeholder fields for future closed-loop control:
    // - target_rpm
    // - measured_rpm
    // - PI/PID integrator state
} control_ctx_t;

void control_init(control_ctx_t *ctx, motor_t *motor);
esp_err_t control_apply_open_loop(control_ctx_t *ctx, const control_command_t *cmd);

#ifdef __cplusplus
}
#endif
