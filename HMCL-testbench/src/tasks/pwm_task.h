#ifndef __PWM_TASK_H__
#define __PWM_TASK_H__

#include "FreeRTOS.h"
#include "task.h"

// Stack sizes of our threads in words (4 bytes)
#define PWM_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// Priorities of our threads - higher numbers are higher priority
#define PWM_TASK_PRIORITY      ( tskIDLE_PRIORITY + 3UL )

extern TaskHandle_t pwmHandle;

void vTaskPwmLed(__unused void *params);

#endif
