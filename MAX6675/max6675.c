/*
 * max6675.c
 *
 *  Created on: 23/02/2020
 *      Author: Evandro Teixeira
 */

#include "max6675.h"

/**
 * @brief
 */
typedef union
{
	uint16_t byte;
	struct
	{
		unsigned A00 : 1;
		unsigned A01 : 1;
		unsigned A02 : 1;
		unsigned value : 12;
		unsigned A15 : 1;
	};
}data_t;

/**
 *
 */
static data_t max6675_read_data(void);
static void max6675_delay(uint16_t t);

/**
 * @brief
 */
static data_t max6675_read_data(void)
{
	data_t data = {0};

	gpio_write(D10,LOW);

	max6675_delay(5);

	data.byte = spi_read(SPI);
	data.byte <<= 8;
	data.byte |= spi_read(SPI);

	max6675_delay(5);

	gpio_write(D10,HIGH);
	return data;
}

/**
 * @brief
 */
static void max6675_delay(uint16_t t)
{
	while(t--)
	{
		__asm("nop");
	}
}

/**
 * @brief
 */
void max6675_init(void)
{
	spi_init(SPI_CONFIG);
	gpio_init(D10,OUTPUT);
	gpio_write(D10,HIGH);
}

/**
 * @brief
 * @param
 * @return
 */
bool max6675_reads_temperature(MAX6675_TemperatureType_t type,float *value)
{
	data_t data = {0};

	data = max6675_read_data();

	/* Checks if the thermocouple is connected correctly */
	if(data.A02 == 1)
	{
		return false;
	}

	*value = (float)(data.value * 0.25f);

	if(type == MAX6675_Fahrenheit)
	{
		*value = (float)(*value * 2.25f);
		*value += 32.00f;
	}
	else if(type == MAX6675_Kelvin)
	{
		*value += 273.15f;
	}

	return true;
}
