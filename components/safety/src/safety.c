/**
 * @file safety.c
 * @brief Safety supervisor implementation.
 */

#include "safety.h"

#include "esp_log.h"

static const char *TAG = "safety";

esp_err_t safety_init(safety_ctx_t *ctx,
                      motor_ctrl_ctx_t *motor,
                      limit_inputs_ctx_t *limits)
{
    if ((ctx == NULL) || (motor == NULL) || (limits == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    ctx->motor  = motor;
    ctx->limits = limits;

    ESP_LOGI(TAG, "Safety supervisor initialised");
    return ESP_OK;
}

esp_err_t safety_check(safety_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    motor_state_t state = motor_ctrl_get_state(ctx->motor);

    /* Stop if extending and extend limit hit */
    if ((state == MOTOR_STATE_EXTENDING) &&
        limit_extend_active(ctx->limits))
    {
        ESP_LOGW(TAG, "Extend limit reached — stopping motor");
        motor_ctrl_stop(ctx->motor);
    }

    /* Stop if retracting and retract limit hit */
    if ((state == MOTOR_STATE_RETRACTING) &&
        limit_retract_active(ctx->limits))
    {
        ESP_LOGW(TAG, "Retract limit reached — stopping motor");
        motor_ctrl_stop(ctx->motor);
    }

    /* Check motion timeout (handled inside motor_ctrl) */
    esp_err_t ret = motor_ctrl_update(ctx->motor);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Motor timeout fault detected");
        return ESP_FAIL;
    }

    return ESP_OK;
}
