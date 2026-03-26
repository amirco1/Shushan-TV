/**
 * @file motor_ctrl.h
 * @brief Relay-based bidirectional actuator controller with state machine.
 *
 * Controls a linear actuator through an SPDT relay pair.
 * Two GPIO outputs drive the relay coils; only one may be active at a time.
 * A dead-time gap is enforced before direction reversal.
 *
 * @note This module does NOT read limit switches directly.  The safety
 *       module calls motor_ctrl_stop() when a limit is reached.
 */

#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Opaque context for one motor channel. */
typedef struct {
    motor_state_t state;            /**< Current state machine state */
    int64_t       motion_start_us;  /**< Timestamp when motion began (us) */
    int64_t       stop_time_us;     /**< Timestamp when relays were turned off */
} motor_ctrl_ctx_t;

/**
 * @brief Initialise the motor controller.
 *
 * Sets the relay outputs to OFF and the state to IDLE.
 * board_init() must be called first.
 *
 * @param ctx Pointer to motor context (caller-allocated).
 * @return ESP_OK on success.
 */
esp_err_t motor_ctrl_init(motor_ctrl_ctx_t *ctx);

/**
 * @brief Command the actuator to extend (lower TV).
 *
 * Energises the extend relay after checking dead-time and current state.
 *
 * @param ctx Pointer to motor context.
 * @return ESP_OK, ESP_ERR_INVALID_STATE if in FAULT, or dead-time not elapsed.
 */
esp_err_t motor_ctrl_extend(motor_ctrl_ctx_t *ctx);

/**
 * @brief Command the actuator to retract (raise TV).
 *
 * Energises the retract relay after checking dead-time and current state.
 *
 * @param ctx Pointer to motor context.
 * @return ESP_OK, ESP_ERR_INVALID_STATE if in FAULT, or dead-time not elapsed.
 */
esp_err_t motor_ctrl_retract(motor_ctrl_ctx_t *ctx);

/**
 * @brief Stop the actuator immediately.
 *
 * De-energises both relays and transitions to IDLE (or STOPPED_LIMIT
 * if called by the safety layer after a limit trigger).
 *
 * @param ctx Pointer to motor context.
 * @return ESP_OK on success.
 */
esp_err_t motor_ctrl_stop(motor_ctrl_ctx_t *ctx);

/**
 * @brief Get the current motor state.
 *
 * @param ctx Pointer to motor context.
 * @return Current motor_state_t value.
 */
motor_state_t motor_ctrl_get_state(const motor_ctrl_ctx_t *ctx);

/**
 * @brief Periodic update — call from the control loop.
 *
 * Checks the motion timeout and forces a stop + FAULT if exceeded.
 *
 * @param ctx Pointer to motor context.
 * @return ESP_OK normally, ESP_FAIL if a timeout fault was latched.
 */
esp_err_t motor_ctrl_update(motor_ctrl_ctx_t *ctx);

/**
 * @brief Clear a latched fault and return to IDLE.
 *
 * @param ctx Pointer to motor context.
 * @return ESP_OK on success, ESP_ERR_INVALID_STATE if not in FAULT.
 */
esp_err_t motor_ctrl_clear_fault(motor_ctrl_ctx_t *ctx);

#ifdef __cplusplus
}
#endif
