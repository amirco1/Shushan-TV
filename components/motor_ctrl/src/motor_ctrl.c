/**
 * @file motor_ctrl.c
 * @brief Relay-based actuator controller implementation.
 */

#include "motor_ctrl.h"
#include "board.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "motor_ctrl";

/** @brief Convert ms to microseconds for esp_timer comparisons. */
#define MS_TO_US(ms)  ((int64_t)(ms) * 1000)

/* ------------------------------------------------------------------ */
/*  Internal helpers                                                   */
/* ------------------------------------------------------------------ */

/** @brief De-energise both relay coils. */
static void relays_off(void)
{
    gpio_set_level(PIN_RELAY_EXTEND,  0);
    gpio_set_level(PIN_RELAY_RETRACT, 0);
}

/** @brief Check if enough dead-time has passed since last relay-off. */
static bool dead_time_elapsed(const motor_ctrl_ctx_t *ctx)
{
    int64_t now = esp_timer_get_time();
    return (now - ctx->stop_time_us) >= MS_TO_US(RELAY_DEAD_TIME_MS);
}

/** @brief Check if the motion timeout has been exceeded. */
static bool motion_timed_out(const motor_ctrl_ctx_t *ctx)
{
    int64_t now = esp_timer_get_time();
    return (now - ctx->motion_start_us) >= MS_TO_US(MOTOR_TIMEOUT_MS);
}

/* ------------------------------------------------------------------ */
/*  Public API                                                         */
/* ------------------------------------------------------------------ */

esp_err_t motor_ctrl_init(motor_ctrl_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    relays_off();

    ctx->state           = MOTOR_STATE_IDLE;
    ctx->motion_start_us = 0;
    ctx->stop_time_us    = esp_timer_get_time();

    ESP_LOGI(TAG, "Motor controller initialised — relays OFF");
    return ESP_OK;
}

esp_err_t motor_ctrl_extend(motor_ctrl_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (ctx->state == MOTOR_STATE_FAULT) {
        ESP_LOGW(TAG, "Cannot extend: fault latched");
        return ESP_ERR_INVALID_STATE;
    }

    /* Already extending — nothing to do */
    if (ctx->state == MOTOR_STATE_EXTENDING) {
        return ESP_OK;
    }

    /* If currently retracting, stop first and enforce dead-time */
    if (ctx->state == MOTOR_STATE_RETRACTING) {
        relays_off();
        ctx->stop_time_us = esp_timer_get_time();
        ctx->state        = MOTOR_STATE_IDLE;
    }

    if (!dead_time_elapsed(ctx)) {
        ESP_LOGW(TAG, "Dead-time not elapsed, try again");
        return ESP_ERR_INVALID_STATE;
    }

    /* Energise extend relay only */
    relays_off(); /* safety: ensure retract is off first */
    gpio_set_level(PIN_RELAY_EXTEND, 1);

    ctx->state           = MOTOR_STATE_EXTENDING;
    ctx->motion_start_us = esp_timer_get_time();

    ESP_LOGI(TAG, "Extending");
    return ESP_OK;
}

esp_err_t motor_ctrl_retract(motor_ctrl_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (ctx->state == MOTOR_STATE_FAULT) {
        ESP_LOGW(TAG, "Cannot retract: fault latched");
        return ESP_ERR_INVALID_STATE;
    }

    if (ctx->state == MOTOR_STATE_RETRACTING) {
        return ESP_OK;
    }

    if (ctx->state == MOTOR_STATE_EXTENDING) {
        relays_off();
        ctx->stop_time_us = esp_timer_get_time();
        ctx->state        = MOTOR_STATE_IDLE;
    }

    if (!dead_time_elapsed(ctx)) {
        ESP_LOGW(TAG, "Dead-time not elapsed, try again");
        return ESP_ERR_INVALID_STATE;
    }

    relays_off();
    gpio_set_level(PIN_RELAY_RETRACT, 1);

    ctx->state           = MOTOR_STATE_RETRACTING;
    ctx->motion_start_us = esp_timer_get_time();

    ESP_LOGI(TAG, "Retracting");
    return ESP_OK;
}

esp_err_t motor_ctrl_stop(motor_ctrl_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    relays_off();
    ctx->stop_time_us = esp_timer_get_time();

    if (ctx->state != MOTOR_STATE_FAULT) {
        ctx->state = MOTOR_STATE_IDLE;
    }

    ESP_LOGI(TAG, "Stopped");
    return ESP_OK;
}

motor_state_t motor_ctrl_get_state(const motor_ctrl_ctx_t *ctx)
{
    if (ctx == NULL) {
        return MOTOR_STATE_FAULT;
    }
    return ctx->state;
}

esp_err_t motor_ctrl_update(motor_ctrl_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    /* Only check timeout while actively moving */
    bool is_moving = (ctx->state == MOTOR_STATE_EXTENDING) ||
                     (ctx->state == MOTOR_STATE_RETRACTING);

    if (is_moving && motion_timed_out(ctx)) {
        relays_off();
        ctx->stop_time_us = esp_timer_get_time();
        ctx->state        = MOTOR_STATE_FAULT;
        ESP_LOGE(TAG, "Motion timeout — FAULT latched");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t motor_ctrl_clear_fault(motor_ctrl_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (ctx->state != MOTOR_STATE_FAULT) {
        return ESP_ERR_INVALID_STATE;
    }

    relays_off();
    ctx->state = MOTOR_STATE_IDLE;
    ESP_LOGI(TAG, "Fault cleared");
    return ESP_OK;
}
