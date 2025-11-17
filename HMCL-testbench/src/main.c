#include <stdio.h>
#include "pico/stdlib.h"

#include "comm_task.h"
#include "pwm_task.h"
#include "common.h"

TaskHandle_t cliHandle;
TaskHandle_t pwmHandle;

// Define the queue handle
QueueHandle_t pwm_update_queue;
sensor_val_t sensor_data;

void system_init()
{
    stdio_init_all();
    sleep_ms(1000);

    // Queue that can hold 5 items of type sensor_val_t
    pwm_update_queue = xQueueCreate(5, sizeof(sensor_val_t));
    if (pwm_update_queue == NULL) {
        printf("ERROR: xQueueCreate failed\n");
    }
}

int main()
{
    // Initialize UART and queue
    system_init();

    xTaskCreate(vTaskPwmLed, "PwmThread", PWM_TASK_STACK_SIZE, NULL, PWM_TASK_PRIORITY, &pwmHandle);
    xTaskCreate(vTaskCliComm, "CliThread", CLI_TASK_STACK_SIZE, NULL, CLI_TASK_PRIORITY, &cliHandle);

    vTaskStartScheduler();
    
    return 0;
}
