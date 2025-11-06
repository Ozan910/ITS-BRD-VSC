/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
//#include "error.h"

#include "token.h"
#include "scanner.h"
#include "display.h"
#include "stack.h"
#include "rechner.h"
#include "fehler.h"

//NUMBER WENN NUMBER EINGEGEBEN NEUER SCAN IN CASE NUMBER NUR NACH NEUEN ZAHLEN ODER NACH ENTER

/*
#define NUMBER     'n'    //!< Token for an int number 
#define PLUS       '+'    //!< Token for + operator 
#define MINUS      '-'    //!< Token for - operator 
#define MULT       '*'    //!< Token for * operator 
#define DIV        '/'    //!< Token for / operator 
#define PRT        'p'    //!< Token for print command 
#define SWAP       'r'    //!< Token for swap command 
#define PRT_ALL    'P'    //!< Token for print total stack command
#define CLEAR      'C'    //!< Token for clear command
#define DOUBLE     'd'    //!< Token for double top of stack command 
#define ENTER      ' '    //!< Token for ENTER button
#define UNEXPECTED 'U'    //!< Token for an unexpected command / button
#define OVERFLOW   'O'    //!< Overflow during input of a number
*/

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	//GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	//TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	initDisplay();
	setNormalMode();
	//printStdout("alles gut stdout?");
	//printToEchoLine('X');


	//superloop
	while(1){
		T_token currentToken = nextToken();
		int ret = SUCCESS;
		switch (currentToken.tok){
			case NUMBER:
				ret = stack_push(currentToken.val);
				break;

			case PLUS:
				ret = addition();
				break;

			case MINUS:
				ret = substraction();
				break;
			
			case MULT:
				ret = multiply();
				break;
			
			case DIV:
				ret = divide();
				break;

			case PRT:{
				if(getCount() == 0){
					ret = STACK_EMPTY;
					break;
				}
				int x;
				ret = stack_peek(&x);
				if(ret == 0){
					myPrint(x);
				}
				break;
			}

			case SWAP:
				ret = swap();
				break;
			

			case PRT_ALL:{
				int currentStackSize = getCount();
				if(currentStackSize == 0){
					ret = STACK_EMPTY;
					break;
				}
				int stackContent[currentStackSize];
				//im while loop stack in array schreiben
				int i = 0;
				int val;
				
				while(i < currentStackSize){
					stack_pop(&val);
					myPrint(val);
					stackContent[i] = val;
					i++;
				}
				while(i > 0){
					i--;
					stack_push(stackContent[i]);
				}
				break;
			}

			case CLEAR:
				stack_reset();
				clearStdout();
				clearEchoTerm();
				break;

			case DOUBLE:
				ret = duplicate();
				break;

			//case ENTER:
			//	break;

			//case UNEXPECTED:
			//	break;

			case OVERFLOW:
			ret = INTEGER_OVERFLOW;
				break;

			default:
				ret = SUCCESS;
				break;
		}

		refresh(ret);
	}
}

//refresh nur einmal am ende aufrufen
//print in rechner verlagern zahl in print und dann printet er string lol
//number schreiben: num token nur bei enter:

// EOF
