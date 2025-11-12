/*
 * serial.c
 *  ECE 306 Homework 08 – UART setup + echo + minimal USB_rx buffer
 */

#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "ports.h"

//------------------------------------------------------------------------------
// Public init
//------------------------------------------------------------------------------
void Init_Serial(void){
  Init_Serial_UCA0();
  Init_Serial_UCA1();
}

//------------------------------------------------------------------------------
// UCA0 : IoT channel (P1.6 RX, P1.7 TX) -> AD2 / J9
//------------------------------------------------------------------------------
void Init_Serial_UCA0(void){
  UCA0CTLW0 = UCSWRST;           // hold in reset
  UCA0CTLW0 |= UCSSEL__SMCLK;    // SMCLK
  UCA0BRW   = 4;                 // 115200 @ 8 MHz
  UCA0MCTLW = 0x5551;
  UCA0CTLW0 &= ~UCSWRST;         // release
  UCA0IE   |= UCRXIE;            // RX interrupt enable
  UCA0IFG  &= ~UCRXIFG;          // clear stray RX flag ONLY
}

//------------------------------------------------------------------------------
// UCA1 : PC back-channel (P4.2 RX, P4.3 TX) -> USB
//------------------------------------------------------------------------------
void Init_Serial_UCA1(void){
  UCA1CTLW0 = UCSWRST;           // hold in reset
  UCA1CTLW0 |= UCSSEL__SMCLK;    // SMCLK
  UCA1BRW   = 4;                 // 115200 @ 8 MHz
  UCA1MCTLW = 0x5551;
  UCA1CTLW0 &= ~UCSWRST;         // release
  UCA1IE   |= UCRXIE;            // RX interrupt enable
  UCA1IFG  &= ~UCRXIFG;          // clear stray RX flag ONLY
}

//------------------------------------------------------------------------------
// TX helpers
//------------------------------------------------------------------------------
void USB_tx(char* command){
  int i;
  for (i=0; command[i] != '\0'; i++){
    while (!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = command[i];
  }
}

void AD2_tx(const char* command){
  int i;
  for (i=0; command[i] != '\0'; i++){
    while (!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = command[i];
  }
}

//------------------------------------------------------------------------------
// ISR: UCA0 (IoT) – forward to PC
//------------------------------------------------------------------------------
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
  switch(__even_in_range(UCA0IV,0x08)){
    case 0x02: {                    // RXIFG
      unsigned char b = UCA0RXBUF;
      while(!(UCA1IFG & UCTXIFG));
      UCA1TXBUF = b;                // forward to PC
    } break;
    default: break;
  }
}

//------------------------------------------------------------------------------
// Minimal single-char buffer for LCD display of PC input
//------------------------------------------------------------------------------
static char usb_buffer[20];
static unsigned char usb_ready = 0;

//------------------------------------------------------------------------------
// ISR: UCA1 (PC) – capture for LCD + forward to IoT
//------------------------------------------------------------------------------
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void) {
  switch(__even_in_range(UCA1IV,0x08)) {
    case 0x02: {                    // RXIFG
      unsigned char b = UCA1RXBUF;
      usb_buffer[0] = (char)b;      // store latest char
      usb_buffer[1] = '\0';
      usb_ready = 1;                // signal "new data"
      while(!(UCA0IFG & UCTXIFG));
      UCA0TXBUF = b;                // forward to IoT
    } break;
    default: break;
  }
}

//------------------------------------------------------------------------------
// Return pointer to latest char (or NULL if none)
//------------------------------------------------------------------------------
char *USB_rx(void) {
  if (usb_ready) {
    usb_ready = 0;
    return usb_buffer;
  }
  return 0;
}
