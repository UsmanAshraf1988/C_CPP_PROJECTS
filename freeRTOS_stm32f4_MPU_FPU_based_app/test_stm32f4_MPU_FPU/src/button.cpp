/** ***************************************************************************
 * @file    button.cpp
 * @author  Dr. Klaus Schaefer
 * @brief   Pushbutton handler module
 ******************************************************************************
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include "common.h"
#include "stm324xg_eval.h"

#include "task_builder.h"

COMMON TaskHandle_t button_task_handle;
COMMON TimerHandle_t de_bounce_timer;

/** @brief  This function handles the KEY and TAMPER interrupt request. */
extern "C" void
EXTI15_10_IRQHandler(void)
{
  ENTER_ISR;
  portBASE_TYPE TaskWoken = pdFALSE;

  volatile bool key    =__HAL_GPIO_EXTI_GET_IT(KEY_BUTTON_PIN);
  volatile bool tamper =__HAL_GPIO_EXTI_GET_IT(TAMPER_BUTTON_PIN);
  // reset both EXTI I/O interrupt latches
  __HAL_GPIO_EXTI_CLEAR_IT( KEY_BUTTON_PIN);
  __HAL_GPIO_EXTI_CLEAR_IT( TAMPER_BUTTON_PIN);

  /* disable further interrupts for a while */
  BSP_PB_Init (BUTTON_KEY, BUTTON_MODE_GPIO);
  BSP_PB_Init (BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /* trigger timer callback */
  xTimerStartFromISR(de_bounce_timer, &TaskWoken);

  /* wake up button task */
  if( key)
    vTaskNotifyGiveFromISR( button_task_handle, &TaskWoken);

  /* preempt the active task if necessary */
  portEND_SWITCHING_ISR(TaskWoken);
}

/** @brief  This function handles interrupt requests from WAKEUP
 *
 * It will be called whenever WAKEUP is pressed
 * */
extern "C" void
EXTI0_IRQHandler (void)
{
  /* reset EXTI I/O interrupt latch */
  __HAL_GPIO_EXTI_CLEAR_IT(WAKEUP_BUTTON_PIN);
  BSP_PB_Init (BUTTON_WAKEUP, BUTTON_MODE_GPIO);
  portBASE_TYPE TaskWoken = pdFALSE;
  xTimerStartFromISR(de_bounce_timer, &TaskWoken);
  /* preempt the active task if necessary */
  portEND_SWITCHING_ISR(TaskWoken);
}

/** @brief  This function enables the button interrupt 
 * 
 * The interrupt is re-enabled after waiting for a possible bouncing phase.
 * */
void de_bounce_callback(xTimerHandle)
{
  // once more: reset EXTI I/O interrupt latch
  __HAL_GPIO_EXTI_CLEAR_IT( KEY_BUTTON_PIN);
  BSP_PB_Init (BUTTON_KEY, BUTTON_MODE_EXTI);
  BSP_PB_Init (BUTTON_WAKEUP, BUTTON_MODE_EXTI);
  BSP_PB_Init (BUTTON_TAMPER, BUTTON_MODE_EXTI);
}

/** @brief  Task function responsible for LED blinking on pushbutton command */
void pushbutton_task(void *)
{
  /* Initialize timer */
  de_bounce_timer = xTimerCreate(0, 5, 0, 0, de_bounce_callback);
  ASSERT( de_bounce_timer != 0);

  /* Initialize our LED */
  BSP_LED_Init (LED4);

  /* Initialize USER Button with interrupt capability */
  BSP_PB_Init (BUTTON_KEY, BUTTON_MODE_EXTI);
  BSP_PB_Init (BUTTON_WAKEUP, BUTTON_MODE_EXTI);
  BSP_PB_Init (BUTTON_TAMPER, BUTTON_MODE_EXTI);

  // now go protected
  portSWITCH_TO_USER_MODE();

  for (;;)
    {
      /* indefinitely wait for pushbutton message */
      uint32_t events = ulTaskNotifyTake( pdTRUE, portMAX_DELAY);

      while( events--)
	{
	      /* blink */
	      BSP_LED_On(LED4);
	      vTaskDelay(configTICK_RATE_HZ / 3);
	      BSP_LED_Off(LED4);
	      vTaskDelay(configTICK_RATE_HZ);
	}
    }
}

/** ******************************************************************
 * @brief Task parameters including MPU + FPU settings
 *********************************************************************/
ROM TaskParameters_t button_TaskParameters =
  {
  pushbutton_task, 	/* pvTaskCode       */
  (const char *) "button", /* pcName	    */
  256,			/* usStackDepth	in units of 32 bit words */
  (void *) 0, 		/* pvParameters     */
  portPRIVILEGE_BIT,	/* uxPriority       */
  0,			/* puxStackBuffer: allocated dynamically */
    { /* xRegions */
    /* Base-address Length Parameters */
      { COMMON_BLOCK, COMMON_SIZE, portMPU_REGION_READ_WRITE },
      { 0, 0, 0 },
      { 0, 0, 0 }
    }
  };

/** ******************************************************************
 * @brief global function used to create the button task object
 *********************************************************************/
void start_button_task (uint8_t priority)
{
  TaskParameters_t p = button_TaskParameters;
  p.uxPriority |= priority;
  task_t task( p);
  button_task_handle=task.get_handle();
}

