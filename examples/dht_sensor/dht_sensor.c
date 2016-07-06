/* 
 *
 * This sample code is in the public domain.
 */
#include <stdio.h>
#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "dht.h"
#include "esp8266.h"

/* An example using the ubiquitous DHT** humidity sensors
 * to read and print a new temperature and humidity measurement
 * from a sensor attached to GPIO pin 4.
 */
uint8_t const dht_gpio = 12;

void dhtMeasurementTask(void *pvParameters)
{
    int16_t temperature = 0;
    int16_t humidity = 0;

    // DHT sensors that come mounted on a PCB generally have
    // pull-up resistors on the data pin.  It is recommended
    // to provide an external pull-up resistor otherwise...
    gpio_set_pullup(dht_gpio, false, false);

    while(1) {
        if (dht_read_data(dht_gpio, &humidity, &temperature)) {
            printf("Humidity: %d.%d%% Temp: %d.%dC\n", 
                    humidity / 10, humidity % 10, 
                    temperature / 10, abs(temperature) % 10);
        } else {
            printf("Could not read data from sensor\n");
        }

        // Three second delay...
        vTaskDelay(3000 / portTICK_RATE_MS);
    }
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    xTaskCreate(dhtMeasurementTask, (signed char *)"dhtMeasurementTask", 256, NULL, 2, NULL);
}

