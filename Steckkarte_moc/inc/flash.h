/*
 * flash.h
 *
 *  Created on: 12.08.2020
 *      Author: Johannes Wittmann
 */

#ifndef FLASH_H_
#define FLASH_H_

void init_flash();
void write_block(uint32_t address, uint8_t* data);
void read_block(uint32_t address, uint8_t* data);

#endif /* FLASH_H_ */
