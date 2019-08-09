/*
 * ci74hc595.h
 *
 *  Created on: 07/08/2019
 *      Author: Evandro Teixeira
 */

#ifndef THIRDPARTKL05Z_CI74HC595_CI74HC595_H_
#define THIRDPARTKL05Z_CI74HC595_CI74HC595_H_

#include "../KL05Z_Libraries/board_frdm_kl05z.h"

void ci74hc595_init(GPIO_MemMapPtr gpio_CLK,	uint32_t pin_CLK,	// Pin CLK
					GPIO_MemMapPtr gpio_LATCH,	uint32_t pin_LATCH,	// Pin LATCH
					GPIO_MemMapPtr gpio_DATA,	uint32_t pin_DATA);	// Pin DATA

bool ci74hc595_transmits_8_bits(uint8_t data);
bool ci74hc595_transmits_16_bits(uint16_t data);


#endif /* THIRDPARTKL05Z_CI74HC595_CI74HC595_H_ */
