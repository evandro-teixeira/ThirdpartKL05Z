/*
 * lcd16x2.h
 *
 *  Created on: 03/05/2020
 *      Author: Evandro Teixeira
 */

#ifndef THIRDPARTKL05Z_LCD16X2_LCD16X2_H_
#define THIRDPARTKL05Z_LCD16X2_LCD16X2_H_


#include "../KL05Z_Libraries/board_frdm_kl05z.h"

#define LCD16X2_NUMBER_PIN	7
#define LCD16X2_LINE1		0
#define LCD16X2_LINE2		1

/**
 * @brief
 * @param	Pin RS
 * @param	Pin RW
 * @param	Pin EN
 * @param 	Pin D4
 * @param	Pin D5
 * @param	Pin D6
 * @param 	Pin D7
 */
void lcd16x2_init(	GPIO_MemMapPtr gpio_rs,	uint32_t pin_rs,	// Pin RS
					GPIO_MemMapPtr gpio_rw,	uint32_t pin_rw,	// Pin RW
					GPIO_MemMapPtr gpio_en,	uint32_t pin_en,	// Pin EN
					GPIO_MemMapPtr gpio_d4,	uint32_t pin_d4,	// Pin D4
					GPIO_MemMapPtr gpio_d5,	uint32_t pin_d5,	// Pin D5
					GPIO_MemMapPtr gpio_d6,	uint32_t pin_d6,	// Pin D6
					GPIO_MemMapPtr gpio_d7,	uint32_t pin_d7);	// Pin D7


void lcd16x2_write_string(uint8_t line,char *c);
void lcd16x2_xy(uint8_t x, uint8_t y);
void lcd16x2_write_char(char c);

#endif /* THIRDPARTKL05Z_LCD16X2_LCD16X2_H_ */
