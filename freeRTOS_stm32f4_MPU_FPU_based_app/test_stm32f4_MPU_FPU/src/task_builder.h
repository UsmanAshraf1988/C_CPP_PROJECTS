/**
 @file task_builder.h
 @brief class task_t
 @author: Dr. Klaus Schaefer
 */

#ifndef TASKBUILDER_H
#define TASKBUILDER_H

#include "FreeRTOS.h"
#include "task.h"

#include "common.h"

class task_t
{
public:
  task_t(TaskParameters_t p)
{
    if( p.puxStackBuffer == 0) // dynamically allocate stack memory if none given
    {
	p.puxStackBuffer=(StackType_t* )pvPortMallocAlignedMemory( p.usStackDepth*sizeof( portSTACK_TYPE), p.usStackDepth*sizeof( portSTACK_TYPE));
	ASSERT( p.puxStackBuffer != 0);
    }
    xTaskCreateRestricted(&p, &task_handle);
}
  inline TaskHandle_t get_handle( void) const
  {
    return task_handle;
  }
private:
  TaskHandle_t task_handle;
};

#endif /* TASKBUILDER_H */
