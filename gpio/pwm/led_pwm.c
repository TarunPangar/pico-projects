#include <pico/stdlib.h>
#include "hardware/pwm.h"

#define LED_PIN 2

int main()
{
    uint slice;

    stdio_init_all();

    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    slice=pwm_gpio_to_slice_num (LED_PIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(slice, &config, true);

    int fade = 0;
    bool going_up = true;

    while(1) {
        pwm_set_gpio_level (LED_PIN, fade * fade);

        if (going_up) {
            ++fade;
            if (fade > 255) {
                fade = 255;
                going_up = false;
            }
        } else {
            --fade;
            if (fade < 0) {
                fade = 0;
                going_up = true;
            }
        }

        sleep_ms(5);
    }

    return 0;
}
