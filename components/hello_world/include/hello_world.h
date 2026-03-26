/**
 * @file hello_world.h
 * @brief Bring-up module: blinks the built-in LED and prints "Hello World!"
 *        when the BOOT button is pressed.
 *
 * Use this module to verify that the DevKit is programmed correctly
 * and that basic GPIO + serial output work before moving on to
 * motor control.
 */

#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start the hello-world bring-up task.
 *
 * Creates a FreeRTOS task that:
 * - Toggles the built-in LED at ~2 Hz (visible blink)
 * - Polls the BOOT button each cycle
 * - Prints "Hello World!" to the serial monitor on each button press
 *
 * @return ESP_OK on success, or an error code if the task could not start.
 *
 * @note Call board_init() before this function so that GPIO pins
 *       are already configured.
 */
esp_err_t hello_world_start(void);

#ifdef __cplusplus
}
#endif
