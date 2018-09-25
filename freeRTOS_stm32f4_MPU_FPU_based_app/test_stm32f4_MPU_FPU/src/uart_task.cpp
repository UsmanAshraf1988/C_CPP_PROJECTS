/*
 * uart.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: schaefer
 */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "common.h"

#include "FreeRTOS.h"
#include "task.h"

#include "task_builder.h"

ROM char message[] = "hello via UART\r\n";

/** @brief  demo task demonstrating USART usage */
void
uart_task (void *)
{
  uint8_t buffer[2] = { 0 };
  uart uart3;

  uart3.puts (message);

  portSWITCH_TO_USER_MODE(); // go protected now

  while (true)
    {
      uart3.wait_4_character ();
      buffer[0] = uart3.receive ();
      uart3.puts ((const char *) buffer);
    }
}

#define STACK_SIZE 128
static portSTACK_TYPE CCM_DATA stack[STACK_SIZE] __attribute__((aligned(STACK_SIZE*sizeof(portSTACK_TYPE))));

static ROM TaskParameters_t task_parameter =
  { uart_task, 	// code to be executed
      "uart",		// name
      STACK_SIZE,
      (void *) 0, 	 // pointer to the data being passed to the code
      portPRIVILEGE_BIT, // priority
      stack,
	{
	// memory sections:
	// Base-address Length Parameters
	      { COMMON_BLOCK, COMMON_SIZE, portMPU_REGION_READ_WRITE },
	      { 0, 0, 0 },
	      { 0, 0, 0 }
	}
  };

void start_uart_task (uint8_t priority)
{
  TaskParameters_t p = task_parameter;
  p.uxPriority |= priority;
  task_t task (p);
}
