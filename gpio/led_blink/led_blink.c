#include "pico/stdlib.h"

#define LED_PIN 2

int main() 
{
    stdio_init_all();
    sleep_ms(1000);  // Allow USB serial to initialize

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);  // Turn LED ON
        printf("LED ON\n");
        sleep_ms(1000);

        gpio_put(LED_PIN, 0);  // Turn LED OFF
        printf("LED OFF\n");
        sleep_ms(1000);
    }

    return 0;
}
