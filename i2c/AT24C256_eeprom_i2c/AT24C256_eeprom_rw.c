#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AT24C256_eeprom_rw.h"

extern uint8_t ADDRESS;

void AT24C256_write(uint16_t mem_address, uint8_t data)
{
    uint8_t buf[3];
    buf[0] = (mem_address >> 8) & 0xff; // MSB first
    buf[1] = mem_address & 0xff;        // LSB next
    buf[2] = data;                      // data to be written at that address

    i2c_write_blocking(i2c_default, ADDRESS, buf, 3, false);
    sleep_ms(20);
    printf("Data written at address: %x\n", mem_address);
}

uint8_t AT24C256_read(uint16_t mem_address)
{
    uint8_t buf[2];
    buf[0] = (mem_address >> 8) & 0xff; // MSB first
    buf[1] = mem_address & 0xff;        // LSB next

    uint8_t data;

    i2c_write_blocking(i2c_default, ADDRESS, buf, 2, true);
    i2c_read_blocking(i2c_default, ADDRESS, &data, 1, false);

    return data;
}

void AT24C256_page_write(uint16_t mem_address, uint8_t *data, int length)
{
    while(length > 0) {
        int space_in_page = PAGE_SIZE - (mem_address % PAGE_SIZE);
        int write_size = (length < space_in_page) ? length : space_in_page;

        uint8_t buf[2 + PAGE_SIZE];
        buf[0] = (mem_address >> 8) & 0xff; // MSB first
        buf[1] = mem_address & 0xff;        // LSB next
        memcpy(&buf[2], data, write_size);

        i2c_write_blocking(i2c_default, ADDRESS, buf, 2 + write_size, false);
        sleep_ms(20);

        mem_address += write_size;
        data += write_size;
        length -= write_size;
    }
}

uint8_t *AT24C256_seq_read(uint16_t mem_address, int length)
{
    uint8_t buf[2];
    buf[0] = (mem_address >> 8) & 0xff; // MSB first
    buf[1] = mem_address & 0xff;        // LSB next

    uint8_t *data = (uint8_t *) malloc(length * sizeof(uint8_t));

    i2c_write_blocking(i2c_default, ADDRESS, buf, 2, true);
    i2c_read_blocking(i2c_default, ADDRESS, data, length, false);

    return data;
}
