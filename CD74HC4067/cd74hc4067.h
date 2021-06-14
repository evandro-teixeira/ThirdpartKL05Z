/*
 * cd74hc4067.h
 *
 *  Created on: 13/06/2021
 *      Author: Evandro Teixeira
 */

#ifndef THIRDPARTKL05Z_CD74HC4067_CD74HC4067_H_
#define THIRDPARTKL05Z_CD74HC4067_CD74HC4067_H_

#include "../KL05Z_Libraries/board_frdm_kl05z.h"

typedef enum
{
	Channel_0 = 0,
	Channel_1,
	Channel_2,
	Channel_3,
	Channel_4,
	Channel_5,
	Channel_6,
	Channel_7,
	Channel_8,
	Channel_9,
	Channel_10,
	Channel_11,
	Channel_12,
	Channel_13,
	Channel_14,
	Channel_15,
	Channel_None
}cd74hc4067_Channel_t;

void cd74hc4067_Init(GPIO_MemMapPtr gpio_S0, uint32_t pin_S0,
					 GPIO_MemMapPtr gpio_S1, uint32_t pin_S1,
					 GPIO_MemMapPtr gpio_S2, uint32_t pin_S2,
					 GPIO_MemMapPtr gpio_S3, uint32_t pin_S3,
					 GPIO_MemMapPtr gpio_EN, uint32_t pin_EN);

bool cd74hc4067_SetChannel(cd74hc4067_Channel_t channel);

#endif /* THIRDPARTKL05Z_CD74HC4067_CD74HC4067_H_ */
