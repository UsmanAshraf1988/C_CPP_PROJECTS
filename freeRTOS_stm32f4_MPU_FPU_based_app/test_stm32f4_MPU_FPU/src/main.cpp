/**
 * @file    main.cpp
 * @brief   system start code
 * @author  Dr. Klaus Schaefer schaefer@eit.h-da.de
 * @mainpage
 * Sample Projekt for the STM3240G-Eval Evaluation-Board
 */

#include "common.h"
#include "task_builder.h"
#include "semphr.h"

void init_system(void);
void LCD_init(void);

#define DEFAULT_PRIORITY 2

void start_button_task(uint8_t priority = DEFAULT_PRIORITY);
void start_bargraph_task(uint8_t priority = DEFAULT_PRIORITY);
void start_uart_task(uint8_t priority = DEFAULT_PRIORITY);
void start_led_task(uint8_t priority = DEFAULT_PRIORITY);
void start_adc_task(uint8_t priority = DEFAULT_PRIORITY);

COMMON SemaphoreHandle_t xMutex_lcd = NULL;
/** @brief main function, entry point of the application */
void main(void) {

	//xMutex_lcd = xSemaphoreCreateMutex();

	//xSemaphoreGive(xMutex_lcd);
	init_system();
	HAL_Init();
	LCD_init();
	xMutex_lcd = xSemaphoreCreateMutex();


	start_button_task();
	start_bargraph_task();
	start_uart_task();
	start_led_task();

	start_adc_task();

	vTaskStartScheduler(); // ATTENTION: this call destroys the stack of main() including all auto variables
}
