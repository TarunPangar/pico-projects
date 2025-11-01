#include <stdio.h>
#include "pico/stdlib.h"

#include "comm_task.h"
#include "common.h"

TaskHandle_t cliHandle;

int main()
{
    xTaskCreate(vTaskCliComm, "CliThread", CLI_TASK_STACK_SIZE, NULL, CLI_TASK_PRIORITY, &cliHandle);

    vTaskStartScheduler();
    
    return 0;
}
