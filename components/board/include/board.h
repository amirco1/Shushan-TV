#pragma once

#include "driver/gpio.h"
#include "driver/ledc.h"

#ifdef __cplusplus
extern "C" {
#endif

// Board-level motor pin map (update for your custom PCB).
#define BOARD_MOTOR1_PWM_GPIO       GPIO_NUM_18
#define BOARD_MOTOR1_DIR_GPIO       GPIO_NUM_19

// Board-level LEDC routing for motor #1.
#define BOARD_MOTOR1_LEDC_TIMER     LEDC_TIMER_0
#define BOARD_MOTOR1_LEDC_CHANNEL   LEDC_CHANNEL_0

// PWM defaults. Keep conservative values until tuned on hardware.
#define BOARD_MOTOR_PWM_FREQ_HZ     20000
#define BOARD_MOTOR_PWM_RES         LEDC_TIMER_10_BIT

void board_init(void);

#ifdef __cplusplus
}
#endif
