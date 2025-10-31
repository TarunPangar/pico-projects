#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "FreeRTOS.h"
#include "task.h"

#define LED_PIN 25

// Stack sizes of our threads in words (4 bytes)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// Priorities of our threads - higher numbers are higher priority
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )

void main_task(__unused void *params)
{
    while (true) {
        gpio_put(LED_PIN, 1);  // Turn LED ON
        printf("LED ON\n");
        sleep_ms(1000);

        gpio_put(LED_PIN, 0);  // Turn LED OFF
        printf("LED OFF\n");
        sleep_ms(1000);
    }
}

int main()
{
    TaskHandle_t task;
    
//    stdio_init_all();
//    sleep_ms(1000);  // Allow USB serial to initialize

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    xTaskCreate(main_task, "MainThread", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);

    (void)task;
    vTaskStartScheduler();
    
    return 0;
}
