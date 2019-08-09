/*
 * ci74hc595.c
 *
 *  Created on: 07/08/2019
 *      Author: Evandro Teixeira
 */
#include "ci74hc595.h"

#define CI74HC595_BYTE			8
#define CI74HC595_TIME_DELAY	10
#define CI74HC595_8_BIT			1
#define CI74HC595_16_BIT		2

/**
 *
 */
typedef struct
{
	GPIO_MemMapPtr io;
	uint32_t pin;
}pin_t;

/**
 *
 */
static pin_t clk, latch, data;
static bool status_init = false;

/**
 *
 */
static bool ci74hc595_transmits_data(uint16_t value, uint8_t num_byte);
static void ci74hc595_delay(uint32_t t);

/**
 *
 */
void ci74hc595_init(GPIO_MemMapPtr gpio_CLK,	uint32_t pin_CLK,	// Pin CLK
					GPIO_MemMapPtr gpio_LATCH,	uint32_t pin_LATCH,	// Pin LATCH
					GPIO_MemMapPtr gpio_DATA,	uint32_t pin_DATA)	// Pin DATA
{
	clk.io = gpio_CLK;
	clk.pin = pin_CLK;

	latch.io = gpio_LATCH;
	latch.pin = pin_LATCH;

	data.io = gpio_DATA;
	data.pin = pin_DATA;

	gpio_init(clk.io,clk.pin,OUTPUT);
	gpio_init(latch.io,latch.pin,OUTPUT);
	gpio_init(data.io,data.pin,OUTPUT);

	status_init = true;
}

/**
 *
 */
bool ci74hc595_transmits_8_bits(uint8_t data)
{
	uint16_t value = (uint16_t)( data );
	return ci74hc595_transmits_data(value, CI74HC595_8_BIT );
}

/**
 *
 */
bool ci74hc595_transmits_16_bits(uint16_t data)
{
	return ci74hc595_transmits_data(data, CI74HC595_16_BIT );
}

/**
 *
 */
static bool ci74hc595_transmits_data(uint16_t value, uint8_t num_byte)
{
	uint8_t i = 0;

	if((status_init == true) && ((num_byte == 1 || num_byte == 2)))
	{
		for(i=0;i<(CI74HC595_BYTE*num_byte);i++)
		{
			if(value & 1)
			{
				gpio_write(data.io,data.pin,HIGH);
			}
			else
			{
				gpio_write(data.io,data.pin,LOW);
			}

			gpio_write(clk.io,clk.pin,LOW);
			ci74hc595_delay(CI74HC595_TIME_DELAY);
			gpio_write(clk.io,clk.pin,HIGH);
			value >>= 1;
		}

		gpio_write(latch.io,latch.pin,LOW);
		ci74hc595_delay(CI74HC595_TIME_DELAY);
		gpio_write(latch.io,latch.pin,HIGH);
		gpio_write(latch.io,latch.pin,LOW);

		return true;
	}
	else
	{
		return false;
	}
}

/**
 *
 */
static void ci74hc595_delay(uint32_t t)
{
	while(t--)
	{
		__NOP();
	}
}


