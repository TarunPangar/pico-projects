#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#include "pwm_task.h"
#include "common.h"

static void pwmLedInit(int pin);

static void pwmLedInit(int pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configure and start the slice only if it was disabled
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);

    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

void vTaskPwmLed(__unused void *params)
{
    sensor_val_t received_data;

    pwmLedInit(LED_PIN);

    while(1) {
        // Block indefinitely (portMAX_DELAY) until data arrives
        if (xQueueReceive(pwm_update_queue, &received_data, portMAX_DELAY) == pdPASS) {
            // 2. Data received, now update the PWM hardware
            pwm_set_gpio_level(LED_PIN, VOLT_TO_PWM_COUNT(received_data.value));
        }
    }
}
