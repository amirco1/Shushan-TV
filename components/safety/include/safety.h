#pragma once

#include <stdbool.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool fault_latched;
    // Placeholders for safety telemetry:
    // - motor_current_amps
    // - board_voltage_v
    // - encoder_alive
} safety_ctx_t;

void safety_init(safety_ctx_t *ctx);
esp_err_t safety_check(safety_ctx_t *ctx);

#ifdef __cplusplus
}
#endif
