/*
 * mcp23017.h
 *
 *  Created on: 18/08/2019
 *      Author: Evandro Teixeira
 */

#ifndef THIRDPARTKL05Z_MCP23017_MCP23017_H_
#define THIRDPARTKL05Z_MCP23017_MCP23017_H_


#include "../KL05Z_Libraries/board_frdm_kl05z.h"

#define MCP23017_ENABLE		LOW
#define MCP23017_DISABLE	HIGH
/* REGISTER ADDRESSES PORTA */
#define MCP23017_IODIRA 	0x00
#define MCP23017_IPOLA 		0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 	0x06
#define MCP23017_INTCONA 	0x08
#define MCP23017_IOCONA 	0x0A
#define MCP23017_GPPUA 		0x0C
#define MCP23017_INTFA 		0x0E
#define MCP23017_INTCAPA 	0x10
#define MCP23017_GPIOA 		0x12
#define MCP23017_OLATA 		0x14
/* REGISTER ADDRESSES PORTB */
#define MCP23017_IODIRB 	0x01
#define MCP23017_IPOLB 		0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 	0x07
#define MCP23017_INTCONB 	0x09
#define MCP23017_IOCONB 	0x0B
#define MCP23017_GPPUB 		0x0D
#define MCP23017_INTFB 		0x0F
#define MCP23017_INTCAPB 	0x11
#define MCP23017_GPIOB 		0x13
#define MCP23017_OLATB 		0x15

#define MCP23017_INT_ERR 	0xFF

#define MCP23017_IODIR		\
{							\
		/* PORTA */			\
		MCP23017_IODIRA,	\
		/* PORTB */			\
		MCP23017_IODIRB		\
}

#define MCP23017_IPOL 		\
{							\
		/* PORTA */			\
		MCP23017_IPOLA,		\
		/* PORTB */			\
		MCP23017_IPOLB		\
}

#define MCP23017_GPINTEN 	\
{							\
		/* PORTA */			\
		MCP23017_GPINTENA,	\
		/* PORTB */			\
		MCP23017_GPINTENB	\
}

#define MCP23017_DEFVAL		\
{							\
		/* PORTA */			\
		MCP23017_DEFVALA,	\
		/* PORTB */			\
		MCP23017_DEFVALB	\
}

#define MCP23017_INTCON		\
{							\
		/* PORTA */			\
		MCP23017_INTCONA,	\
		/* PORTB */			\
		MCP23017_INTCONB	\
}

#define MCP23017_IOCON		\
{							\
		/* PORTA */			\
		MCP23017_IOCONA,	\
		/* PORTB */			\
		MCP23017_IOCONB		\
}

#define MCP23017_GPPU		\
{							\
		/* PORTA */			\
		MCP23017_GPPUA,		\
		/* PORTB */			\
		MCP23017_GPPUB		\
}

#define MCP23017_INTF		\
{							\
		/* PORTA */			\
		MCP23017_INTFA,		\
		/* PORTB */			\
		MCP23017_INTFB		\
}

#define MCP23017_INTCAP		\
{							\
		/* PORTA */			\
		MCP23017_INTCAPA,	\
		/* PORTB */			\
		MCP23017_INTCAPB	\
}

#define MCP23017_GPIO		\
{							\
		/* PORTA */			\
		MCP23017_GPIOA,		\
		/* PORTB */			\
		MCP23017_GPIOB		\
}

#define MCP23017_OLAT		\
{							\
		/* PORTA */			\
		MCP23017_OLATA,		\
		/* PORTB */			\
		MCP23017_OLATB		\
}

typedef enum
{
	MCP23017_PORTA = 0,
	MCP23017_PORTB,
	MCP23017_NUMBER_PORT,
}mcp23017_port_t;

typedef enum
{
	MCP23017_INPUT = 0,
	MCP23017_OUTPUT,
}mcp23017_dir_t;

typedef enum
{
	MCP23017_LOW = 0,
	MCP23017_HIGH,
}mcp23017_value_t;

typedef enum
{
	MCP23017_INT_HIGH = 0, // Interrupt Active-High
	MCP23017_INT_LOW, // Interrupt Active-Low
}mcp23017_irq_t;

typedef enum
{
	MCP23017_PIN0 = 0x01,
	MCP23017_PIN1 = 0x02,
	MCP23017_PIN2 = 0x04,
	MCP23017_PIN3 = 0x08,
	MCP23017_PIN4 = 0x10,
	MCP23017_PIN5 = 0x20,
	MCP23017_PIN6 = 0x40,
	MCP23017_PIN7 = 0x80,
	MCP23017_PIN_ALL = 0xFF
}mcp23017_pin_t;

void mcp23017_init(I2C_MemMapPtr i2c,uint8_t add, GPIO_MemMapPtr gpio_reset,uint32_t pin_reset, /* PIN RESET 	*/
				GPIO_MemMapPtr gpio_intA,uint32_t pin_intA, /* PIN INTERRUPT A'*/
				GPIO_MemMapPtr gpio_intB,uint32_t pin_intB /* PIN INTERRUPT B'*/);

void mcp23017_pin_dir(mcp23017_port_t port, mcp23017_pin_t pin, mcp23017_dir_t dir);
void mcp23017_pull_set(mcp23017_port_t port,uint8_t value);
uint8_t mcp23017_pull_get(mcp23017_port_t port);
void mcp23017_input_polarity_set(mcp23017_port_t port,uint8_t value);
uint8_t mcp23017_input_polarity_get(mcp23017_port_t port);
void mcp23017_outputs_write(mcp23017_port_t port,uint8_t data);
uint8_t mcp23017_inputs_read(mcp23017_port_t port);
void mcp23017_interrupt_on_changes(mcp23017_port_t port,mcp23017_pin_t pin);
void mcp23017_disable_interrupts(mcp23017_port_t port,mcp23017_pin_t pin);
void mcp23017_acknowledge_interrupt(mcp23017_port_t port, uint8_t *pin, uint8_t *value);
void mcp23017_comparison_value(mcp23017_port_t port, mcp23017_pin_t pin,mcp23017_value_t value);
void mcp23017_interrupt_pin_set(mcp23017_port_t port, mcp23017_value_t pin,mcp23017_irq_t irq);
uint8_t mcp23017_interrupt_value_get(mcp23017_port_t port);


#endif /* THIRDPARTKL05Z_MCP23017_MCP23017_H_ */
