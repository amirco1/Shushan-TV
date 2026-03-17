#include "motor.h"

#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "motor";

static uint32_t motor_max_duty(const motor_t *motor)
{
    return (1U << motor->cfg.pwm_resolution) - 1U;
}

esp_err_t motor_init(motor_t *motor, const motor_init_config_t *cfg)
{
    ESP_RETURN_ON_FALSE(motor != NULL, ESP_ERR_INVALID_ARG, TAG, "motor is NULL");
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "cfg is NULL");

    motor->cfg = *cfg;

    ledc_timer_config_t timer_cfg = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = cfg->pwm_resolution,
        .timer_num = cfg->ledc_timer,
        .freq_hz = (int)cfg->pwm_freq_hz,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_RETURN_ON_ERROR(ledc_timer_config(&timer_cfg), TAG, "ledc_timer_config failed");

    ledc_channel_config_t channel_cfg = {
        .gpio_num = cfg->pwm_gpio,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = cfg->ledc_channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = cfg->ledc_timer,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_RETURN_ON_ERROR(ledc_channel_config(&channel_cfg), TAG, "ledc_channel_config failed");

    gpio_config_t dir_gpio_cfg = {
        .pin_bit_mask = (1ULL << cfg->dir_gpio),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&dir_gpio_cfg), TAG, "gpio_config dir failed");

    ESP_RETURN_ON_ERROR(gpio_set_level(cfg->dir_gpio, 0), TAG, "gpio_set_level dir failed");
    motor->initialized = true;

    ESP_LOGI(TAG, "Motor initialized: pwm_gpio=%d dir_gpio=%d", cfg->pwm_gpio, cfg->dir_gpio);
    return ESP_OK;
}

esp_err_t motor_set_open_loop(motor_t *motor, motor_direction_t direction, uint8_t duty_percent)
{
    ESP_RETURN_ON_FALSE(motor != NULL && motor->initialized, ESP_ERR_INVALID_STATE, TAG,
                        "motor not initialized");
    ESP_RETURN_ON_FALSE(duty_percent <= 100U, ESP_ERR_INVALID_ARG, TAG, "duty out of range");

    const uint32_t duty_raw = (motor_max_duty(motor) * duty_percent) / 100U;

    ESP_RETURN_ON_ERROR(gpio_set_level(motor->cfg.dir_gpio, direction == MOTOR_DIRECTION_REVERSE),
                        TAG, "gpio_set_level dir failed");
    ESP_RETURN_ON_ERROR(ledc_set_duty(LEDC_LOW_SPEED_MODE, motor->cfg.ledc_channel, duty_raw),
                        TAG, "ledc_set_duty failed");
    ESP_RETURN_ON_ERROR(ledc_update_duty(LEDC_LOW_SPEED_MODE, motor->cfg.ledc_channel),
                        TAG, "ledc_update_duty failed");

    return ESP_OK;
}

esp_err_t motor_stop(motor_t *motor)
{
    return motor_set_open_loop(motor, MOTOR_DIRECTION_FORWARD, 0);
}
