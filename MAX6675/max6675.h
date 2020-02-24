/*
 * max6675.h
 *
 *  Created on: 23/02/2020
 *      Author: Evandro Teixeira
 */

#ifndef THIRDPARTKL05Z_MAX6675_MAX6675_H_
#define THIRDPARTKL05Z_MAX6675_MAX6675_H_


#include "../KL05Z_Libraries/board_frdm_kl05z.h"
#include <stdbool.h>

typedef enum
{
	MAX6675_Celsius = 0,
	MAX6675_Fahrenheit,
	MAX6675_Kelvin
}MAX6675_TemperatureType_t;

/*
 * CLK: 180 KHz
 * SPI0
 * D12 ~ PTA6: Miso
 * D11 ~ PTA7: Mosi
 * D13 ~ PTB0: SCK
 * D10 ~ PTA5: SS
 */
void max6675_init(void);
bool max6675_reads_temperature(MAX6675_TemperatureType_t type,float *value);

#endif /* THIRDPARTKL05Z_MAX6675_MAX6675_H_ */
