/*
 * hcsr04.c
 *
 *  Created on: 12/06/2021
 *      Author: Evandro Teixeira
 */
#include "hcsr04.h"

/* Macro to convert a microsecond period to raw count value */
#define USEC_TO_COUNT(us, clockFreqInHz)	(uint32_t)((uint64_t)us * clockFreqInHz / 1000000U)
#define ENABLE_DEBUG						0
#define TIMEOUT								5000
#define IRQ_PERIOD							10  /* interruption period */
#define PULSE_WIDTH							10  /* 10 microsecond pulse width */

/**
 *
 */
typedef struct
{
	GPIO_MemMapPtr gpio;
	uint32_t pin;
}hcsr04_pin_t;

typedef struct
{
	hcsr04_pin_t Trig;
	hcsr04_pin_t Echo;
}hcsr04_config_t;

static hcsr04_config_t hcsr04_pin;
static uint32_t hcsr04_counter = 0;
void hcsr04_IrqTimer(void);
void hcsr04_ResetTimer(void);
uint32_t hcsr04_GetTimer(void);
void hcsr04_Delay(uint32_t t);
void hcsr04_IrqEcho(void);

/**
 *
 */
//bool hcsr04_Init(hcsr04_config_t pin)
void hcsr04_Init(GPIO_MemMapPtr gpio_Trig, uint32_t pin_Trig,
				GPIO_MemMapPtr gpio_Echo, uint32_t pin_Echo)
{
	hcsr04_pin.Echo.gpio = gpio_Echo;
	hcsr04_pin.Echo.pin = pin_Echo;
	hcsr04_pin.Trig.gpio = gpio_Trig;
	hcsr04_pin.Trig.pin = pin_Trig;

	/* Configure Trig pin as output */
	gpio_init(hcsr04_pin.Trig.gpio,hcsr04_pin.Trig.pin,GPIO_OUTPUT);
	gpio_write(hcsr04_pin.Trig.gpio,hcsr04_pin.Trig.pin, LOW);
	/* Configure Echo pin as input */
	gpio_init(hcsr04_pin.Echo.gpio,hcsr04_pin.Echo.pin,GPIO_INPUT);

	/* Configure the PIT timer */
	pit_init( USEC_TO_COUNT(IRQ_PERIOD,SystemCoreClock), PIT_CHANNEL_0);
	pit_stop( PIT_CHANNEL_0 );
	pit_set_callback_irq(PIT_CHANNEL_0, hcsr04_IrqTimer);
}

/**
 *
 */
bool hcsr04_GetDistance(uint32_t *Distance)
{
	uint8_t state_pin;
	uint32_t distance = 0;
	uint16_t timeout[2] = {0};

	/* Generate trigger pulse */
	gpio_write(hcsr04_pin.Trig.gpio,hcsr04_pin.Trig.pin, HIGH);
	hcsr04_Delay(PULSE_WIDTH);
	gpio_write(hcsr04_pin.Trig.gpio,hcsr04_pin.Trig.pin, LOW);

	/* Wait for rising edge on Echo signal */
	do
	{
		timeout[0]++;
		state_pin = gpio_read(hcsr04_pin.Echo.gpio,hcsr04_pin.Echo.pin);
	}while((state_pin == LOW) && (timeout[0] < TIMEOUT));

	/* start the timer */
	hcsr04_ResetTimer();
	pit_start( PIT_CHANNEL_0 );

	/* Wait for falling edge on Echo signal */
	do
	{
		timeout[1]++;
		state_pin = gpio_read(hcsr04_pin.Echo.gpio,hcsr04_pin.Echo.pin);
	}while((state_pin == HIGH) && (timeout[1] < TIMEOUT));

	/* turn off the timer */
	pit_stop( PIT_CHANNEL_0 );

	/* Check if it is a valid reading */
	if((timeout[0] >= TIMEOUT) || (timeout[1] >= TIMEOUT))
	{
		*Distance = distance;
		return false;
	}
	else
	{
		/* Calculate distance in cm */
		distance = (uint32_t)((float)((hcsr04_GetTimer() * 10) * 0.0343F )); //0.01715F
		*Distance = distance;
		return true;
	}

}

/**
 *
 */
bool hcsr04_GetPulseWidth(uint32_t *Pulse)
{
	uint8_t state_pin;
	uint32_t pulse = 0;
	uint16_t timeout[2] = {0};

	/* Generate trigger pulse */
	gpio_write(hcsr04_pin.Trig.gpio,hcsr04_pin.Trig.pin, HIGH);
	hcsr04_Delay(PULSE_WIDTH);
	gpio_write(hcsr04_pin.Trig.gpio,hcsr04_pin.Trig.pin, LOW);

	/* Wait for rising edge on Echo signal */
	do
	{
		timeout[0]++;
		state_pin = gpio_read(hcsr04_pin.Echo.gpio,hcsr04_pin.Echo.pin);
	}while((state_pin == LOW) && (timeout[0] < TIMEOUT));

	/* start the timer */
	hcsr04_ResetTimer();
	pit_start( PIT_CHANNEL_0 );

	/* Wait for falling edge on Echo signal */
	do
	{
		timeout[1]++;
		state_pin = gpio_read(hcsr04_pin.Echo.gpio,hcsr04_pin.Echo.pin);
	}while((state_pin == HIGH) && (timeout[1] < TIMEOUT));

	/* turn off the timer */
	pit_stop( PIT_CHANNEL_0 );

	/* Check if it is a valid reading */
	if((timeout[0] >= TIMEOUT) || (timeout[1] >= TIMEOUT))
	{
		*Pulse = pulse;
		return false;
	}
	else
	{
		*Pulse = (uint32_t)(hcsr04_GetTimer() * 10);
		return true;
	}
}

/**
 *
 */
void hcsr04_Delay(uint32_t t)
{
	while(t)
	{
		__ASM("NOP");
		t--;
	}
}

/**
 *
 */
void hcsr04_IrqTimer(void)
{

	hcsr04_counter++;
}

/**
 *
 */
uint32_t hcsr04_GetTimer(void)
{
	return hcsr04_counter;
}

/**
 *
 */
void hcsr04_ResetTimer(void)
{
	hcsr04_counter = 0;
}
