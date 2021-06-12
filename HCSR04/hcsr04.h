/*
 * hcsr04.h
 *
 *  Created on: 12/06/2021
 *      Author: Evandro Teixeira
 */

#ifndef THIRDPARTKL05Z_HCSR04_HCSR04_H_
#define THIRDPARTKL05Z_HCSR04_HCSR04_H_

#include "../KL05Z_Libraries/board_frdm_kl05z.h"

void hcsr04_Init(GPIO_MemMapPtr gpio_Trig,	uint32_t pin_Trig,
				GPIO_MemMapPtr gpio_Echo,	uint32_t pin_Echo);
bool hcsr04_GetDistance(uint32_t *Distance);

#endif /* THIRDPARTKL05Z_HCSR04_HCSR04_H_ */
