/*
 * cd74hc4067.c
 *
 *  Created on: 13/06/2021
 *      Author: Evandro Teixeira
 */
#include "cd74hc4067.h"

/* // VALUES of each channel */
#define VALUES_CHANNEL			\
{								\
	{/* S0 S1 S2 S3 EN - Ch0 */	\
		0,0,0,0,0           	\
	},							\
	{/* S0 S1 S2 S3 EN - Ch1 */	\
		1,0,0,0,0 	 			\
	},							\
	{/* S0 S1 S2 S3 EN - Ch2 */	\
		0,1,0,0,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch3 */	\
		1,1,0,0,0	  			\
	},							\
	{/* S0 S1 S2 S3 EN - Ch4 */	\
		0,0,1,0,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch5 */	\
		1,0,1,0,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch6 */	\
		0,1,1,0,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch7 */	\
		1,1,1,0,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch8 */	\
		0,0,0,1,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch9 */	\
		1,0,0,1,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch10*/	\
		0,1,0,1,0 	 			\
	},							\
	{/* S0 S1 S2 S3 EN - Ch11*/	\
		1,1,0,1,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch12*/	\
		0,0,1,1,0 	 			\
	},							\
	{/* S0 S1 S2 S3 EN - Ch13*/	\
		1,0,1,1,0 	 			\
	},							\
	{/* S0 S1 S2 S3 EN - Ch14*/	\
		0,1,1,1,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - Ch15*/	\
		1,1,1,1,0  				\
	},							\
	{/* S0 S1 S2 S3 EN - None*/	\
		0,0,0,0,1  				\
	},							\
}

typedef struct
{
	bool Pin_S0;
	bool Pin_S1;
	bool Pin_S2;
	bool Pin_S3;
	bool Pin_EN;
}cd74hc4067_value_t;

typedef struct
{
	GPIO_MemMapPtr Gpio;
	uint32_t Pin;
}cd74hc4067_pin_t;

typedef struct
{
	cd74hc4067_pin_t S0;
	cd74hc4067_pin_t S1;
	cd74hc4067_pin_t S2;
	cd74hc4067_pin_t S3;
	cd74hc4067_pin_t EN;
}cd74hc4067_config_t;

/**
 *
 */
static const cd74hc4067_value_t Value_Pin[17] = VALUES_CHANNEL;
cd74hc4067_config_t Config;

/**
 *
 */
void cd74hc4067_Init(GPIO_MemMapPtr gpio_S0, uint32_t pin_S0,
					 GPIO_MemMapPtr gpio_S1, uint32_t pin_S1,
					 GPIO_MemMapPtr gpio_S2, uint32_t pin_S2,
					 GPIO_MemMapPtr gpio_S3, uint32_t pin_S3,
					 GPIO_MemMapPtr gpio_EN, uint32_t pin_EN)
{
	Config.S0.Gpio = gpio_S0;
	Config.S0.Pin  = pin_S0;
	Config.S1.Gpio = gpio_S1;
	Config.S1.Pin  = pin_S1;
	Config.S2.Gpio = gpio_S2;
	Config.S2.Pin  = pin_S2;
	Config.S3.Gpio = gpio_S3;
	Config.S3.Pin  = pin_S3;
	Config.EN.Gpio = gpio_EN;
	Config.EN.Pin  = pin_EN;

	gpio_init(Config.S0.Gpio,Config.S0.Pin,OUTPUT);
	gpio_write(Config.S0.Gpio,Config.S0.Pin,LOW);
	gpio_init(Config.S1.Gpio,Config.S1.Pin,OUTPUT);
	gpio_write(Config.S1.Gpio,Config.S1.Pin,LOW);
	gpio_init(Config.S2.Gpio,Config.S2.Pin,OUTPUT);
	gpio_write(Config.S2.Gpio,Config.S2.Pin,LOW);
	gpio_init(Config.S3.Gpio,Config.S3.Pin,OUTPUT);
	gpio_write(Config.S3.Gpio,Config.S3.Pin,LOW);
	gpio_init(Config.EN.Gpio,Config.EN.Pin,OUTPUT);
	gpio_write(Config.EN.Gpio,Config.EN.Pin,HIGH);

}

/**
 *
 */
bool cd74hc4067_SetChannel(cd74hc4067_Channel_t channel)
{
	if(channel > Channel_None)
	{
		gpio_write(Config.S0.Gpio,Config.S0.Pin,Value_Pin[Channel_None].Pin_S0);
		gpio_write(Config.S1.Gpio,Config.S1.Pin,Value_Pin[Channel_None].Pin_S1);
		gpio_write(Config.S2.Gpio,Config.S2.Pin,Value_Pin[Channel_None].Pin_S2);
		gpio_write(Config.S3.Gpio,Config.S3.Pin,Value_Pin[Channel_None].Pin_S3);
		gpio_write(Config.EN.Gpio,Config.EN.Pin,Value_Pin[Channel_None].Pin_EN);
		return false;
	}
	else
	{
		gpio_write(Config.S0.Gpio,Config.S0.Pin,Value_Pin[channel].Pin_S0);
		gpio_write(Config.S1.Gpio,Config.S1.Pin,Value_Pin[channel].Pin_S1);
		gpio_write(Config.S2.Gpio,Config.S2.Pin,Value_Pin[channel].Pin_S2);
		gpio_write(Config.S3.Gpio,Config.S3.Pin,Value_Pin[channel].Pin_S3);
		gpio_write(Config.EN.Gpio,Config.EN.Pin,Value_Pin[channel].Pin_EN);
		return true;
	}
}


