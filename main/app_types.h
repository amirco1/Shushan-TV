/**
 * @file app_types.h
 * @brief Shared application types and enumerations.
 *
 * All command sources (RF remote, BLE, Wi-Fi, serial) and the motor
 * control layer share these types so they speak the same language.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief High-level commands that any control source can issue. */
typedef enum {
    CMD_NONE = 0,   /**< No pending command */
    CMD_EXTEND,     /**< Extend the actuator (lower TV) */
    CMD_RETRACT,    /**< Retract the actuator (raise TV) */
    CMD_STOP        /**< Immediately stop motion */
} command_t;

/** @brief Motor / actuator state machine states. */
typedef enum {
    MOTOR_STATE_IDLE = 0,       /**< No motion, relays off */
    MOTOR_STATE_EXTENDING,      /**< Relay FWD energised, extending */
    MOTOR_STATE_RETRACTING,     /**< Relay REV energised, retracting */
    MOTOR_STATE_STOPPED_LIMIT,  /**< Stopped because a limit was hit */
    MOTOR_STATE_FAULT           /**< Fault latched, manual clear needed */
} motor_state_t;

/** @brief Command source identifier for logging / priority. */
typedef enum {
    CMD_SRC_NONE = 0,
    CMD_SRC_SERIAL,     /**< Debug serial console */
    CMD_SRC_RF433,      /**< 433 MHz remote */
    CMD_SRC_BLE,        /**< Bluetooth */
    CMD_SRC_WIFI        /**< Wi-Fi / cloud */
} command_source_t;

#ifdef __cplusplus
}
#endif
