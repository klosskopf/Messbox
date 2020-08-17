/*
 * flash.h
 *
 *  Created on: 12.08.2020
 *      Author: Johannes Wittmann
 */

#ifndef FLASH_H_
#define FLASH_H_

typedef enum {IDLE, READ_DATA, ERASE_DATA, ERASE_ALL, WRITE_DATA}FlashStates_t;

void init_flash();
void write_block(volatile uint32_t address, volatile uint8_t* data);
void read_block(volatile uint32_t address, volatile uint8_t* data);
void erase_block(volatile uint32_t address);
void erase_device();
FlashStates_t get_flash_state();

#endif /* FLASH_H_ */
