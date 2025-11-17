#ifndef __PWM_TASK_H__
#define __PWM_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Stack sizes of our threads in words (4 bytes)
#define PWM_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// Priorities of our threads - higher numbers are higher priority
#define PWM_TASK_PRIORITY      ( tskIDLE_PRIORITY + 3UL )

// Macro to convert voltage to pwm value
#define VOLT_TO_PWM_COUNT(volt) ((uint32_t)(((volt) / MAX_VOLT) * MAX_PWM))

extern TaskHandle_t pwmHandle;
extern QueueHandle_t pwm_update_queue;

void vTaskPwmLed(__unused void *params);

#endif
