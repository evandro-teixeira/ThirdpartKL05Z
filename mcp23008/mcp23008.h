/*
 * mcp23008.h
 *
 *  Created on: 18/08/2019
 *      Author: Evandro Teixeira
 */

#ifndef THIRDPARTKL05Z_MCP23008_MCP23008_H_
#define THIRDPARTKL05Z_MCP23008_MCP23008_H_

#include "../KL05Z_Libraries/board_frdm_kl05z.h"

#define MCP23008_ENABLE		LOW
#define MCP23008_DISABLE	HIGH
/* REGISTER ADDRESSES */
#define MCP23008_IODIR 		0x00	// 00h IODIR
#define MCP23008_IPOL 		0x01	// 01h IPOL
#define MCP23008_GPINTEN 	0x02	// 02h GPINTEN
#define MCP23008_DEFVAL		0x03	// 03h DEFVAL
#define MCP23008_INTCON 	0x04	// 04h INTCON
#define MCP23008_IOCON 		0x05	// 05h IOCON
#define MCP23008_GPPU 		0x06	// 06h GPPU
#define MCP23008_INTF 		0x07	// 07h INTF
#define MCP23008_INTCAP 	0x08	// 08h INTCAP (Read-only)
#define MCP23008_GPIO 		0x09	// 09h GPIO
#define MCP23008_OLAT 		0x0A	// 0Ah OLAT

typedef enum
{
	MCP23008_INPUT = 0,
	MCP23008_OUTPUT,
}mcp23008_dir_t;

typedef enum
{
	MCP23008_LOW = 0,
	MCP23008_HIGH,
}mcp23008_value_t;

typedef enum
{
	MCP23008_INT_HIGH = 0, // Interrupt Active-High
	MCP23008_INT_LOW,	// Interrupt Active-Low
}mcp23008_irq_t;

typedef enum
{
	MCP23008_PIN0 = 0x01,
	MCP23008_PIN1 = 0x02,
	MCP23008_PIN2 = 0x04,
	MCP23008_PIN3 = 0x08,
	MCP23008_PIN4 = 0x10,
	MCP23008_PIN5 = 0x20,
	MCP23008_PIN6 = 0x40,
	MCP23008_PIN7 = 0x80,
	MCP23008_PIN_ALL = 0xFF
}mcp23008_pin_t;

void mcp23008_init(I2C_MemMapPtr i2c,uint8_t add,	GPIO_MemMapPtr gpio_reset,uint32_t pin_reset, /* PIN RESET */
				GPIO_MemMapPtr gpio_int,uint32_t pin_int /* PIN INTERRUPT */);
void mcp23008_pin_dir(mcp23008_pin_t pin, mcp23008_dir_t dir);
void mcp23008_pull_set(uint8_t value);
uint8_t mcp23008_pull_get(void);
void mcp23008_input_polarity_set(uint8_t value);
uint8_t mcp23008_input_polarity_get(void);
void mcp23008_outputs_write(uint8_t data);
uint8_t mcp23008_inputs_read(void);
void mcp23008_interrupt_on_changes(mcp23008_pin_t pin); 
void mcp23008_disable_interrupts(mcp23008_pin_t pin); 
void mcp23008_acknowledge_interrupt(uint8_t *pin, uint8_t *value);
void mcp23008_comparison_value(mcp23008_pin_t pin,mcp23008_value_t value);
void mcp23008_interrupt_pin_set(mcp23008_value_t pin,mcp23008_irq_t irq);
uint8_t mcp23008_interrupt_value_get(void);
																
#endif /* THIRDPARTKL05Z_MCP23008_MCP23008_H_ */
