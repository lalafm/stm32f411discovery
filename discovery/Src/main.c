#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"
#include "led.h"
#include "dma.h"
#include "lsm303dlhc.h"

static void uart_callback(uint32_t key);
static void dma_callback(void);
static void adc_callback(uint32_t sensor_value);
static void sysTick_callback(void);

int main(void)
{

	green_led_init();
	lsm303_init();
    /* Loop forever */
	while(1)
	{
		lsm303_read();
	}
	return 0;
}

static void uart_callback(uint32_t key)
{
	if(key == '1')
	{
		green_led_set();
	}
	else
	{
		green_led_clear();
	}
}

static void dma_callback(void)
{
	green_led_toggle();
}

static void adc_callback(uint32_t sensor_value)
{
	printf("Sensor value: %d \n\r", (int)sensor_value);
}

static void sysTick_callback(void)
{
	printf("A second passed... \n\r");
	green_led_toggle();
}
