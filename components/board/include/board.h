/**
 * @file board.h
 * @brief Pin definitions and board-level initialisation for ESP32 DevKit V1.
 *
 * All GPIO assignments live here so that a pin change only requires
 * editing one file.  Update these values to match your PCB.
 *
 * @note Pin numbers below are provisional defaults.  Verify against
 *       your schematic before first power-up with the actuator connected.
 */

#pragma once

#include "driver/gpio.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Relay outputs (driving relay coils via transistor) ---- */
#define PIN_RELAY_EXTEND    GPIO_NUM_18  /**< Relay coil: extend direction */
#define PIN_RELAY_RETRACT   GPIO_NUM_19  /**< Relay coil: retract direction */

/* ---- Hall-effect limit inputs (open-collector, active-low) ---- */
#define PIN_LIMIT_EXTEND    GPIO_NUM_21  /**< Hall sensor at full-extend */
#define PIN_LIMIT_RETRACT   GPIO_NUM_22  /**< Hall sensor at full-retract */

/* ---- 433 MHz receiver data output ---- */
#define PIN_RF433_DATA      GPIO_NUM_23  /**< Data pin from 433 MHz Rx */

/* ---- On-board peripherals (DevKit V1) ---- */
#define PIN_LED_BUILTIN     GPIO_NUM_2   /**< Built-in blue LED */
#define PIN_BUTTON_BOOT     GPIO_NUM_0   /**< BOOT button, active-low */

/* ---- Timing constants ---- */

/** @brief Dead-time in ms before allowing direction reversal. */
#define RELAY_DEAD_TIME_MS          100U

/** @brief Max continuous motion time in ms before forced stop.
 *  Adjust after measuring real actuator travel time. */
#define MOTOR_TIMEOUT_MS            30000U

/** @brief Debounce window in ms for limit-switch inputs. */
#define LIMIT_DEBOUNCE_MS           20U

/** @brief Main control loop period in ms. */
#define CONTROL_LOOP_PERIOD_MS      50U

/**
 * @brief Initialise all board-level GPIO pins.
 *
 * Configures relay outputs as push-pull (default LOW / off),
 * limit inputs with pull-ups, button input with pull-up,
 * and LED output.
 *
 * @return ESP_OK on success, or an error code.
 */
esp_err_t board_init(void);

#ifdef __cplusplus
}
#endif
