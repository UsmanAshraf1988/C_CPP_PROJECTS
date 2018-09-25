/**
 @file adc.cpp
 @brief ADC driver for STM3240G-Eval
 @author: Dr. Klaus Schaefer
 */

#include <adc.h>

#include "stm32f4xx_hal.h"
#include "stm324xg_eval.h"
#include "common.h"
#include "task_builder.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "stm324xg_eval_lcd.h"

#define ADCx                            ADC3
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE();
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_9
#define ADCx_CHANNEL_GPIO_PORT          GPIOF

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_7

/* Definition for ADCx's NVIC */
#define ADCx_IRQn                      	ADC_IRQn
extern COMMON SemaphoreHandle_t xMutex_lcd;
COMMON static ADC_HandleTypeDef AdcHandle;
COMMON static volatile uint16_t adc_value;
//COMMON static SemaphoreHandle_t xMutex_lcd;
adc::adc(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* ADC3 Periph clock enable */
	ADCx_CLK_ENABLE()
	;
	/* Enable GPIO clock ****************************************/
	ADCx_CHANNEL_GPIO_CLK_ENABLE()
	;

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* ADC3 Channel8 GPIO pin configuration */
	GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

	/*##-3- Configure the NVIC #################################################*/
	/* NVIC configuration for DMA transfer complete interrupt (USART1_TX) */
	HAL_NVIC_SetPriority(ADCx_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADCx_IRQn);
	ADC_ChannelConfTypeDef sConfig;

	/*##-1- Configure the ADC peripheral #######################################*/
	AdcHandle.Instance = ADCx;

	AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
	AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
	AdcHandle.Init.ScanConvMode = DISABLE;
	AdcHandle.Init.ContinuousConvMode = ENABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion = 0;
	AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.NbrOfConversion = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection = DISABLE;

	if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
		asm("bkpt 0");

	/*##-2- Configure ADC regular channel ######################################*/
	sConfig.Channel = ADCx_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.Offset = 0;

	if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
		asm("bkpt 0");

	/*##-3- Start the conversion process and enable interrupt ##################*/
	if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
		asm("bkpt 0");
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) {
	ASSERT(0); // this is a DUMMY that should not be called !
}
extern "C" void HAL_ADCEx_InjectedConvCpltCallback(
		ADC_HandleTypeDef* AdcHandle) {
	ASSERT(0); // this is a DUMMY that should not be called !
}

/** @brief ADC task procedure */
void adc_task(void *) {

	adc the_adc;
	char buffer[10] = { 0 };

	portSWITCH_TO_USER_MODE();
	// now go protected

	while (true) {

		if ( xSemaphoreTake( xMutex_lcd, ( portTickType ) 10 ) == pdTRUE) {
			uint16_t value = adc_value;
			for (uint8_t i = 4; i < 0xff; --i) {
				buffer[i] = (uint8_t) (value % 10) + '0';
				value /= 10;
			}
			BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
			BSP_LCD_DisplayStringAtLine(LINE(10), (uint8_t *) buffer);

			xSemaphoreGive(xMutex_lcd);
			vTaskDelay(50);
		}

	}
}

/** @brief ADC conversion end interrupt */
extern "C" void ADC_IRQHandler(void) {
	adc_value = AdcHandle.Instance->DR;
	/* Clear the ADCx flag for regular end of conversion */
	__HAL_ADC_CLEAR_FLAG(&AdcHandle, ADC_FLAG_EOC);
}

static ROM TaskParameters_t task_parameter = { adc_task,// code to be executed
		"ADC",			// name
		128, 			// stack size
		(void *) 0,		 	// pointer to the data being passed to the code
		portPRIVILEGE_BIT, 		// priority
		0, {
		// memory sections:
		// Base-address Length Parameters
				{ COMMON_BLOCK, COMMON_SIZE, portMPU_REGION_READ_WRITE }, {
						(void *) 0x60000000, 0x10000000,
						portMPU_REGION_READ_WRITE }, { 0, 0, 0 } } };

/** @brief task start helper */
void start_adc_task(uint8_t priority) {
	TaskParameters_t p = task_parameter;
	p.uxPriority |= priority;
	task_t task(p);
}
