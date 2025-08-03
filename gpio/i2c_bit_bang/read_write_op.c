#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

#include "read_write_op.h"

void start()
{
    gpio_set_dir(SDA_PIN, GPIO_OUT);
    SDA_HIGH;
    DELAY;
    SCL_HIGH;
    DELAY;
    SDA_LOW;
    DELAY;
    SCL_LOW;
}

void stop()
{
    gpio_set_dir(SDA_PIN, GPIO_OUT);
    SDA_LOW;
    DELAY;
    SCL_HIGH;
    DELAY;
    SDA_HIGH;
    DELAY;
}

bool send_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++) {
        gpio_set_dir(SDA_PIN, GPIO_OUT);
        (byte & 0x80) ? SDA_HIGH : SDA_LOW;
        byte <<= 1;

        DELAY;
        SCL_HIGH;
        DELAY;
        SCL_LOW;
        DELAY;
    }

    // ACK
    gpio_set_dir(SDA_PIN, GPIO_IN);
    DELAY;
    SCL_HIGH;
    DELAY;
    bool ack = !SDA_READ;
    SCL_LOW;
    DELAY;
    gpio_set_dir(SDA_PIN, GPIO_OUT);

    return ack;
}

int write_data(uint8_t slave_addr, uint16_t mem_addr, uint8_t data)
{
	uint8_t bytes[3];
	bytes[0] = (mem_addr >> 8) & 0xff;
    bytes[1] = mem_addr & 0xff;
    bytes[2] = data;

	// START condition
	start();

	// Send slave_addr + write byte
	if (!send_byte((slave_addr << 1) | 0)) {
		printf("NACK received on slave addr\n");
		stop();
		return -1;
	}

	for (int i = 0; i < 3; i++) {
		if(!send_byte(bytes[i])) {
			printf("NACK received on byte\n");
			stop();
			return -1;
		}
	}

	stop();

	sleep_ms(10);

	return 0;
}

uint8_t receive_byte(bool send_ack)
{
    uint8_t byte = 0;
    gpio_set_dir(SDA_PIN, GPIO_IN);

    for (int i = 0; i < 8; i++) {
        SCL_HIGH;
        DELAY;
        byte = (byte << 1) | SDA_READ;
        SCL_LOW;
        DELAY;
    }

    // Send ACK or NACK
    gpio_set_dir(SDA_PIN, GPIO_OUT);
    if (send_ack) {
        SDA_LOW;  // ACK
    } else {
        SDA_HIGH; // NACK (for single byte read)
    }

    DELAY;
    SCL_HIGH;
    DELAY;
    SCL_LOW;
    DELAY;

    return byte;
}

int read_data(uint8_t slave_addr, uint16_t mem_addr, uint8_t *data)
{
	// START condition
    start();

	// Slave address + Write
    if (!send_byte((slave_addr << 1) | 0)) {
        printf("NACK on slave write addr\n");
        stop();
        return -1;
    }

    if (!send_byte((mem_addr >> 8) & 0xFF)) {
        printf("NACK on mem_addr MSB\n");
        stop();
        return -1;
    }

    if (!send_byte(mem_addr & 0xFF)) {
        printf("NACK on mem_addr LSB\n");
        stop();
        return -1;
    }

	// Repeated start
    start();

	// Slave addres + read bit
    if (!send_byte((slave_addr << 1) | 1)) {
        printf("NACK on slave read addr\n");
        stop();
        return -1;
    }

	// readins 1 byte and send NACK
    *data = receive_byte(false);
    stop();

    return 0;
}
