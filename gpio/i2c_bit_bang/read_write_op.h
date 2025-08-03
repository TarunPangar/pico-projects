#ifndef __READ_WRITE_OP_H__
#define __READ_WRITE_OP_H__

#define SDA_PIN 0
#define SCL_PIN 1
#define SDA_TEST 2
#define SCL_TEST 3

#define SCL_HIGH    gpio_put(SCL_PIN, 1)
#define SCL_LOW     gpio_put(SCL_PIN, 0)
#define SDA_HIGH    gpio_put(SDA_PIN, 1)
#define SDA_LOW     gpio_put(SDA_PIN, 0)
#define DELAY       sleep_us(1.25)
#define SDA_READ    gpio_get(SDA_PIN)

void start();
void stop();
int write_data(uint8_t slave_addr, uint16_t mem_addr, uint8_t data);
int read_data(uint8_t slave_addr, uint16_t mem_addr, uint8_t *data);

#endif
