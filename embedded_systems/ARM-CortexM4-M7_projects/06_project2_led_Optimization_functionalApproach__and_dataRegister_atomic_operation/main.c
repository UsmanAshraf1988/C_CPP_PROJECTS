//! @file main.c
//!
//!
//! @brief TI Launchpad Port-F led drivers are implemented from scratch.
//!
//!
//! @Author Usman Ashraf
//! @email  u.ashraf1988@gmail.com
//!
//! GNU Lesser General Public License v3.0, free to use, re-use, distriution and modification
//! provided this above header information. Author take no responsibility for any outcome of
//! this code.
//!
//! If you need more information, please, contact me at my above email address.
//!


#include "tm4c123gh6pm.h"

// Macros and Constant
#define LED_RED     (1u<<1)
#define LED_BLUE    (1u<<2)
#define LED_GREEN   (1u<<3)

#define LOOP_FINAL_VALUE       3000000U
#define TOTAL_NUMBER_OF_LIGHTS 7U
#define CONST_ZERO             0U


// Global Data
volatile static unsigned int loopCounter = CONST_ZERO;
static unsigned int lightNoCounter = CONST_ZERO;
  
const static unsigned int typesOfLightsArr [ TOTAL_NUMBER_OF_LIGHTS ] = { LED_RED,  
  LED_BLUE, LED_GREEN, ( LED_RED | LED_BLUE ), ( LED_BLUE | LED_GREEN ),
  ( LED_GREEN | LED_RED), ( LED_RED | LED_BLUE | LED_GREEN ) };


// Function Declaration

// Function to initialize global data.
void initializeLEDGlobalData(void);

// Function to configure LED pins on GPIO Port F.
void configureLEDPinsOnGPIOPortF(void);

// Function to run LED sequence.
void runLEDSequence(void);

int main()
{
  initializeLEDGlobalData();
  configureLEDPinsOnGPIOPortF();
  
  while(1){
    runLEDSequence();
  }// while
  
  //return 0;
} // main



// Function to initialize global data.
void initializeLEDGlobalData(void){

  loopCounter = CONST_ZERO;
  lightNoCounter = CONST_ZERO;  
}  //initializeLEDGlobalData()



// Function to configure LED pins on GPIO Port F.
void configureLEDPinsOnGPIOPortF(void){
  
  //SYSCTL_RCGCGPIO_R5 //GPIO Port F Run Mode Clock Gating Control.
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // setting gate clock for port f.
  
  // seting port f pin 1,2,3 dir as out.
  GPIO_PORTF_DIR_R |= ( LED_RED | LED_BLUE | LED_GREEN );
  
  // setting port f pin 1,2,3 as digitally enabled pin. (by default all pins
  // are disabled).
  GPIO_PORTF_DEN_R |= ( LED_RED | LED_BLUE | LED_GREEN );

}  // configureLEDPinsOnGPIOPortF()



// Function to run LED sequence.
void runLEDSequence(void){
  
  loopCounter = CONST_ZERO;    
  // setting led on.
  // GPIO_PORTF_DATA_R == 0x400253FC ==> last 10 bits ==> 3FC for port F.
  // Pot F atomic operation 3FC.
  // 3FC  ==  0  0  1  1    1  1  1  1    1  1  0  0
  // addr ==  x  x  a7 a6   a5 a4 a3 a2   a1 a0 x  x
  // PortF =  x  x  p7 p6   p5 p4 p3 p2   p1 p0 x  x 
  // so all pin data of portF data register would be processed.
  //GPIO_PORTF_DATA_R |= typesOfLightsArr [ lightNoCounter ] ;

  // Port F data register usage out 256 available registers, 0x40025000[14]
  // 15*4 == 56 == 0x38
  // 0x40025000[14] == 0x40025000 + 0x38 == 0x40025038
  // 38 == 0011 1000 ; so excluding a0, a1, but we are giving signal to a2,a3,a4.
  // so pin data p2, p3, p4 of portF only would be processed. 
  GPIO_PORTF_DATA_BITS_R[14] |= typesOfLightsArr [ lightNoCounter ] ;
  while( loopCounter < LOOP_FINAL_VALUE ) {
    loopCounter++;
  }


  
  loopCounter = CONST_ZERO;
  //setting led off.
  // GPIO_PORTF_DATA_R == 0x400253FC ==> last 10 bits ==> 3FC for port F.
  // Pot F atomic operation 3FC.
  // 3FC  ==  0  0  1  1    1  1  1  1    1  1  0  0
  // addr ==  x  x  a7 a6   a5 a4 a3 a2   a1 a0 x  x
  // PortF =  x  x  p7 p6   p5 p4 p3 p2   p1 p0 x  x 
  // so all pin data of portF data register would be processed.
  //GPIO_PORTF_DATA_R &= ~( typesOfLightsArr [ lightNoCounter ] );

  // Port F data register usage out 256 available registers, 0x40025000[14]
  // 15*4 == 56 == 0x38
  // 0x40025000[14] == 0x40025000 + 0x38 == 0x40025038
  // 38 == 0011 1000 ; so excluding a0, a1, but we are giving signal to a2,a3,a4.
  // so pin data p2, p3, p4 of portF only would be processed.   
  GPIO_PORTF_DATA_BITS_R[14] &= ~( typesOfLightsArr [ lightNoCounter ] );
  while( loopCounter < LOOP_FINAL_VALUE ) {
    loopCounter++;
  }

  
  // It is post led ledTypecheck, so for const_zero we are checking against
  // "TOTAL_NUMBER_OF_LIGHTS-1".
  if ( lightNoCounter == TOTAL_NUMBER_OF_LIGHTS-1 ) {
    lightNoCounter = CONST_ZERO;
  } else {
    lightNoCounter++;
  }
  
} // runLEDSequence()
