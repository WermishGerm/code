/*
 * timers.c
 *  Final version – Homework 08 safe with car project
 *  TB0 : LCD / Second_Count / delay timing
 *  TB2 : switch debounce (independent of car’s TB1)
 */

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "macros.h"
#include "ports.h"

//------------------------------------------------------------------------------
// Global counters
//------------------------------------------------------------------------------
volatile unsigned int timer_counter = 0;
volatile unsigned int LCD_counter   = 0;
volatile unsigned int Second_Count  = 0;
extern unsigned int delay_start;

// switch state flags come from switches.c
extern unsigned char sw1_position;
extern unsigned char sw2_position;

//------------------------------------------------------------------------------
//  TB0 : main system timer (unchanged)
//------------------------------------------------------------------------------
void Init_Timers(void){
    Init_Timer_B0();
    //Init_Timer_B1();
    Init_Timer_B2();  // <-- ADD THIS LINE
    //Init_Timer_B3();
}

//------------------------------------------------------------------------------
void Init_Timer_B0(void){
  TB0CTL   = TBSSEL__SMCLK | TBCLR | MC__CONTINOUS;   // SMCLK, continuous
  TB0CTL  |= ID__2;                                   // /2
  TB0EX0   = TBIDEX__8;                               // /8
  TB0CCR0  = TB0CCR0_INTERVAL;
  TB0CCTL0 = CCIE;                                    // enable CCR0 interrupt
  TB0CTL  &= ~TBIE;                                   // disable overflow
  TB0CTL  &= ~TBIFG;                                  // clear flag
}

//------------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
  timer_counter++;
  LCD_counter++;
  Second_Count++;

  if (delay_start > 0){
    delay_start--;
  }
  TB0CCR0 += TB0CCR0_INTERVAL;    // schedule next interrupt
}

//------------------------------------------------------------------------------
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
  switch(__even_in_range(TB0IV,14)){
  case 0: break;
  case 2: TB0CCR1 += TB0CCR1_INTERVAL; break;
  case 4: TB0CCR2 += TB0CCR2_INTERVAL; break;
  case 14: break;
  default: break;
  }
}

//------------------------------------------------------------------------------
//  TB2 : dedicated debounce timer (Homework 08 switches)
//------------------------------------------------------------------------------
void Init_Timer_B2(void) {
  TB2CTL = TBSSEL__SMCLK;    // SMCLK source (8 MHz)
  TB2CTL |= MC__CONTINUOUS;  // Continuous mode
  TB2CTL |= TBCLR;           // Clear timer

  // CCR1 is used for switch debouncing
  // Will be configured in switch ISR when needed
  TB2CCTL1 = 0;              // Start disabled

  // No interrupt enabled yet - switches will enable when needed
}
//------------------------------------------------------------------------------
//  TB2 CCR1 ISR – re-enable switches after debounce
//------------------------------------------------------------------------------
#pragma vector = TIMER2_B1_VECTOR
__interrupt void Timer2_B1_ISR(void){
  switch (__even_in_range(TB2IV,14)){
  case 2:  // CCR1
    if (sw1_position){
      sw1_position = FALSE;
      P4IFG &= ~SW1;
      P4IES |= SW1;   // always detect next press (highlow)
      P4IE  |= SW1;
    }
    if (sw2_position){
      sw2_position = FALSE;
      P2IFG &= ~SW2;
      P2IES |= SW2;    // always trigger on press (highlow)
      P2IE  |= SW2;
    }
    TB2CCTL1 &= ~CCIFG;            // clear flag, keep CCIE set
    break;
  default:
    break;
  }
}
