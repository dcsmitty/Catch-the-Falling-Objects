/* Partner 1 Name & E-mail:dsmit017 (dsmit017@ucr.edu)
* Lab Section:23
* Assignment: Custom Lab Project
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "C:\Users\student\Desktop\includes\io.c"
#include "C:\Users\student\Desktop\includes\timer.h"
#include "C:\Users\student\Desktop\includes\keypad.h"
#include <string.h>



//--------Task scheduler data structure--------------------
// Struct for Tasks represent a running process in our
// simple real-time operating system.
/*Tasks should have members that include: state, period, a
measurement of elapsed time, and a function pointer.*/
typedef struct _task {
	//Task's current state, period, and the time elapsed
	// since the last tick
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	//Task tick function
	int (*TickFct)(int);
} task;
//--------End Task scheduler data structure----------------
//--------Shared Variables---------------------------------
unsigned char SM2_output = 0x00;
unsigned char SM3_output = 0x00;
unsigned char pause = 0;
//--------End Shared Variables-----------------------------
//--------User defined FSMs--------------------------------

unsigned char Message1[] = {"1) Play!"};
unsigned char Message2[] = {"2) High Score!"};

unsigned char button1;
unsigned char button2;
unsigned char DiffFlag;

enum MenuStates{MenuStart, MenuWait} Menustate;
unsigned char count;

int MenuTick(int state)
{
	if(!DiffFlag)
	{
		switch(Menustate)//transitions
		{
			case MenuStart:
				LCD_DisplayString(1, Message1);
				LCD_Cursor(17);
				for(unsigned i = 0; i<14; i++)
				{
					LCD_WriteData(Message2[i]);
				}
				Menustate = MenuWait;
				break;
			case MenuWait:
				if(button1)
				{
					DiffFlag = 1;
				}
				else if(button2)
				{
					LCD_DisplayString(1, "High Score = 0");
				}
				break;
		}
	
		switch(Menustate)//state actions
		{
			case MenuStart:
				break;
			case MenuWait:
				break;
		}
	}
	
	return Menustate;
}

enum DiffStates{DiffStart, DiffWait, DiffEasy, DiffMedium, DiffHard, DiffWait2} Diffstate;
unsigned char Diff1[] = {"1) Easy"};
unsigned char Diff2[] = {"2) Medium"};
unsigned char Diff3[] = {"3) Hard"};
	
	
int Difficulty(int state)
{
	if(DiffFlag)
	{
		switch(Diffstate)//transitions
		{
			case DiffStart:
				LCD_ClearScreen();
				LCD_DisplayString(1, Diff1);
				LCD_Cursor(10);
				for(unsigned i = 0; i < 7; i++)
				{
					LCD_WriteData(Diff3[i]);
				}
				LCD_Cursor(17);
				for(unsigned i = 0; i < 9; i++)
				{
					LCD_WriteData(Diff2[i]);
				}
				Diffstate = DiffWait;
				break;
			case DiffWait:
				break;
			case DiffEasy:
				break;
			case DiffMedium:
				break;
			case DiffHard:
				break;
			case DiffWait2:
				break;
		}
	
		switch(Diffstate)//state actions
		{
			case DiffStart:
				break;
			case DiffWait:
				break;
			case DiffEasy:
				break;
			case DiffMedium:
				break;
			case DiffHard:
				break;
			case DiffWait2:
				break;
		}
	}
	
	return Diffstate;
}

// Implement scheduler code from PES.
int main()
{
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA
	// to pull down logic
	DDRA = 0x03; PORTA = 0xFC;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	// . . . etc
	
	
	LCD_init();
	
	count = 0;
	DiffFlag = 0;
	
	// Period for the tasks
	unsigned long int Menu_period = 1;
	unsigned long int Diff_period = 1;
	
	//Declare an array of tasks
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	// Task 1
	task1.state = -1;
	task1.period = Menu_period;
	task1.elapsedTime = Menu_period;
	task1.TickFct = &MenuTick;
	
	// Task 2
	task2.state = -1;
	task2.period = Diff_period;
	task2.elapsedTime = Diff_period;
	task2.TickFct = &Difficulty;
	
	// Set the timer and turn it on
	TimerSet(1);
	TimerOn();
	
	// Scheduler for-loop iterator
	unsigned short i;
	while(1) {
		button1 = ~PINA & 0x04;
		button2 = ~PINA & 0x08;
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	// Error: Program should not exit!
	return 0;
}
