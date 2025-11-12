/*
 * led.c
 *
 *  Created on: Sep 10, 2025
 *      Author: Juan Contreras
 *      Function moved over from main.c
 *      LED State machine function
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"
extern volatile unsigned char display_changed;
extern volatile char one_time = 0;
extern volatile unsigned int Time_Sequence = 0;
unsigned int lt_stop;



/*
 * Carlson_StateMachine
 * Changes LED states and updates LCD based on Time_Sequence.
 */
void Carlson_StateMachine(void){
    switch(Time_Sequence){
      case 250:                        //
        if(one_time){
          Init_LEDs();
          if (lt_stop == 0){
          lcd_BIG_mid();
          }
          display_changed = 1;
          one_time = 0;
        }
        Time_Sequence = 0;             //
        //stopall();
        break;
      case 200:                        //
        if(one_time){
//          P1OUT &= ~RED_LED;            // Change State of LED 4
          P6OUT |= GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 150:                         //
        if(one_time){
          P1OUT |= RED_LED;            // Change State of LED 4
          P6OUT &= ~GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 100:                         //
        if(one_time){
//          lcd_4line();
          if (lt_stop == 0){
          lcd_BIG_bot();
          }
          P6OUT |= GRN_LED;            // Change State of LED 5
          display_changed = 1;
          one_time = 0;
        }
        break;
      case  50:                        //
        if(one_time){
          one_time = 0;
        }
        break;                         //
      default: break;
    }
}
