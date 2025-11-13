#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#include "pwm_task.h"
#include "common.h"

void on_pwm_wrap() {
    static uint32_t  led = 62500;
    static uint32_t  gpio1 = MAX_PWM;
    static uint32_t  gpio2 = MAX_PWM / 2;
    static bool going_up = true;
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(LED_PIN));
    pwm_clear_irq(pwm_gpio_to_slice_num(GPIO1_PIN));
    pwm_clear_irq(pwm_gpio_to_slice_num(GPIO2_PIN));

    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value
    pwm_set_gpio_level(LED_PIN, led);
    pwm_set_gpio_level(GPIO1_PIN,  gpio1);
    pwm_set_gpio_level(GPIO2_PIN, gpio2);
}

void pwmLedInit(int pins)
{
    // Tell the LED pin that the PWM is in charge of its value.
    gpio_set_function(pins, GPIO_FUNC_PWM);
    // Figure out which slice we just connected to the LED pin
    uint slice_num = pwm_gpio_to_slice_num(pins);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), on_pwm_wrap);
    irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 4.f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);
}

void vTaskPwmLed(__unused void *params)
{
    pwmLedInit(LED_PIN);
    pwmLedInit(GPIO1_PIN);
    pwmLedInit(GPIO2_PIN);

    while(1)
        tight_loop_contents();
}
