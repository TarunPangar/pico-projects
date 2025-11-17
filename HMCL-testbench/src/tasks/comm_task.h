#ifndef __COMM_TASK_H__
#define __COMM_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Stack sizes of our threads in words (4 bytes)
#define CLI_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// Priorities of our threads - higher numbers are higher priority
#define CLI_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )

#define CLI_BUFFER_LEN	128

extern TaskHandle_t cliHandle;
extern QueueHandle_t pwm_update_queue;

void vTaskCliComm(__unused void *params);

#endif
