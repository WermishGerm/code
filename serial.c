/*
 * serial.c
 *  Project 8: Modified for message buffering (10 chars or CR-terminated)
 */
#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "ports.h"

//------------------------------------------------------------------------------
// Message buffers for UCA0 (AD2) and UCA1 (PC)
//------------------------------------------------------------------------------
static char uca0_rx_buffer[11];           // AD2 messages (10 chars + null)
static unsigned char uca0_rx_index = 0;   // Current buffer position
static unsigned char uca0_rx_ready = 0;   // Message complete flag

static char uca1_rx_buffer[11];           // PC messages (10 chars + null)
static unsigned char uca1_rx_index = 0;   // Current buffer position
static unsigned char uca1_rx_ready = 0;   // Message complete flag

//------------------------------------------------------------------------------
// Public init
//------------------------------------------------------------------------------
void Init_Serial(void){
  Init_Serial_UCA0();
  Init_Serial_UCA1();
}

//------------------------------------------------------------------------------
// UCA0 : AD2 / IoT channel (P1.6 RX, P1.7 TX) -> J9 connector
//------------------------------------------------------------------------------
void Init_Serial_UCA0(void){
  UCA0CTLW0 = UCSWRST;           // hold in reset
  UCA0CTLW0 |= UCSSEL__SMCLK;    // SMCLK
  UCA0BRW   = 4;                 // 115200 @ 8 MHz
  UCA0MCTLW = 0x5551;
  UCA0CTLW0 &= ~UCSWRST;         // release
  UCA0IE   |= UCRXIE;            // RX interrupt enable
  UCA0IFG  &= ~UCRXIFG;          // clear stray RX flag

  // Clear buffer
  uca0_rx_index = 0;
  uca0_rx_ready = 0;
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
  UCA1IFG  &= ~UCRXIFG;          // clear stray RX flag

  // Clear buffer
  uca1_rx_index = 0;
  uca1_rx_ready = 0;
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
// ISR: UCA0 (AD2 / IoT) – Accumulate incoming message
//------------------------------------------------------------------------------
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
  switch(__even_in_range(UCA0IV,0x08)){
    case 0x02: {                          // RXIFG
      unsigned char rx_char = UCA0RXBUF;

      // Check for message terminator (CR or LF) or 10 characters
      if (rx_char == '\r' || rx_char == '\n') {
        if (uca0_rx_index > 0) {          // Only process if we have data
          uca0_rx_buffer[uca0_rx_index] = '\0';  // Null terminate
          uca0_rx_ready = 1;              // Signal complete message
          uca0_rx_index = 0;              // Reset for next message
        }
      }
      else if (uca0_rx_index < 10) {      // Room in buffer
        uca0_rx_buffer[uca0_rx_index++] = rx_char;

        // Check if we've hit 10 characters
        if (uca0_rx_index >= 10) {
          uca0_rx_buffer[uca0_rx_index] = '\0';
          uca0_rx_ready = 1;
          uca0_rx_index = 0;
        }
      }
      else {
        // Buffer overflow protection - reset
        uca0_rx_index = 0;
      }
    } break;

    default: break;
  }
}

//------------------------------------------------------------------------------
// ISR: UCA1 (PC) – Accumulate incoming message
//------------------------------------------------------------------------------
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void) {
  switch(__even_in_range(UCA1IV,0x08)) {
    case 0x02: {                          // RXIFG
      unsigned char rx_char = UCA1RXBUF;

      // Check for message terminator (CR or LF) or 10 characters
      if (rx_char == '\r' || rx_char == '\n') {
        if (uca1_rx_index > 0) {          // Only process if we have data
          uca1_rx_buffer[uca1_rx_index] = '\0';  // Null terminate
          uca1_rx_ready = 1;              // Signal complete message
          uca1_rx_index = 0;              // Reset for next message
        }
      }
      else if (uca1_rx_index < 10) {      // Room in buffer
        uca1_rx_buffer[uca1_rx_index++] = rx_char;

        // Check if we've hit 10 characters
        if (uca1_rx_index >= 10) {
          uca1_rx_buffer[uca1_rx_index] = '\0';
          uca1_rx_ready = 1;
          uca1_rx_index = 0;
        }
      }
      else {
        // Buffer overflow protection - reset
        uca1_rx_index = 0;
      }
    } break;

    default: break;
  }
}

//------------------------------------------------------------------------------
// Return pointer to received AD2 message (or NULL if none ready)
// NOTE: For Project 8, you're using AD2 on UCA0, so this is the one to use
//------------------------------------------------------------------------------
char *USB_rx(void) {
  if (uca0_rx_ready) {
    uca0_rx_ready = 0;
    return uca0_rx_buffer;
  }
  return (char*)0;  // NULL
}

//------------------------------------------------------------------------------
// Alternative: Return PC message if needed for debugging
//------------------------------------------------------------------------------
char *PC_rx(void) {
  if (uca1_rx_ready) {
    uca1_rx_ready = 0;
    return uca1_rx_buffer;
  }
  return (char*)0;  // NULL
}
