#include "button_gpio.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iot_button.h"
#include <stdio.h>

#define LED_PIN 18
#define BUTTON_PIN 17

#define BUTTON_ACTIVE_LEVEL 0

static const char* TAG_BUTTON = "BUTTON";

static void button_press_up_event_cb(void *arg, void *data) {
    ESP_LOGI(TAG_BUTTON, "Button press up!");
    gpio_set_level(LED_PIN, 0);
}

static void button_press_down_event_cb(void *arg, void *data) {
    ESP_LOGI(TAG_BUTTON, "Button press down!");
    gpio_set_level(LED_PIN, 1);
}

void app_main() {
    // Configure the LED pin as output
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;        // Disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT;              // Set as output mode
    io_conf.pin_bit_mask = (1ULL << LED_PIN) | (1ULL << BUTTON_PIN);     // Bit mask of the pins to set
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Disable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;     // Disable pull-up mode
    gpio_config(&io_conf); // Configure GPIO with the given settings

    // Configure the button
    const button_config_t btn_cfg = { // Button led
        .long_press_time = 5000, // Long press time in milliseconds
        .short_press_time = 200, // Short press time in milliseconds
    };


    const button_gpio_config_t btn_gpio_cfg = { // Button led
        .gpio_num = BUTTON_PIN,
        .active_level = BUTTON_ACTIVE_LEVEL,
        .disable_pull = false,
    };

    button_handle_t btn;
    // Create a new button device
    esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &btn);
    ESP_ERROR_CHECK(ret);

    // Configure the callbacks
    ret = iot_button_register_cb(btn, BUTTON_PRESS_UP, NULL,
                                 button_press_up_event_cb, NULL);
    ESP_ERROR_CHECK(ret);
    ret = iot_button_register_cb(btn, BUTTON_PRESS_DOWN, NULL,
                                 button_press_down_event_cb, NULL);
    ESP_ERROR_CHECK(ret);

    printf("Hello, Wokwi!\n");

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
