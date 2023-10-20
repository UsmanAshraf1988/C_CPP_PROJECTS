/*
 ============================================================================
 Name        : FSM_light.c
 Author      : Usman Ashraf
 Version     : 0.1v
 Copyright   : GPL2
 Description : Light-bulb with finite-state-machine, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/*Light-bulb FSM*/
/*Callback function for state*/
typedef void (*stateCallbackFunction)(void);

/*possible states*/
typedef enum {
	INIT,
	ON,
	OFF,
	BROKEN
} STATE;

/*possible action*/
typedef enum {
	CURRENT_0_AMP,
	CURRENT_100_MILIAMP,
	CURRENT_900_MILIAMP
} ACTION;

/*state table structure*/
typedef struct {
	STATE currentState;
	ACTION action;
	STATE nextState;
	stateCallbackFunction currentStateCallbackFunction;
}STATE_TABLE;

/*functions of state machine*/
void lightOn(void)		{	printf("light ON \n");	}
void lightOff(void)		{	printf("light OFF \n");	}
void lightBroken(void)	{	printf("light Broken \n");	}
void execute_FSM(STATE*, ACTION, stateCallbackFunction*);
void print_stateAndAction(STATE, ACTION, stateCallbackFunction);

/*state table*/
static STATE_TABLE state_table[] = {
		{INIT, 		CURRENT_100_MILIAMP, 	ON, 		lightOn },
		{INIT, 		CURRENT_900_MILIAMP, 	BROKEN, 	lightBroken },

		{ON, 		CURRENT_0_AMP, 			OFF, 		lightOff },
		{ON, 		CURRENT_900_MILIAMP, 	BROKEN, 	lightBroken },

		{OFF, 		CURRENT_100_MILIAMP, 	ON, 		lightOn },
		{OFF, 		CURRENT_900_MILIAMP, 	BROKEN, 	lightBroken },
};


int main(void) {

	/*INIT state*/
	STATE current_state = INIT;
	stateCallbackFunction currentState_callbackFunction = NULL;

	unsigned int option=0u;
	while(1){
		printf("Enter:: 0 to OFF/INIT, 1 to ON, 2 to Break\n");
		fflush(stdout);
		scanf("%d", &option); /*Unfortunately scanf_s is not available in my installed compiler version.*/

		if(option>=0 && option <=2){
			execute_FSM(&current_state, (ACTION)option, &currentState_callbackFunction);
		}else{
			printf("Wrong input: Therefore same state: Try next time\n");
		}
	}

	return EXIT_SUCCESS;
}



void execute_FSM(STATE * current_state, ACTION current_action, stateCallbackFunction * currentState_callbackFunction){
	unsigned int i=0u;
	unsigned int no_of_state_table_entries = (sizeof(state_table)/sizeof(state_table[0]));
	for(i=0; i<no_of_state_table_entries; i++){
		if( (*current_state == state_table[i].currentState) && (current_action==state_table[i].action) ){
			*current_state = state_table[i].nextState;
			*currentState_callbackFunction =  state_table[i].currentStateCallbackFunction;
		}
	}
	print_stateAndAction(*current_state, current_action, *currentState_callbackFunction);
}


void print_stateAndAction(STATE current_state, ACTION current_action, stateCallbackFunction currentState_callbackFunction){
	if(current_state==INIT){
		printf("State: INIT \n");
	}else if(current_state==ON){
		printf("State: ON \n");
	}else if(current_state==OFF){
		printf("State: OFF \n");
	}else if(current_state==BROKEN){
		printf("State: BROKEN \n");
	}else{
		printf("State: UNKOWN-ERROR \n");
	}

	if(current_action==CURRENT_0_AMP){
		printf("Action: CURRENT_0_AMP \n");
	}else if(current_action==CURRENT_100_MILIAMP){
		printf("Action: CURRENT_100_MILIAMP \n");
	}else if(current_action==CURRENT_900_MILIAMP){
		printf("Action: CURRENT_900_MILIAMP \n");
	}else{
		printf("Action: UNKOWN-ERROR \n");
	}
	if(currentState_callbackFunction!=NULL){
		currentState_callbackFunction();
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	fflush(stdout);
}
