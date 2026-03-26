/**
 * @file safety.h
 * @brief Safety supervisor — enforces limits, timeouts, and interlocks.
 *
 * Sits between the command bus and the motor controller.
 * Reads limit inputs each tick and forces motor stop when a limit
 * is reached or a fault condition is detected.
 */

#pragma once

#include "esp_err.h"
#include "motor_ctrl.h"
#include "limit_inputs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Safety supervisor context. */
typedef struct {
    motor_ctrl_ctx_t    *motor;  /**< Motor controller to supervise */
    limit_inputs_ctx_t  *limits; /**< Limit input readings */
} safety_ctx_t;

/**
 * @brief Initialise the safety supervisor.
 *
 * @param ctx    Pointer to caller-allocated safety context.
 * @param motor  Pointer to the motor controller context.
 * @param limits Pointer to the limit inputs context.
 * @return ESP_OK on success.
 */
esp_err_t safety_init(safety_ctx_t *ctx,
                      motor_ctrl_ctx_t *motor,
                      limit_inputs_ctx_t *limits);

/**
 * @brief Run one safety check cycle — call every control-loop tick.
 *
 * Checks:
 * - If extending and extend limit is active -> stop
 * - If retracting and retract limit is active -> stop
 * - Delegates timeout checking to motor_ctrl_update()
 *
 * @param ctx Pointer to safety context.
 * @return ESP_OK if no fault, ESP_FAIL if a fault was detected.
 */
esp_err_t safety_check(safety_ctx_t *ctx);

#ifdef __cplusplus
}
#endif
