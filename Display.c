/*
 * Display.c
 *
 *  Created on: Sep 10, 2025
 *      Author: Juan Contreras
 *      The following is the function originally found in main.c
 *      Includes were added to ensure that this separate c file can function properly
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;



/*
 * Display_Process
 * Updates LCD if conditions are met.
 */

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}
