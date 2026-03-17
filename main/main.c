#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "board.h"
#include "control.h"
#include "motor.h"
#include "safety.h"

static const char *TAG = "app";

void app_main(void)
{
    ESP_LOGI(TAG, "Starting brushed DC motor skeleton firmware");

    board_init();

    // Motor + control init for a single open-loop motor channel.
    motor_t motor1;
    motor_init_config_t motor1_cfg = {
        .pwm_gpio = BOARD_MOTOR1_PWM_GPIO,
        .dir_gpio = BOARD_MOTOR1_DIR_GPIO,
        .ledc_timer = BOARD_MOTOR1_LEDC_TIMER,
        .ledc_channel = BOARD_MOTOR1_LEDC_CHANNEL,
        .pwm_freq_hz = BOARD_MOTOR_PWM_FREQ_HZ,
        .pwm_resolution = BOARD_MOTOR_PWM_RES,
    };
    ESP_ERROR_CHECK(motor_init(&motor1, &motor1_cfg));

    control_ctx_t control;
    control_init(&control, &motor1);

    safety_ctx_t safety;
    safety_init(&safety);

    // Demo open-loop ramp: 0 -> 30 -> 60 -> 30 -> 0 (% duty)
    const int speed_sequence[] = {0, 30, 60, 30, 0};
    const size_t speed_sequence_len = sizeof(speed_sequence) / sizeof(speed_sequence[0]);
    size_t i = 0;

    while (true) {
        control_command_t cmd = {
            .direction = MOTOR_DIRECTION_FORWARD,
            .speed_percent = speed_sequence[i],
        };

        // In future closed-loop mode, this will run scheduling and regulator logic.
        ESP_ERROR_CHECK(control_apply_open_loop(&control, &cmd));

        // Placeholder safety check:
        // - over-current from ADC
        // - encoder timeout / stalled rotor detection
        // - undervoltage / thermal flags
        if (safety_check(&safety) != ESP_OK) {
            ESP_LOGE(TAG, "Safety fault: forcing motor stop");
            ESP_ERROR_CHECK(motor_stop(&motor1));
            break;
        }

        ESP_LOGI(TAG, "Applied open-loop command: dir=%d speed=%d%%",
                 cmd.direction, cmd.speed_percent);

        i = (i + 1) % speed_sequence_len;
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
