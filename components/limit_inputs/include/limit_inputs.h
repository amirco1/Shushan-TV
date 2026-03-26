/**
 * @file limit_inputs.h
 * @brief Hall-effect limit switch input reading with software debounce.
 *
 * Wraps the two hall-sensor limit inputs so the rest of the firmware
 * never calls gpio_get_level() directly for these signals.
 * Active-low logic: sensor output goes LOW when a magnet is detected.
 */

#pragma once

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Debounced limit input state. */
typedef struct {
    bool extend_active;     /**< true when extend limit is triggered */
    bool retract_active;    /**< true when retract limit is triggered */
    /* Internal debounce counters */
    uint8_t extend_count;
    uint8_t retract_count;
} limit_inputs_ctx_t;

/**
 * @brief Initialise the limit input context.
 *
 * GPIO pins are already configured by board_init(); this just zeros
 * the debounce state.
 *
 * @param ctx Pointer to caller-allocated context.
 * @return ESP_OK on success.
 */
esp_err_t limit_inputs_init(limit_inputs_ctx_t *ctx);

/**
 * @brief Update debounced limit states — call every control-loop tick.
 *
 * Reads raw GPIO levels and applies a simple counter-based debounce.
 *
 * @param ctx Pointer to limit input context.
 * @return ESP_OK on success.
 */
esp_err_t limit_inputs_update(limit_inputs_ctx_t *ctx);

/**
 * @brief Check if the extend limit is active (magnet detected).
 *
 * @param ctx Pointer to limit input context.
 * @return true if the actuator has reached full extension.
 */
bool limit_extend_active(const limit_inputs_ctx_t *ctx);

/**
 * @brief Check if the retract limit is active (magnet detected).
 *
 * @param ctx Pointer to limit input context.
 * @return true if the actuator has reached full retraction.
 */
bool limit_retract_active(const limit_inputs_ctx_t *ctx);

#ifdef __cplusplus
}
#endif
