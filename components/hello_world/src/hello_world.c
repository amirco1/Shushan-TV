/**
 * @file hello_world.c
 * @brief Bring-up module: LED blink + button "Hello World!" print.
 */

#include "hello_world.h"
#include "board.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "hello";

/** @brief LED blink period in ms (250 ms = ~2 Hz toggle). */
#define LED_TOGGLE_PERIOD_MS    250U

/** @brief Stack size for the hello-world task in bytes. */
#define HELLO_TASK_STACK_SIZE   2048U

/** @brief Task priority (low, since this is just bring-up). */
#define HELLO_TASK_PRIORITY     2U

/**
 * @brief Main loop for the bring-up task.
 *
 * Toggles the LED each cycle and checks the BOOT button.
 * On a falling edge (button pressed), logs "Hello World!" to serial.
 *
 * @param arg Unused task parameter.
 */
static void hello_world_task(void *arg)
{
    (void)arg;

    uint32_t led_state      = 0U;
    uint32_t prev_btn_level = 1U; /* pull-up default: HIGH when released */

    ESP_LOGI(TAG, "Hello-world task started. Press BOOT button...");

    while (1) {
        /* Toggle LED */
        led_state ^= 1U;
        gpio_set_level(PIN_LED_BUILTIN, (int)led_state);

        /* Read BOOT button (active-low) */
        uint32_t btn_level = (uint32_t)gpio_get_level(PIN_BUTTON_BOOT);

        /* Detect falling edge: was HIGH, now LOW */
        if ((prev_btn_level == 1U) && (btn_level == 0U)) {
            ESP_LOGI(TAG, "Hello World!");
        }
        prev_btn_level = btn_level;

        vTaskDelay(pdMS_TO_TICKS(LED_TOGGLE_PERIOD_MS));
    }
}

esp_err_t hello_world_start(void)
{
    BaseType_t ret = xTaskCreate(
        hello_world_task,
        "hello_world",
        HELLO_TASK_STACK_SIZE,
        NULL,
        HELLO_TASK_PRIORITY,
        NULL
    );

    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create hello_world task");
        return ESP_FAIL;
    }

    return ESP_OK;
}
