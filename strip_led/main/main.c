#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "led_strip.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>

#define LED_PIN 16

#define LED_NUMBER 3

static const char *TAG = "example";

void app_main() {
    // Configure the LED pin as output
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;        // Disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT;              // Set as output mode
    io_conf.pin_bit_mask = (1ULL << LED_PIN);     // Bit mask of the pins to set
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Disable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;     // Disable pull-up mode
    gpio_config(&io_conf); // Configure GPIO with the given settings

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_PIN,   // The GPIO that connected to the LED strip's data line
        .max_leds = LED_NUMBER,        // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal
    };

    led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .flags.with_dma = true,         // Using DMA can improve performance and help drive more LEDs
        .spi_bus = SPI2_HOST,           // SPI bus ID
    };

    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
    ESP_LOGI(TAG, "Created LED strip object with SPI backend");

    printf("Hello, Wokwi!\n");

    while (true) {
      bool led_on_off = false;

      ESP_LOGI(TAG, "Start blinking LED strip");
      while (1) {
        if (led_on_off) {
          /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each
           * color */
          for (int i = 0; i < LED_NUMBER; i++) {
            ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, 255, 255, 255));
          }
          /* Refresh the strip to send data */
          ESP_ERROR_CHECK(led_strip_refresh(led_strip));
          ESP_LOGI(TAG, "LED ON!");
        } else {
          /* Set all LED off to clear all pixels */
          ESP_ERROR_CHECK(led_strip_clear(led_strip));
          ESP_LOGI(TAG, "LED OFF!");
        }

        led_on_off = !led_on_off;
        vTaskDelay(pdMS_TO_TICKS(500));
      }
    }
}
