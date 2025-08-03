#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AT24C256_eeprom_rw.h"

#define SCAN_I2C_BUS 0

uint8_t ADDRESS = 0x54;

void scan_i2c_bus() 
{
    printf("Scanning I2C bus...\n");
    for (int addr = 0x08; addr <= 0x77; addr++) {
        uint8_t dummy = 0;
        int result = i2c_write_blocking(i2c_default, addr, &dummy, 1, false);
        if (result >= 0) {
            printf("Found I2C device at address 0x%02X\n", addr);
        }
    }
}

void init_i2c_pins()
{
	// This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico) 
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

int main() 
{
    stdio_init_all();
	sleep_ms(2000);
   
	printf("Hello, World! Tarun...\n");
	// Initialize pins for i2c0
	init_i2c_pins();

	// function to find the i2c slave address
	if (SCAN_I2C_BUS == 1)
		scan_i2c_bus();

	// Verify AT24C256_write and AT24C256_read function
	AT24C256_write(0x0010, 0xAA);
	uint8_t read_data = AT24C256_read(0x0010);
	printf("read data is: %x\n", read_data);

	// Verify AT24C256_page_write and AT24C256_seq_read function
	uint8_t data[100];
	for(int i = 0; i < 100; i++) {
		data[i] = i;
	}
	AT24C256_page_write(0x0040, data, 100);

	uint8_t *seq_data = AT24C256_seq_read(0x0040, 100);
	for(int i = 0; i < 100; i++) {
		printf("%d ", seq_data[i]);
	}
	printf("\n");

	free(seq_data);

}
