#include <stdlib.h>

#include "base_type.h"
#include "game.h"
#include "os.h"
#include "seven.h"
#include "log.h"



/*************************************************************************
 * Private, static functions
 ************************************************************************/
/**
 * Reset the internal timer variable
 * @param void
 * @return void
 */
static void GAME__resetTickCounter();

/**
 * Read the internal timer variable
 * @param void
 * @return uint32_t representing the time in ms
 */
static uint32_t GAME__readTickCounter();

/**
 * Set and start the random time alarm
 * @param void
 * @return void
 */
static void GAME__resetRandomAlarm();

/**
 * Set and start the timeout alarm
 * @param void
 * @return void
 */
static void GAME__resetTimeoutAlarm();

/**
 * Set the LED's to 1 or 2
 * @param void
 * @return void
 */
static void GAME__setRandomLed();

/**
 * Set the LED's to 0
 * @param void
 * @return void
 */
static void GAME__clearRandomLed();

/**
 * Get the LED's value
 * @param void
 * @return void
 */
static boolean_t GAME__getRandomLed(uint8_t led);


/*************************************************************************
 * Private, static variables
 ************************************************************************/
typedef enum state{ISSTANDBY, ISWAITINGRANDOMTIME, ISWAITINGUSER} GAME_state_t;


static GAME_state_t GAME__state = ISSTANDBY;       //current state of the game
static uint8_t GAME__led = 0;                      //value being displayed
static uint32_t GAME__counter = 0;                 //current timer counter value, one increment is 1ms


void GAME_playGameOneRound(EventMaskType myEvent)
{
    uint8_t         evCount = 0;        //The position of the highest bit in the eventmask
    EventMaskType   evLocal;            //Event mask only containing the remaining highest bit
    uint32_t        reactionTime = 0;   //Reaction time of the player

    while (myEvent)
    {
        //Get the highest bit of the event mask and process it
        evCount = 0;
		evLocal = myEvent;

        //Shift the eventmask to the right until only one bit which is set is left
        //This is the highest bit
		while (evLocal != 1)
		{
			evLocal=evLocal>>1;
			evCount++;
		}

        //Create a local mask which only contains the highest bit.
        //Clear this bit in the local copy
        evLocal = 1<<(evCount);
		myEvent = myEvent & ~evLocal;

        //The state machine
        switch (GAME__state)
        {
        case ISSTANDBY : 
            if ((evLocal == ev_SW2) || (evLocal == ev_SW3))
            {
                GAME__clearRandomLed();
                GAME__resetRandomAlarm();

                GAME__state = ISWAITINGRANDOMTIME;
            }
            break;

        case ISWAITINGRANDOMTIME :
			if(evLocal == ev_RandomTime)
			{
				GAME__setRandomLed();
				GAME__resetTimeoutAlarm();
				GAME__resetTickCounter();
				GAME__state = ISWAITINGUSER;
				
			
            //Add your code here
			}
            break;

        case ISWAITINGUSER : 
			if(evLocal == ev_SW2 || evLocal == ev_SW3 || evLocal == ev_Timeout)
			{
				if ((evLocal == ev_SW2 && GAME__led == 1) || (evLocal == ev_SW3 && GAME__led == 2))
				{
					LOG_comment("Well Done! You passed EOS!");
					LOG_write("Your time is ", GAME__readTickCounter(), 'd');
					//LOG_comment(GAME__readTickCounter());
				}
				else
				{
					LOG_comment("OOPS! You failed in EOS :-P ");
				}
				GAME__state = ISSTANDBY;
				LOG_comment("Reaction Test program. Press any one button to start");
			}
            //Add your code here
            break;

        default:
            //This should never happen
            break;
        } //statemachine
    } //while event
} //GAME_playGameOneRound


static void GAME__resetTickCounter()
{
    //Set counter to 0
    //Add your code here
	GAME__counter = 0;
}

static uint32_t GAME__readTickCounter()
{
    return GAME__counter;
}

void GAME_Increment1msSoftwareTimer()
{
    //Increment the game counter by 1
	GAME__counter++;
}

static void GAME__resetRandomAlarm()
{
    uint32_t randomtime;
    
    //Seed the random generator
    //Add your code here
	randomtime = rand() % 1000;
	
    //Get the random time in micro seconds
    //Add your code here
	randomtime = randomtime * 3 * 1000;
	
    //Fire alarm only once / cancel before for robustness
    CancelAlarm(alrm_RandomTime);
	SetRelAlarm(alrm_RandomTime, randomtime, randomtime);
    //Add your code here
}

static void GAME__resetTimeoutAlarm()
{
    //Fire alarm only once after 1s
    CancelAlarm(alrm_Timeout);
    SetRelAlarm(alrm_Timeout, 1000000,0);
}

static void GAME__setRandomLed()
{
    uint8_t randomValue;

    randomValue = rand() % 100;

    if (randomValue < 50)
    {
	    GAME__led = 1;
    }
    else
    {
	    GAME__led = 2;
    }

    SEVEN_set(1,GAME__led);
    SEVEN_set(2,GAME__led);
}

static void GAME__clearRandomLed()
{
    SEVEN_set(1,0);
    SEVEN_set(2,0);

    GAME__led = 0;
}

static boolean_t GAME__getRandomLed(uint8_t led)
{
    return (GAME__led == led);
}

