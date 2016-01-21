// Stepper.c
// Runs on LM4F120 or TM4C123
// Index implementation of a Moore finite state machine to operate
// a Stepper Motor
// Your Name:Shweta Dube
// created: November 14, 2015
// last modified: December 01, 2015
// 


// define your registers here

#include "Nokia5110.h"
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "Control.h"
#include "Driver.h" 
 

int mailbox=0;
char String[9]={'S','T','O','P','P','E','D','\0'}; // string to output to screen
char String1[11]={'T','U','R','N',' ','L','E','F','T','\0'}; // string to output to screen
char String2[12]={'T','U','R','N',' ','R','I','G','H','T','\0'};; // string to output to screen
char String3[12]={'H','A','L','F',' ','R','O','U','N','D','\0'}; // string to output to screen
//void input_switch(void);
void DisableInterrupts(void);               // Disable interrupts
void EnableInterrupts(void);                // Enable interrupts
unsigned long data;
// Define the Linked data structure
// Linked data structure
struct State {
 unsigned long output;     //output
 const struct State *Next[4];		//for 4 states, CW,CCWand HC next array goes to 1st,2nd,3rd and 4th etc state by incrementing  
};       
typedef const struct State SType;


//Declaration of states

SType FSM[13] = { 
			//	00		01			10			11
 {0x00,{&FSM[0],&FSM[1],&FSM[5],&FSM[9]}}, //CW
 {0x0B,{&FSM[2],&FSM[2],&FSM[2],&FSM[2]}},
 {0x0D,{&FSM[3],&FSM[3],&FSM[3],&FSM[3]}},
 {0x15,{&FSM[4],&FSM[4],&FSM[4],&FSM[4]}},
 {0x13,{&FSM[0],&FSM[0],&FSM[0],&FSM[0]}},
 {0x13,{&FSM[6],&FSM[6],&FSM[6],&FSM[6]}}, //CCW
 {0x15,{&FSM[7],&FSM[7],&FSM[7],&FSM[7]}},
 {0x0D,{&FSM[8],&FSM[8],&FSM[8],&FSM[8]}},
 {0x0B,{&FSM[0],&FSM[0],&FSM[0],&FSM[0]}},
 {0x0B,{&FSM[10],&FSM[10],&FSM[10],&FSM[10]}},   //HC
 {0x0D,{&FSM[11],&FSM[11],&FSM[11],&FSM[11]}},
 {0x15,{&FSM[12],&FSM[12],&FSM[12],&FSM[12]}},
 {0x13,{&FSM[0],&FSM[0],&FSM[0],&FSM[0]}},
 };
int i=0;
int j;
unsigned long st;
unsigned long input;
unsigned long scanip;
int main(void){ volatile unsigned long delay;
 SType *pt;

	st=0;
	PLL_Init();       // 80 MHz, Program 10.1
  SysTick_Init();   // Program 10.2
	Control_Init();
	Driver_Init(); 
	Control_Init();
	Nokia5110_Init();                        // LCD display Initialization
	// initialize Ports here. 
	
  pt=&FSM[0];
	while(1){
		
		if(mailbox==1)
		{
		//GPIO_PORTE_DATA_R|=0x01;      //PE0 1
		GPIO_PORTE_DATA_R = pt->output; 										// step motor 
		//GPIO_PORTE_DATA_R=(data<<2)&0x0E;
		input =Control_In(); 									// get Input from sitches
			if (input==0x18)									//switch pressed combination received for TURN RIGHT
			{
				scanip = 0x02;
			}
			else if (input==0x14)				//switch pressed combination received for TURN RIGHT
			{
				scanip=0x01;
			}
			else if (input==0x1c)      //switch pressed combination received for HALF CYCLE
			{
				scanip=0x03;
			}
				else 
			{
				scanip=0x00;
			}
				
		switch(scanip){
				case 0x02:	
							
							Nokia5110_Clear(); 											//clear the LCD
							Nokia5110_SetCursor(1,0);								//setting cursor to x=1 , y=0 position on the LCD
							Nokia5110_OutString(String1);		//output a character on the LCD 'TURN LEFT'
							SysTick_Wait1ms(6); 					// wait for time according to state  to get speed 30 RPM
							pt = pt->Next[scanip];//FSM[state].Next[input];		// change the state based on input and current state
									break; 
				
				case 0x01:  
							Nokia5110_Clear(); 											//clear the LCD
							Nokia5110_SetCursor(1,0);								//setting cursor to x=1 , y=0 position on the LCD
							Nokia5110_OutString(String2);		//output a character on the LCD 'TURN RIGHT'
							SysTick_Wait1ms(6); 					// wait for time according to state  to get speed 30 RPM
							pt = pt->Next[scanip];//FSM[state].Next[input];		// change the state based on input and current state
									break; 
				
				case 0x03:
				//	j=1;   //setting  the flag to 1 for half cycle
				if(j==1)
				{
				do
				{
							Nokia5110_Clear(); 											//clear the LCD
							Nokia5110_SetCursor(1,0);								//setting cursor to x=1 , y=0 position on the LCD
							Nokia5110_OutString(String3);		//output a character on the LCD 'HALF ROUND'
							SysTick_Wait1ms(5); 					// wait for time according to state to get speed 5 RPM
							pt = pt->Next[scanip]; //FSM[state].Next[input];		// change the state based on input and current state
								i++;
					//input =Control_In(); 
				} while (i==100);
			}
				else 
				{
						GPIO_PORTE_DATA_R = 0x00; 
				}
					break;
				
				default:
							Nokia5110_Clear(); 											//clear the LCD
							Nokia5110_SetCursor(1,0);								//setting cursor to x=1 , y=0 position on the LCD
							Nokia5110_OutString(String);	//output a character on the LCD 'STOPPED'
									break;
			}  
		}
  }
}

//check for motor on/off

void GPIOPortB_Handler(void)
{
	GPIO_PORTB_ICR_R = 0x10;
		mailbox=1;
j=1;	//mailbox is set to 1 if switch 1 (motor on/off) is pressed
}

/*void input_switch(void)
{
	unsigned long input_sw;
	
	
	
}*/


