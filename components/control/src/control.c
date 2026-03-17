#include "control.h"

#include "esp_check.h"

void control_init(control_ctx_t *ctx, motor_t *motor)
{
    if (ctx == NULL) {
        return;
    }
    ctx->motor = motor;
}

esp_err_t control_apply_open_loop(control_ctx_t *ctx, const control_command_t *cmd)
{
    ESP_RETURN_ON_FALSE(ctx != NULL, ESP_ERR_INVALID_ARG, "control", "ctx is NULL");
    ESP_RETURN_ON_FALSE(ctx->motor != NULL, ESP_ERR_INVALID_STATE, "control", "motor is NULL");
    ESP_RETURN_ON_FALSE(cmd != NULL, ESP_ERR_INVALID_ARG, "control", "cmd is NULL");

    // Keep API robust against invalid caller input.
    int speed = cmd->speed_percent;
    if (speed < 0) {
        speed = 0;
    }
    if (speed > 100) {
        speed = 100;
    }

    return motor_set_open_loop(ctx->motor, cmd->direction, (uint8_t)speed);
}
