
// Control.c
// This software configures the off-board control switches
// Runs on LM4F120 or TM4C123
// Program written by: Shweta Dube
// Date Created: 10/22/15
// Last Modified: 10/24/15 
// Lab number: 6
// Hardware connections PB4-PB2 are connected to off-board switches
// PB4 is used to turn the motor on/off, if PB2 is 1 the motor will be on waiting for commands
// PB3 makes the motor turn clockwise
// PB2 makes the motor turn counter clockwise
// PB4 tirggers edge-trigger interrupts at both edges

#include "tm4c123gh6pm.h"
#include "Control.h"
// defing registers here

unsigned short delay;
unsigned long sw;
void EnableInterrupts(void);  // Enable interrupts
// **************Control_Init*********************
// Initialize control switches inputs, called once 
// Input: none 
// Output: none

void Control_Init(void){
	SYSCTL_RCGC2_R |= 0x00000002;     //activate clock for Port B
  delay = SYSCTL_RCGC2_R;           //allow time for clock to start
  GPIO_PORTB_AMSEL_R = 0x00;        //disable analog on PB
  GPIO_PORTB_PCTL_R = 0x00000000;   //PCTL GPIO on PF4-0
  GPIO_PORTB_DIR_R &= ~0x1C;        //PB2-PB4 input 
  GPIO_PORTB_AFSEL_R = 0x00;        //disable alt funct on PB7-0
  GPIO_PORTB_DEN_R = 0xFF;          //enable digital I/O on PB4-0
	//GPIO_PORTB_PUR_R &= 0x10;       //enable weak pull-up on PB4
  GPIO_PORTB_IBE_R |=0x10;          //PB4 is both edges
  GPIO_PORTB_IM_R |= 0x10;          //arm interrupt on PB4
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0x00002000;     // priority 1 
	NVIC_EN0_R=0x00000002;						//EN0 enabled for port B
	EnableInterrupts();
}	 
	
	
// **************Control_In*********************
// Input from switches  
// Input: none 
// Output: 0 to 7 depending on keys
// PB4 is on/off, PB3 CW, PB2 CCW
// 

unsigned long Control_In(void){ 
	
  sw=0; 
	sw=(GPIO_PORTB_DATA_R & 0x1C); //checks for PB2 and PB3 and PB4
	return(sw); //returns switch pressed whether its sw1 and sw2(CW) or sw3(CCW) 
 }

