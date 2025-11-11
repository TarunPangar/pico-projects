#include <stdio.h>
#include "pico/stdlib.h"

#include "comm_task.h"
#include "pwm_task.h"
#include "common.h"

TaskHandle_t cliHandle;
TaskHandle_t pwmHandle;

int main()
{
    xTaskCreate(vTaskCliComm, "CliThread", CLI_TASK_STACK_SIZE, NULL, CLI_TASK_PRIORITY, &cliHandle);

    xTaskCreate(vTaskPwmLed, "PwmThread", PWM_TASK_STACK_SIZE, NULL, PWM_TASK_PRIORITY, &pwmHandle);

    vTaskStartScheduler();
    
    return 0;
}
