/**
 * @file board.c
 * @brief Board-level GPIO initialisation for ESP32 DevKit V1.
 */

#include "board.h"

#include "esp_log.h"

static const char *TAG = "board";

/** @brief Configure relay output pins (default: off / LOW). */
static esp_err_t init_relay_outputs(void)
{
    const gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << PIN_RELAY_EXTEND) |
                        (1ULL << PIN_RELAY_RETRACT),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };

    esp_err_t ret = gpio_config(&cfg);
    if (ret != ESP_OK) {
        return ret;
    }

    /* Ensure relays start de-energised */
    gpio_set_level(PIN_RELAY_EXTEND,  0);
    gpio_set_level(PIN_RELAY_RETRACT, 0);

    return ESP_OK;
}

/** @brief Configure limit-switch inputs with internal pull-ups. */
static esp_err_t init_limit_inputs(void)
{
    const gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << PIN_LIMIT_EXTEND) |
                        (1ULL << PIN_LIMIT_RETRACT),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };

    return gpio_config(&cfg);
}

/** @brief Configure built-in LED and BOOT button. */
static esp_err_t init_devkit_peripherals(void)
{
    const gpio_config_t led_cfg = {
        .pin_bit_mask = (1ULL << PIN_LED_BUILTIN),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };

    esp_err_t ret = gpio_config(&led_cfg);
    if (ret != ESP_OK) {
        return ret;
    }

    const gpio_config_t btn_cfg = {
        .pin_bit_mask = (1ULL << PIN_BUTTON_BOOT),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };

    return gpio_config(&btn_cfg);
}

esp_err_t board_init(void)
{
    esp_err_t ret;

    ret = init_relay_outputs();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Relay output init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = init_limit_inputs();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Limit input init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = init_devkit_peripherals();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "DevKit peripheral init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Board init complete");
    return ESP_OK;
}
