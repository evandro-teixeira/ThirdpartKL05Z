/*
 * mcp23008.c
 *
 *  Created on: 18/08/2019
 *      Author: Evandro Teixeira
 */
#include "../MCP23008/mcp23008.h"

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
static pin_t reset, interrupt;
static uint8_t address;
static I2C_MemMapPtr i2cptr;

/**
 *
 */
static void mcp23008_interrupt(void);
static void mcp23008_reset(bool value);
static void mcp23008_write_register(uint8_t reg, uint8_t data);
static uint8_t mcp23008_read_register(uint8_t reg);

/**
 *
 */
static void mcp23008_interrupt(void)
{

} 

/**
 *
 */
static void mcp23008_reset(bool value)
{
	gpio_write(reset.io,reset.pin,(gpio_status_t)value);
} 

/**
 *
 */
static void mcp23008_write_register(uint8_t reg, uint8_t data)
{
	i2c_send_data(i2cptr,address,reg,data);
} 

/**
 *
 */
static uint8_t mcp23008_read_register(uint8_t reg)
{
	return i2c_read_data(i2cptr,address,reg);
} 
 
/**
 *
 */ 
void mcp23008_init(I2C_MemMapPtr i2c,uint8_t add,	GPIO_MemMapPtr gpio_reset,uint32_t pin_reset, /* PIN RESET */
				GPIO_MemMapPtr gpio_int,uint32_t pin_int /* PIN INTERRUPT */)
{
	i2cptr = i2c;

	/* */
	address = add;
	
	/* */
	reset.io = gpio_reset;
	reset.pin = pin_reset;
	
	/* */
	interrupt.io = gpio_int;
	interrupt.pin = pin_int;
	
	/* */
	gpio_init(reset.io,reset.pin,OUTPUT);
	
	/* */
	gpio_init(interrupt.io,interrupt.pin,INPUT);
	gpio_interrupt_configuration(interrupt.io,interrupt.pin,Interrupt_on_Falling_Edge);
	gpio_set_callback_irq(interrupt.io,mcp23008_interrupt);
	
	/* */
	mcp23008_reset(MCP23008_ENABLE);
}

/**
 *
 */
void mcp23008_pin_dir(mcp23008_pin_t pin, mcp23008_dir_t dir)
{
	uint8_t reg = 0;
	reg = mcp23008_read_register(MCP23008_IODIR);
	if(dir == MCP23008_INPUT)
	{
		mcp23008_write_register(MCP23008_IODIR, reg | pin);	
	}
	else if(dir == MCP23008_OUTPUT)
	{
		mcp23008_write_register(MCP23008_IODIR, reg &~ pin);
	}
	else 	
	{
	}
} 

/**
 *
 */
void mcp23008_pull_set(uint8_t value)
{
	mcp23008_write_register(MCP23008_GPPU,value);
} 

/**
 *
 */
uint8_t mcp23008_pull_get(void)
{
	return mcp23008_read_register(MCP23008_GPPU);
} 

/**
 *
 */
void mcp23008_input_polarity_set(uint8_t value)
{
	mcp23008_write_register(MCP23008_IPOL,value); 
} 

/**
 *
 */
uint8_t mcp23008_input_polarity_get(void)
{
	return mcp23008_read_register(MCP23008_IPOL);
} 

/**
 *
 */
void mcp23008_outputs_write(uint8_t data)
{
	mcp23008_write_register(MCP23008_GPIO,data);
}

/**
 *
 */
uint8_t mcp23008_inputs_read(void)
{
	return mcp23008_read_register(MCP23008_GPIO);
}

/**
 *
 */ 
void mcp23008_interrupt_on_changes(mcp23008_pin_t pin) 
{
	uint8_t valeu = mcp23008_read_register(MCP23008_IOCON);
	valeu &= ~pin;
	mcp23008_write_register(MCP23008_IOCON,valeu);
	valeu = mcp23008_read_register(MCP23008_GPINTEN);
	valeu |= pin;
	mcp23008_write_register(MCP23008_GPINTEN,valeu);
}

/**
 *
 */
void mcp23008_disable_interrupts(mcp23008_pin_t pin) 
{
	uint8_t valeu = mcp23008_read_register(MCP23008_GPINTEN);
	valeu |= pin;
	mcp23008_write_register(MCP23008_GPINTEN,valeu);
} 

/**
 *
 */
void mcp23008_acknowledge_interrupt(uint8_t *pin, uint8_t *value)
{
	*pin = mcp23008_read_register(MCP23008_INTF);
	*value = mcp23008_read_register(MCP23008_INTCAP);
} 

/**
 *
 */
void mcp23018_comparison_value(mcp23008_pin_t pin,mcp23008_value_t value)
{
	uint8_t val = mcp23008_read_register(MCP23008_DEFVAL);
	if(value == MCP23008_HIGH)
	{
		val |= pin;
	}
	else if(value == MCP23008_LOW)
	{
		val &= ~pin;
	}
	else
	{
	}
	mcp23008_write_register(MCP23008_DEFVAL,val);
} 

/**
 *
 */
void mcp23008_interrupt_pin_set(mcp23008_value_t pin,mcp23008_irq_t irq)
{
	uint8_t val = mcp23008_read_register(MCP23008_INTCON);
	val |= pin;
	mcp23008_write_register(MCP23008_INTCON,val);
	
	val = mcp23008_read_register(MCP23008_DEFVAL);
	val |= pin;
	mcp23008_write_register(MCP23008_DEFVAL,val);
	
	val = mcp23008_read_register(MCP23008_GPINTEN);
	val |= pin;
	mcp23008_write_register(MCP23008_GPINTEN,val);	
} 

/**
 *
 */
uint8_t mcp23008_interrupt_value_get(void)
{
	return mcp23008_read_register(MCP23008_INTCAP);
}
