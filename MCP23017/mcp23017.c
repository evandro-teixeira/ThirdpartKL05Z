/*
 * mcp23017.c
 *
 *  Created on: 18/08/2019
 *      Author: Evandro Teixeira
 */
#include "../MCP23017/mcp23017.h"

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
static pin_t reset, interrupt_a,interrupt_b;
static uint8_t address;
static I2C_MemMapPtr i2cptr;
const static uint8_t iodir[MCP23017_NUMBER_PORT] = 		MCP23017_IODIR;
const static uint8_t ipol[MCP23017_NUMBER_PORT] = 		MCP23017_IPOL;
const static uint8_t gpinten[MCP23017_NUMBER_PORT] = 	MCP23017_GPINTEN;
const static uint8_t defval[MCP23017_NUMBER_PORT] = 	MCP23017_DEFVAL;
const static uint8_t intcon[MCP23017_NUMBER_PORT] = 	MCP23017_INTCON;
const static uint8_t iocon[MCP23017_NUMBER_PORT] = 		MCP23017_IOCON;
const static uint8_t gppu[MCP23017_NUMBER_PORT] = 		MCP23017_GPPU;
const static uint8_t intf[MCP23017_NUMBER_PORT] = 		MCP23017_INTF;
const static uint8_t intcap[MCP23017_NUMBER_PORT] =	 	MCP23017_INTCAP;
const static uint8_t gpio[MCP23017_NUMBER_PORT] = 		MCP23017_GPIO;
const static uint8_t olat[MCP23017_NUMBER_PORT] = 		MCP23017_OLAT;

/**
 *
 */
static void mcp23017_interrupt(void);
static void mcp23017_reset(bool value);
static void mcp23017_write_register(uint8_t reg, uint8_t data);
static uint8_t mcp23017_read_register(uint8_t reg);

/**
 *
 */
static void mcp23017_interrupt(void)
{

} 

/**
 *
 */
static void mcp23017_reset(bool value)
{
	gpio_write(reset.io,reset.pin,value);
}

/**
 *
 */
static void mcp23017_write_register(uint8_t reg, uint8_t data)
{
	i2c_send_data(i2cptr,address,reg,data);
}

/**
 *
 */
static uint8_t mcp23017_read_register(uint8_t reg)
{
	return i2c_read_data(i2cptr,address,reg);
} 

/**
 *
 */ 
void mcp23017_init(I2C_MemMapPtr i2c,uint8_t add, GPIO_MemMapPtr gpio_reset,uint32_t pin_reset,/* PIN RESET 	*/
				GPIO_MemMapPtr gpio_intA,uint32_t pin_intA, /* PIN INTERRUPT A'*/
				GPIO_MemMapPtr gpio_intB,uint32_t pin_intB /* PIN INTERRUPT B'*/)
{
	/* */
	i2cptr = i2c;

	/* */
	address = add;
	
	/* */
	reset.io = gpio_reset;
	reset.pin = pin_reset;
	
	/* */
	interrupt_a.io = gpio_intA;
	interrupt_a.pin = pin_intA;
	
	/* */
	interrupt_b.io = gpio_intB;
	interrupt_b.pin = pin_intB;
	
	/* */
	gpio_init(reset.io,reset.pin,OUTPUT);
	
	/* */
	gpio_init(interrupt_a.io,interrupt_a.pin,INPUT);
	gpio_interrupt_configuration(interrupt_a.io,interrupt_a.pin,Interrupt_on_Falling_Edge);
	gpio_set_callback_irq(interrupt_a.io,mcp23017_interrupt);
	
	/* */
	mcp23017_reset(MCP23017_ENABLE);
}

/**
 *
 */
void mcp23017_pin_dir(mcp23017_port_t port, mcp23017_pin_t pin, mcp23017_dir_t dir)
{
	uint8_t reg = 0;
	
	reg = mcp23017_read_register(iodir[port]);
	if(dir == MCP23017_INPUT)
	{
		mcp23017_write_register(iodir[port], reg | pin);
	}
	else if(dir == MCP23017_OUTPUT)
	{
		mcp23017_write_register(iodir[port], reg &~ pin);
	}
	else
	{
	}
} 

/**
 *
 */
void mcp23017_pull_set(mcp23017_port_t port,uint8_t value)
{
	mcp23017_write_register(gppu[port],value);
}

/**
 *
 */
uint8_t mcp23017_pull_get(mcp23017_port_t port)
{
	return mcp23017_read_register(gppu[port]);
}

/**
 *
 */
void mcp23017_input_polarity_set(mcp23017_port_t port,uint8_t value)
{
	mcp23017_write_register(ipol[port],value);
}

/**
 *
 */
uint8_t mcp23017_input_polarity_get(mcp23017_port_t port)
{
	return mcp23017_read_register(ipol[port]);
}

/**
 *
 */
void mcp23017_outputs_write(mcp23017_port_t port,uint8_t data)
{
	mcp23017_write_register(gpio[port],data);
}

/**
 *
 */
uint8_t mcp23017_inputs_read(mcp23017_port_t port)
{
	return mcp23017_read_register(gpio[port]);
}

/**
 *
 */
void mcp23017_interrupt_on_changes(mcp23017_port_t port,mcp23017_pin_t pin)
{
	uint8_t valeu = mcp23017_read_register(iocon[port]);
	valeu &= ~pin;
	mcp23017_write_register(iocon[port],valeu);
	valeu = mcp23017_read_register(gpinten[port]);
	valeu |= pin;
	mcp23017_write_register(gpinten[port],valeu);
}

/**
 *
 */
void mcp23017_disable_interrupts(mcp23017_port_t port,mcp23017_pin_t pin)
{
	uint8_t valeu = mcp23017_read_register(gpinten[port]);
	valeu |= pin;
	mcp23017_write_register(gpinten[port],valeu);
}


/**
 *
 */
void mcp23017_acknowledge_interrupt(mcp23017_port_t port, uint8_t *pin, uint8_t *value)
{
	*pin = mcp23017_read_register(intf[port]);
	*value = mcp23017_read_register(intcap[port]);
}

/**
 *
 */
void mcp23017_comparison_value(mcp23017_port_t port, mcp23017_pin_t pin,mcp23017_value_t value)
{
	uint8_t val = mcp23017_read_register(defval[port]);
	if(value == MCP23017_HIGH)
	{
		val |= pin;
	}
	else if(value == MCP23017_LOW)
	{
		val &= ~pin;
	}
	else
	{
	}
	mcp23017_write_register(defval[port],val);
}

/**
 *
 */
void mcp23017_interrupt_pin_set(mcp23017_port_t port, mcp23017_value_t pin,mcp23017_irq_t irq)
{
	uint8_t val = mcp23017_read_register(intcon[port]);
	val |= pin;
	mcp23017_write_register(intcon[port],val);

	val = mcp23017_read_register(defval[port]);
	val |= pin;
	mcp23017_write_register(defval[port],val);

	val = mcp23017_read_register(gpinten[port]);
	val |= pin;
	mcp23017_write_register(gpinten[port],val);
}

/**
 *
 */
uint8_t mcp23017_interrupt_value_get(mcp23017_port_t port)
{
	return mcp23017_read_register(intcap[port]);
}




