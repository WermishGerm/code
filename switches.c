/*
 * switches.c
 *  Hybrid ISR debounce for car + HW08 flags for transmit.c
 */

#include "msp430.h"
#include "ports.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "macros.h"

// Debounce state (existing)
volatile unsigned char sw1_position;
volatile unsigned char sw2_position;

// HW08 flags read by transmit.c
volatile unsigned char hw8_sw1_pressed = 0;
volatile unsigned char hw8_sw2_pressed = 0;

// SW1 (P4.0)
// --- SW1 (P4.1) ---
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
  if (P4IFG & SW1) {
    P4IE  &= ~SW1;         // disable during debounce
    P4IFG &= ~SW1;         // ack
    sw1_position    = TRUE;
    hw8_sw1_pressed = 1;   // <-- foreground will act on this

    TB2CCTL1 &= ~CCIFG;
    TB2CCR1   = TB2R + TB2CCR1_INTERVAL;
    TB2CCTL1 |= CCIE;
  }
}

// SW2 (P2.3)
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void) {
  if (P2IFG & SW2) {
    P6OUT ^= GRN_LED;
    P2IE  &= ~SW2;
    P2IFG &= ~SW2;
    sw2_position    = TRUE;
    hw8_sw2_pressed = 1;

    TB2CCTL1 &= ~CCIFG;
    TB2CCR1   = TB2R + TB2CCR1_INTERVAL;
    TB2CCTL1 |= CCIE;
  }
}



void Clear_Variables(void){
  // --- Clear all port interrupt flags ---
  P1IFG = 0;
  P2IFG = 0;
  P3IFG = 0;
  P4IFG = 0;
  P5IFG = 0;
  P6IFG = 0;

  // --- Clear switch-specific flags again (belt-and-suspenders) ---
  P4IFG &= ~SW1;   // clear any stale SW1 edge
  P2IFG &= ~SW2;   // clear any stale SW2 edge

  // --- Reset software-level variables ---
  hw8_sw1_pressed = 0;
  hw8_sw2_pressed = 0;
  sw1_position    = 0;
  sw2_position    = 0;

  // --- Re-enable clean edge detection for next press ---
  P4IES |= SW1;    // detect next press (highlow)
  P2IES |= SW2;    // detect next press (highlow)
  P4IE  |= SW1;    // ensure interrupt enabled
  P2IE  |= SW2;
}
