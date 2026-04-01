/**
 * @file command_bus.h
 * @brief Central command routing — all control sources feed commands here.
 *
 * The command bus is a simple FreeRTOS queue.  Any source (RF remote,
 * BLE, Wi-Fi, serial console) can submit a command, and the main
 * control loop consumes them one at a time.
 */

#pragma once

#include <stdbool.h>

#include "app_types.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Maximum number of commands that can be queued. */
#define COMMAND_QUEUE_LENGTH  4U

/** @brief A command message including its source for logging. */
typedef struct {
    command_t        cmd;    /**< The command to execute */
    command_source_t source; /**< Who issued it */
} command_msg_t;

/**
 * @brief Create the command queue.  Call once at startup.
 *
 * @return ESP_OK on success, ESP_FAIL if queue creation failed.
 */
esp_err_t command_bus_init(void);

/**
 * @brief Submit a command to the bus (non-blocking).
 *
 * Safe to call from any task or ISR context.
 *
 * @param cmd    The command to submit.
 * @param source Identifies the caller for debug logging.
 * @return ESP_OK if queued, ESP_FAIL if the queue is full.
 */
esp_err_t command_bus_submit(command_t cmd, command_source_t source);

/**
 * @brief Get the next pending command (non-blocking).
 *
 * @param msg Pointer to receive the command message.
 * @return true if a command was available, false if queue is empty.
 */
bool command_bus_get_next(command_msg_t *msg);

#ifdef __cplusplus
}
#endif
