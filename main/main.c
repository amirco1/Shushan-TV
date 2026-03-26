/**
 * @file main.c
 * @brief Application entry point for the Shushan TV recliner controller.
 *
 * Phase A (bring-up): runs hello_world only — blink LED, button test.
 * When you are ready for Phase B, uncomment the motor control section.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "board.h"
#include "hello_world.h"

/* Uncomment these when moving to Phase B (motor control) */
// #include "app_types.h"
// #include "motor_ctrl.h"
// #include "limit_inputs.h"
// #include "command_bus.h"
// #include "safety.h"

static const char *TAG = "app";

void app_main(void)
{
    ESP_LOGI(TAG, "=== Shushan TV Recliner Controller ===");
    ESP_LOGI(TAG, "Phase A: bring-up test");

    /* Initialise all board GPIO pins */
    ESP_ERROR_CHECK(board_init());

    /* Start hello-world task: blinks LED, prints on button press */
    ESP_ERROR_CHECK(hello_world_start());

    ESP_LOGI(TAG, "Hello-world running. Press BOOT button to test.");

    /*
     * ============================================================
     * Phase B: Motor control (uncomment when ready)
     * ============================================================
     *
     * motor_ctrl_ctx_t   motor;
     * limit_inputs_ctx_t limits;
     * safety_ctx_t       safety;
     *
     * ESP_ERROR_CHECK(command_bus_init());
     * ESP_ERROR_CHECK(motor_ctrl_init(&motor));
     * ESP_ERROR_CHECK(limit_inputs_init(&limits));
     * ESP_ERROR_CHECK(safety_init(&safety, &motor, &limits));
     *
     * while (1) {
     *     limit_inputs_update(&limits);
     *
     *     command_msg_t msg;
     *     if (command_bus_get_next(&msg)) {
     *         switch (msg.cmd) {
     *             case CMD_EXTEND:  motor_ctrl_extend(&motor);  break;
     *             case CMD_RETRACT: motor_ctrl_retract(&motor); break;
     *             case CMD_STOP:    motor_ctrl_stop(&motor);    break;
     *             default: break;
     *         }
     *     }
     *
     *     safety_check(&safety);
     *     vTaskDelay(pdMS_TO_TICKS(CONTROL_LOOP_PERIOD_MS));
     * }
     */
}
