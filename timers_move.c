/*
 * timers_move.c
 *
 *  Created on: Oct 7, 2025
 *      Author: jcont
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int timer_counter;
extern volatile unsigned int LCD_counter;
volatile unsigned int time_state = 0;

void state_timer(){
    if (timer_counter> 200 && timer_counter < 400){
        time_state = 1;
    }
    else if (timer_counter> 400 && timer_counter < 600){
        time_state = 2;
    }
    else if (timer_counter> 600 && timer_counter < 1000){
        time_state = 3;
    }
    else if (timer_counter> 1000 && timer_counter < 1200){
        time_state = 4;
    }
    else if (timer_counter> 1200 && timer_counter < 1400){
        time_state = 5;
    }
    else if (timer_counter> 1400 && timer_counter < 1600){
        time_state = 6;
    }
    else if (timer_counter> 1600 && timer_counter < 2200){
        time_state = 7;
    }
    else if (timer_counter> 2200 && timer_counter < 2600){
        time_state = 8;
    }
    else if (timer_counter> 2600 && timer_counter < 3200){
        time_state = 9;
    }
    else if (timer_counter > 3200){
        time_state = 10;
    }
}

void timed_moves(void){
    switch(time_state){
    case 1:
        moveforward();
        strcpy(display_line[1]," FORWARD ");
        break;
    case 2:
        stopall();
        strcpy(display_line[1],"  STOP  ");
        break;
    case 3:
        movereverse();
        strcpy(display_line[1]," BACKWARD ");
        break;
    case 4:
        stopall();
        strcpy(display_line[1],"   STOP  ");
        break;

    case 5:
        moveforward();
        strcpy(display_line[1]," FORWARD ");
        break;
    case 6:
        stopall();
        strcpy(display_line[1],"  STOP  ");
        break;
    case 7:
        spin_clockwise();
        strcpy(display_line[1],"CLOCKWISE");
        break;
    case 8:
        stopall();
        strcpy(display_line[1],"   STOP  ");
        break;
    case 9:
        spin_counterclockwise();
        strcpy(display_line[1],"COUNTERCW");
        break;
    case 10:
        stopall();
        strcpy(display_line[1],"   STOP      ");
        break;
    }
}



void LCD_update(void){
    if (LCD_counter>40){
        display_changed=TRUE;
        Display_Update(0,0,0,0);
        LCD_counter = 0;
    }
}


