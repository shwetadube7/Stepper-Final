// Control.h
// This software configures the off-board control switches
// Runs on LM4F120 or TM4C123
// Program written by: Shweta Dube

// Date Created: 10/22/15
// Last Modified: 10/24/15 
// Lab number: 7
// Hardware connections PB4-PB2 are connected to off-board switches
// PB4 is used to turn the motor on/off, if PB2 is 1 the motor will be on waiting for commands
// PB3 makes the motor turn clockwise
// PB2 makes the motor turn counter clockwise
// PB4-2 tirggers edge-trigger interrupts at the rising edge


// Header files contain the prototypes for public functions
// this file explains what the module does

// **************Control_Init*********************
// Initialize switches inputs, called once 
// Input: none 
// Output: none
void Control_Init(void);

// **************Piano_In*********************
// Input from switches inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 
unsigned long Control_In(void);
