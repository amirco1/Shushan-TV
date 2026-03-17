#include "board.h"

#include "esp_log.h"

static const char *TAG = "board";

void board_init(void)
{
    // Board-level initialization hook.
    // Keep this for clocks, rails, transceiver enables, etc.
    ESP_LOGI(TAG, "Board init complete");
}
