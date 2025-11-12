//------------------------------------------------------------------------------
// main.c
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "macros.h"
#include  "ports.h"

//------------------------------------------------------------------------------
extern volatile unsigned int Time_Sequence;
extern volatile unsigned int Second_Count;
extern volatile unsigned char hw8_sw2_pressed;
extern volatile unsigned char hw8_sw1_pressed;

// Forward decls for UI helpers provided in transmit.c
void ui_show_waiting(void);
void display_baud(void);
void menu(void);

//------------------------------------------------------------------------------
void Init_Conditions(void);
void enable_interrupts(void);
void Clear_Variables(void);

//------------------------------------------------------------------------------
void main(void) {
  WDTCTL = WDTPW | WDTHOLD;      // Stop watchdog
  Init_Ports();
  Init_Conditions();
  Init_Clocks();
  Init_Timers();
  Init_Serial();
  Init_LCD();
  Clear_Variables();
  enable_interrupts();




  splash_screen();                // 5-second intro
  display_baud();                 // show initial baud on line 3
  ui_show_waiting();              // line 1: "Waiting", clear line 2/4

  while (ALWAYS) {
    Clear_Variables();
    menu();                       // foreground: RX display + SW toggle/echo
  }
}
