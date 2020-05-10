/*
 * lcd16x2.c
 *
 *  Created on: 03/05/2020
 *      Author: Evandro Teixeira
 */

#include "lcd16x2.h"

/**
 *
 */
typedef struct
{
	GPIO_MemMapPtr port;
	uint32_t pin;
}lcd_pins_t;


/**
 *
 */
typedef union
{
	uint8_t byte;
	struct
	{
		unsigned bit0 : 1;
		unsigned bit1 : 1;
		unsigned bit2 : 1;
		unsigned bit3 : 1;
		unsigned bit4 : 1;
		unsigned bit5 : 1;
		unsigned bit6 : 1;
		unsigned bit7 : 1;
	};
}lcd_data_t;


/**
 *
 */
typedef enum
{
	lcd_rs = 0,
	lcd_rw,
	lcd_en,
	lcd_d4,
	lcd_d5,
	lcd_d6,
	lcd_d7,
}lcd_index_t;

/**
 * @brief
 */
static void lcd16x2_nibble_send(uint8_t dt);
static void lcd16x2_delay(uint32_t t);
static void lcd16x2_byte_send(uint8_t en,uint8_t data);
//static void lcd16x2_xy(uint8_t x, uint8_t y);
//static void lcd16x2_write_char(char c);

/**
 * @brief
 */
volatile lcd_pins_t lcd_pins[LCD16X2_NUMBER_PIN] = {0};

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
					GPIO_MemMapPtr gpio_d7,	uint32_t pin_d7)	// Pin D7
{
	uint8_t i = 0;

	// Copia
	lcd_pins[lcd_rs].port = gpio_rs;
	lcd_pins[lcd_rs].pin  = pin_rs;

	lcd_pins[lcd_rw].port = gpio_rw;
	lcd_pins[lcd_rw].pin  = pin_rw;

	lcd_pins[lcd_en].port = gpio_en;
	lcd_pins[lcd_en].pin  = pin_en;

	lcd_pins[lcd_d4].port = gpio_d4;
	lcd_pins[lcd_d4].pin  = pin_d4;

	lcd_pins[lcd_d5].port = gpio_d5;
	lcd_pins[lcd_d5].pin  = pin_d5;

	lcd_pins[lcd_d6].port = gpio_d6;
	lcd_pins[lcd_d6].pin  = pin_d6;

	lcd_pins[lcd_d7].port = gpio_d7;
	lcd_pins[lcd_d7].pin  = pin_d7;

	for(i=0;i<LCD16X2_NUMBER_PIN;i++)
	{
		gpio_init(lcd_pins[i].port ,lcd_pins[lcd_d7].pin,OUTPUT);
	}
}

/**
 * @brief
 * @param t
 */
static void lcd16x2_delay(uint32_t t)
{
	while(t--){}
}

/**
 * @brief
 * @param dt
 */
static void lcd16x2_nibble_send(uint8_t dt)
{
	lcd_data_t data;
	data.byte = dt;

	// Data D4
	gpio_write(lcd_pins[lcd_d4].port, lcd_pins[lcd_d4].pin,data.bit0);
	// Data D5
	gpio_write(lcd_pins[lcd_d5].port, lcd_pins[lcd_d5].pin,data.bit1);
	// Data D6
	gpio_write(lcd_pins[lcd_d6].port, lcd_pins[lcd_d6].pin,data.bit2);
	// Data D7
	gpio_write(lcd_pins[lcd_d7].port, lcd_pins[lcd_d7].pin,data.bit3);

	// Data EN
	gpio_write(lcd_pins[lcd_en].port, lcd_pins[lcd_en].pin, 1 );
	lcd16x2_delay(50);
	gpio_write(lcd_pins[lcd_en].port, lcd_pins[lcd_en].pin, 0 );
}

/**
 * @brief
 * @param en
 * @param data
 */
static void lcd16x2_byte_send(uint8_t en,uint8_t data)
{
	// Data RW
	gpio_write(lcd_pins[lcd_rw].port, lcd_pins[lcd_rw].pin, 0 );
	// configura a linha rs dependendo do modo selecionado:
	// 0 -> registrador de comando
	// 1 -> registrador de dados
	// Data RS
	gpio_write(lcd_pins[lcd_rs].port, lcd_pins[lcd_rs].pin, (en==0)?0:1 );
	lcd16x2_delay(50);
	// Data EN
	gpio_write(lcd_pins[lcd_en].port, lcd_pins[lcd_en].pin, 0 );
	// Envia LSB
	lcd16x2_nibble_send(data >> 4);
	// Envia MSB
	lcd16x2_nibble_send(data & 0x0F);
	lcd16x2_delay(50);
}

/**
 * @brief
 * @param x
 * @param y
 */
void lcd16x2_xy(uint8_t x, uint8_t y)
{
	uint8_t end = 0;

	if(y != 1)
	{
		end = 0xC0;
	}

	end += (x-1);
	end |= 0x80;

	lcd16x2_byte_send(0,end);
	lcd16x2_delay(5);
}

/**
 * @brief
 * @param c
 */
void lcd16x2_write_char(char c)
{
	switch(c)
	{
		case '\f':
			lcd16x2_byte_send(0x00,0x01);
			lcd16x2_delay(750);
		break;
		case '\n':
		case '\r':
			lcd16x2_byte_send(0x01,0x02);
		break;
		default:
			lcd16x2_byte_send(0x01,c);
			lcd16x2_delay(5);
	}
}

/**
 * @brief
 * @param line
 * @param *c
 */
void lcd16x2_write_string(uint8_t line,char *c)
{
	if(line == 0)
	{
		lcd16x2_xy(0,1);
	}
	else
	{
		lcd16x2_xy(0,2);
	}

	while(*c)
	{
		lcd16x2_write_char(*c);
		c++;
	}
}
