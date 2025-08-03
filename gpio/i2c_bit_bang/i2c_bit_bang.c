#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

#include "read_write_op.h"

void init_i2c_gpio()
{
	gpio_init(SCL_PIN);
	gpio_set_dir(SCL_PIN, GPIO_OUT);
	gpio_init(SDA_PIN);
	gpio_pull_up(SDA_PIN);
	gpio_pull_up(SCL_PIN);
}

void init_gpio_test()
{
	gpio_init(SCL_TEST);
	gpio_set_dir(SCL_TEST, GPIO_IN);
	gpio_init(SDA_TEST);
	gpio_set_dir(SDA_TEST, GPIO_IN);
}

int main()
{
	int ret;

    stdio_init_all();
	sleep_ms(2000);
    
	init_i2c_gpio();

	ret = write_data(0x54, 0x0100, 0x88);
	if(ret == -1) {
		printf("ERROR: in write\n");
	} else {
		printf("Success: wrtie data\n");
	}

	uint8_t data;
	ret = read_data(0x54, 0x0100, &data);
	if(ret == -1) {
        printf("ERROR: in read\n");
    } else {
        printf("Success: read data: %02X\n", data);
    }

}
