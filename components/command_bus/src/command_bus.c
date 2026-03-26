/**
 * @file command_bus.c
 * @brief FreeRTOS queue-based command bus implementation.
 */

#include "command_bus.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"

static const char *TAG = "cmd_bus";

/** @brief The single command queue instance. */
static QueueHandle_t s_cmd_queue = NULL;

esp_err_t command_bus_init(void)
{
    s_cmd_queue = xQueueCreate(COMMAND_QUEUE_LENGTH,
                               sizeof(command_msg_t));

    if (s_cmd_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create command queue");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Command bus initialised (depth=%u)",
             (unsigned)COMMAND_QUEUE_LENGTH);
    return ESP_OK;
}

esp_err_t command_bus_submit(command_t cmd, command_source_t source)
{
    if (s_cmd_queue == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    command_msg_t msg = {
        .cmd    = cmd,
        .source = source,
    };

    if (xQueueSend(s_cmd_queue, &msg, 0) != pdTRUE) {
        ESP_LOGW(TAG, "Command queue full, dropping cmd=%d", (int)cmd);
        return ESP_FAIL;
    }

    return ESP_OK;
}

bool command_bus_get_next(command_msg_t *msg)
{
    if ((s_cmd_queue == NULL) || (msg == NULL)) {
        return false;
    }

    return (xQueueReceive(s_cmd_queue, msg, 0) == pdTRUE);
}
