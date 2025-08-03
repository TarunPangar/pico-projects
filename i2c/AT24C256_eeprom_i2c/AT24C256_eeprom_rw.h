#ifndef __AT24C256_EEPROM_RW_H__ 
#define __AT24C256_EEPROM_RW_H__

void AT24C256_write(uint16_t mem_address, uint8_t data);
uint8_t AT24C256_read(uint16_t mem_address);
void AT24C256_page_write(uint16_t mem_address, uint8_t *data, int length);
uint8_t *AT24C256_seq_read(uint16_t mem_address, int length);

#define PAGE_SIZE 64

#endif
