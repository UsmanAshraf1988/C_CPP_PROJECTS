// *********************************************************************************
//
// Functionality (RS232 protocol):
// - Manual Drive (D)
// - Stop (S)
// - Hardware Diagnostics (H)
// - Pedestrian Protection (4A)

#include <Std_Types.h>             /* AUTOSAR Standard Types */
#include <os.h>                    /* AUTOSAR OS */
#include <Wdg.h>                   /*AUTOSAR Wdg*/ 

#include <Port.h>                  /* AUTOSAR Port Driver */
#include <Dio.h>                   /* AUTOSAR Dio Driver*/ 
#include <mcu.h>				   /* Autosar MCU Driver */


//Platform type definitions
#include "base_type.h"

// Application Code
#include "log.h"			// Debugging via Serial
#include "seven.h"			// Seven segment display
//#include "rfm12.h"

#include "ringbuffer.h"		//Intermediate data buffer



/********************************************************
/* Declarations
********************************************************/

DeclareTask(tsk_Init);
DeclareTask(tsk_Background);
DeclareTask(tsk_Control);
DeclareTask(tsk_Timer);

DeclareAlarm(alrm_Tick1ms);
DeclareAlarm(alrm_RandomTime);
DeclareAlarm(alrm_Timeout);

DeclareEvent(ev_SW2);
DeclareEvent(ev_SW3);
DeclareEvent(ev_RandomTime);
DeclareEvent(ev_Timeout);



void main(void)
{
	StartOS(OSDEFAULTAPPMODE);


} /* void main(void) */



TASK(tsk_Init)
{

	// Port Initialisation
	Port_Init(&Port_ConfigData);

	// Init MCU and enable external interrupts
	Mcu_Init(&McuModuleConfiguration);	
	//LOG_comment("Nach MCU");

	// init the WdgM
	Wdg_Init( &Wdg_ConfigData);  

	//Init the Logging
	LOG_init(PORTUSB, 10);

	// Initialise the seven segment display
	SEVEN_set(1,0);
	SEVEN_set(2,0);

	//Initialisation of the cyclic tasks
	//Time in µs
    //Todo: Install the alarm alrm_sample which will be fired every 1ms
//#error Add your code here....
	SetRelAlarm (alrm_Tick1ms, 1000, 1000);

	EnableAllInterrupts();

	//Activate the "endless loop" tasks
    //Todo: activate the tasks tsk_Control and tsk_Background
	ActivateTask(tsk_Control);
	ActivateTask(tsk_Background);
	LOG_comment("Reaction Test Game. Press any one button to start.");
	//LOG_comment("Finished Initialisation - press any board button to start game");
	TerminateTask();
	


} /* TASK(StartupTask) */

///////////////////////////////////////////////////////////////

// TASK(tsk_Sample)
// wille be fired every 10ms by the alarm alrm_sample
TASK(tsk_Timer)
{
    //Increment the game timer every 1 ms
    //Todo: call this function in itertion 3
    GAME_Increment1msSoftwareTimer();
	
	

	TerminateTask();
} /* TASK(tsk_Sample) */

///////////////////////////////////////////////////////////////

TASK(tsk_Control)
{

    //Declare the receiving event mask
	EventMaskType myEvent;

	while(1)
	{
        //Wait for event mask and read the mask
		WaitEvent(ev_SW2 | ev_SW3 | ev_RandomTime | ev_Timeout);
		GetEvent(tsk_Control, &myEvent);

        //Clear the event mask
        ClearEvent(myEvent);

        //Process the events in the central state machine
        //This line can be deactivated once the game is running
        //LOG_comment("Event has been fired to control task");

        //Todo: call this function in itertion 3
        GAME_playGameOneRound(myEvent);

	}
	TerminateTask();
} /* TASK(tsk_Control) */

///////////////////////////////////////////////////////////////

TASK(tsk_Background)
{
	//LOG_comment("back");

	while (1)
	{
			Wdg_Trigger();
	}
} /* TASK tsk_Background */


/***************************************************************************************************
 * ISR
 **************************************************************************************************/

//IRQ12
ISR(ISR_SW2)
{
	EIRR1_ER12 = 0; //Clear ISR flag

    //Send the event ev_SW2 to tsk_Control
//#error Add your code here....
	SetEvent(tsk_Control, ev_SW2);
	//LOG_comment("i2");
}

//IRQ13
ISR(ISR_SW3)
{
	EIRR1_ER13 = 0;
    //Send the event ev_SW3 to tsk_Control
//#error Add your code here....
	SetEvent(tsk_Control, ev_SW3);
	//LOG_comment("i3");
}



		