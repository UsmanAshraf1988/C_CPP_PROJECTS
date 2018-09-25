/*
 * system_support.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: schaefer
 */

#include "stm32f4xx_hal.h"
//#include "core_cm4.h"

extern "C" void vPortInitMemory ();

void init_system (void)
{
  SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk);

  extern int _s_system_ram;
  extern int _e_system_ram;
  int *ptr;
  for (ptr = &_s_system_ram; ptr < &_e_system_ram; ++ptr)
    *ptr = 0;

  vPortInitMemory ();
}

extern "C" void vApplicationReturnFromTaskProcedureHook(void)
{
  // obviously you returned from a task's procedure !
  while(1)
      asm volatile( "bkpt 0");
}
