/**
 * @file limit_inputs.c
 * @brief Hall-effect limit switch reading with counter-based debounce.
 *
 * The hall sensors are open-collector, active-low.  Internal pull-ups
 * are enabled in board_init().  A stable LOW reading for
 * DEBOUNCE_THRESHOLD consecutive calls means the limit is active.
 */

#include "limit_inputs.h"
#include "board.h"

#include "driver/gpio.h"

/**
 * @brief Number of consecutive matching reads required to change state.
 *
 * With CONTROL_LOOP_PERIOD_MS = 50 ms, 3 reads = 150 ms debounce.
 */
#define DEBOUNCE_THRESHOLD  3U

esp_err_t limit_inputs_init(limit_inputs_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    ctx->extend_active  = false;
    ctx->retract_active = false;
    ctx->extend_count   = 0U;
    ctx->retract_count  = 0U;

    return ESP_OK;
}

/**
 * @brief Generic debounce helper for one input.
 *
 * @param raw_active true if the raw GPIO indicates the limit is hit.
 * @param count      Pointer to the debounce counter for this input.
 * @param state      Pointer to the debounced boolean state.
 */
static void debounce_input(bool raw_active,
                           uint8_t *count,
                           bool *state)
{
    if (raw_active) {
        if (*count < DEBOUNCE_THRESHOLD) {
            (*count)++;
        }
        if (*count >= DEBOUNCE_THRESHOLD) {
            *state = true;
        }
    } else {
        if (*count > 0U) {
            (*count)--;
        }
        if (*count == 0U) {
            *state = false;
        }
    }
}

esp_err_t limit_inputs_update(limit_inputs_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    /* Active-low: GPIO reads 0 when magnet is present */
    bool ext_raw = (gpio_get_level(PIN_LIMIT_EXTEND)  == 0);
    bool ret_raw = (gpio_get_level(PIN_LIMIT_RETRACT) == 0);

    debounce_input(ext_raw, &ctx->extend_count,  &ctx->extend_active);
    debounce_input(ret_raw, &ctx->retract_count, &ctx->retract_active);

    return ESP_OK;
}

bool limit_extend_active(const limit_inputs_ctx_t *ctx)
{
    if (ctx == NULL) {
        return false;
    }
    return ctx->extend_active;
}

bool limit_retract_active(const limit_inputs_ctx_t *ctx)
{
    if (ctx == NULL) {
        return false;
    }
    return ctx->retract_active;
}
