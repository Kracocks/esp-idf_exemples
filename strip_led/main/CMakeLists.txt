idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES heap freertos spi_flash esp_driver_gpio led_strip
                       INCLUDE_DIRS "")