#include "safety.h"

void safety_init(safety_ctx_t *ctx)
{
    if (ctx == NULL) {
        return;
    }
    ctx->fault_latched = false;
}

esp_err_t safety_check(safety_ctx_t *ctx)
{
    if (ctx == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Placeholder logic. In real firmware this function should:
    // 1) sample and filter current sensor,
    // 2) verify encoder heartbeat / speed plausibility,
    // 3) enforce undervoltage / thermal limits,
    // 4) latch + report faults.
    if (ctx->fault_latched) {
        return ESP_FAIL;
    }

    return ESP_OK;
}
