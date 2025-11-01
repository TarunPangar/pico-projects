#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "comm_task.h"
#include "common.h"

void system_init()
{
    stdio_init_all();
    sleep_ms(1000);  // Allow USB serial to initialize

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void vTaskCliComm(__unused void *params)
{
    system_init();

    while (true) {
        gpio_put(LED_PIN, 1);  // Turn LED ON
        printf("LED ON\n");
        sleep_ms(1000);

        gpio_put(LED_PIN, 0);  // Turn LED OFF
        printf("LED OFF\n");
        sleep_ms(1000);
    }
}
