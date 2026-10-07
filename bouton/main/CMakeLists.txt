idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES spi_flash esp_driver_gpio espressif__button
                       INCLUDE_DIRS "")