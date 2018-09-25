/**
 * @file        lcd.cpp
 * @brief       simple LCD driver module
 */

#include "stm324xg_eval_lcd.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "common.h"
#include "task_builder.h"
#include "semphr.h"
//COMMON static SemaphoreHandle_t xMutex_lcd;
extern COMMON SemaphoreHandle_t xMutex_lcd;
volatile uint16_t TextColor, BackColor; // globals needed by LCD driver

/** @brief LCD initialization
 *
 * needs to be once called before theLCD screen can be used
 *  */
void LCD_init(void) {
	/* Initialize the LCD */
	BSP_LCD_Init();

	/* Clear the LCD */
	BSP_LCD_Clear(LCD_COLOR_YELLOW);

	/* Set the LCD Back Color */
	BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);

	/* Set the LCD Text Color */
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
}

/** @brief write a single line of text on the LCD */
void lcd_write_line(unsigned line, char * text) {
	BSP_LCD_DisplayStringAtLine(LINE(line), (uint8_t *) text);
}

static ROM char hello[] = "Hello FreeRTOS";

/** @brief task start helper */
void bargraph_task(void *) {

	lcd_write_line(0, (char *) hello);

	while (true) {




			for (uint16_t width = 0; width < 320; ++width) {
				if ( xSemaphoreTake( xMutex_lcd, ( portTickType ) 10 ) == pdTRUE) {
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_DrawFullRect(0, 50, width, 10);
				BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
				BSP_LCD_DrawFullRect(width, 50, 320 - width - 1, 10);
				//xSemaphoreGive(xMutex_lcd);
				xSemaphoreGive(xMutex_lcd);
						}
				vTaskDelay(1);
			}



			for (uint16_t width = 319; width > 0; --width) {
				if ( xSemaphoreTake( xMutex_lcd, ( portTickType ) 10 ) == pdTRUE) {
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_DrawFullRect(0, 50, width, 10);
				BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
				BSP_LCD_DrawFullRect(width, 50, 320 - width - 1, 10);
				//xSemaphoreGive(xMutex_lcd);

				xSemaphoreGive(xMutex_lcd);
				vTaskDelay(1);

						}
			}

		vTaskDelay(50);

	} // while
}

static ROM TaskParameters_t task_parameter = { bargraph_task, // code to be executed
		"bargraph",	// name
		512, (void *) 0, 	// pointer to the data being passed to the code
		0, 		// priority
		(uint32_t*) 0, {
		// memory sections:
		// Base-address Length Parameters
				{ COMMON_BLOCK, COMMON_SIZE, portMPU_REGION_READ_WRITE }, {
						(void *) 0x60000000, 0x10000000,
						portMPU_REGION_READ_WRITE }, { 0, 0, 0 } } };

void start_bargraph_task(uint8_t priority) {
	TaskParameters_t p = task_parameter;
	p.uxPriority |= priority;
	task_t task(p);
}
