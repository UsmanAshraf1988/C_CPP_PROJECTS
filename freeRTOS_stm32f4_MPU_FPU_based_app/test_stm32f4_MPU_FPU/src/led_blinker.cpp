#include "stm324xg_eval.h"
#include "stm324xg_eval_io.h"

#include "FreeRTOS.h"
#include "task.h"

#include "common.h"
#include "task_builder.h"

struct job_data_t
  {
    Led_TypeDef Led;
    uint32_t On_time;
    uint32_t Off_time;
  };

static ROM job_data_t job_data = { LED1, 111, 333};

void led_blinker(void *parameters);
void led_blinker(void *parameters)
{
  job_data_t *p = (job_data_t *)parameters;
  BSP_LED_Init (p->Led);
  while (true)
    {
      BSP_LED_Toggle(p->Led);
      vTaskDelay (p->On_time);
      BSP_LED_Toggle(p->Led);
      vTaskDelay (p->Off_time);
    }
}

static ROM TaskParameters_t task_parameter =
  {
      led_blinker, 		// code to be executed
      "LED",			// name
      128,			// stack size in 32bit words
      (void *) &job_data, 	// pointer to the data being passed to the code
      0, 			// priority
      (uint32_t*)0,		// dummy stack buffer: will be allocated dynamically
	{
	// memory sections:
	// Base-address Length Parameters
	  { 0, 0, 0 },
	  { 0, 0, 0 },
	  { 0, 0, 0 }
	}
  };

/** @brief task start helper */
void start_led_task (uint8_t priority)
{
  TaskParameters_t p = task_parameter;
  p.uxPriority |= priority;
  task_t led_task ( p);
}

