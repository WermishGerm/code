/*
 * switches.c
 *  Fixed ISR debounce for car + HW08 flags for transmit.c
 */

#include "msp430.h"
#include "ports.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "macros.h"

// Debounce state
volatile unsigned char sw1_position = 0;
volatile unsigned char sw2_position = 0;

// HW08 flags read by transmit.c
volatile unsigned char hw8_sw1_pressed = 0;
volatile unsigned char hw8_sw2_pressed = 0;

// Debounce tracking
static volatile unsigned char sw1_debouncing = 0;
static volatile unsigned char sw2_debouncing = 0;

//------------------------------------------------------------------------------
// SW1 Interrupt (P4.1)
//------------------------------------------------------------------------------
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
  if (P4IFG & SW1) {
    P4IFG &= ~SW1;         // Clear flag immediately

    if (!sw1_debouncing) {
      sw1_debouncing = 1;
      P4IE &= ~SW1;        // Disable interrupt during debounce

      // Set flag for foreground
      sw1_position = 1;
      hw8_sw1_pressed = 1;

      // Start debounce timer
      TB2CCR1 = TB2R + TB2CCR1_INTERVAL;
      TB2CCTL1 &= ~CCIFG;
      TB2CCTL1 |= CCIE;
    }
  }
}

//------------------------------------------------------------------------------
// SW2 Interrupt (P2.3)
//------------------------------------------------------------------------------
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void) {
  if (P2IFG & SW2) {
    P2IFG &= ~SW2;         // Clear flag immediately

    if (!sw2_debouncing) {
      sw2_debouncing = 1;
      P2IE &= ~SW2;        // Disable interrupt during debounce

      // Set flag for foreground
      sw2_position = 1;
      hw8_sw2_pressed = 1;

      // Start debounce timer (reuse same timer)
      TB2CCR1 = TB2R + TB2CCR1_INTERVAL;
      TB2CCTL1 &= ~CCIFG;
      TB2CCTL1 |= CCIE;
    }
  }
}


//------------------------------------------------------------------------------
// Clear all switch variables and flags
//------------------------------------------------------------------------------
void Clear_Variables(void) {
  // Clear all port interrupt flags
  P1IFG = 0;
  P2IFG = 0;
  P3IFG = 0;
  P4IFG = 0;
  P5IFG = 0;
  P6IFG = 0;

  // Reset software flags
  hw8_sw1_pressed = 0;
  hw8_sw2_pressed = 0;
  sw1_position = 0;
  sw2_position = 0;
  sw1_debouncing = 0;
  sw2_debouncing = 0;

  // Ensure switches are configured for press detection (high-to-low)
  P4IES |= SW1;    // High-to-low edge
  P2IES |= SW2;    // High-to-low edge

  // Re-enable interrupts
  P4IE |= SW1;
  P2IE |= SW2;
}
