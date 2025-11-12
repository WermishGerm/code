/*
 * transmit.c - Fixed version
 *  Project 8 UI behavior:
 *    - Line 1: "Waiting" / "Received" / "Transmit"
 *    - Line 2: Transmitted command (when transmitting)
 *    - Line 3: Baud value
 *    - Line 4: Received command
 */

#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include <string.h>
#include "ports.h"

extern char  display_line[4][11];
extern char* display[4];
extern volatile unsigned char update_display;
extern volatile unsigned char display_changed;
extern unsigned int Second_Count;

extern volatile unsigned char hw8_sw1_pressed;
extern volatile unsigned char hw8_sw2_pressed;

// from serial.c
void USB_tx(char* s);
void AD2_tx(const char* s);
char* USB_rx(void);

// Persistent state
static unsigned char baud_460800 = 0;
static char last_received[11] = "";

//------------------------------------------------------------------------------
// LCD Helper Functions
//------------------------------------------------------------------------------
static void padcpy(char *dest, const char *src) {
  const unsigned int LCDW = 10;
  unsigned int i = 0;

  // Copy up to LCDW chars
  while (i < LCDW && src && src[i] && src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }

  // Pad with spaces
  while (i < LCDW) {
    dest[i++] = ' ';
  }
  dest[LCDW] = '\0';
}

static void lcd_push(void) {
  display_changed = TRUE;
  update_display = TRUE;
  Display_Update(0, 0, 0, 0);
}

static void lcd_clear_all(void) {
  int i;
  for (i = 0; i < 4; i++) {
    padcpy(display_line[i], "");
  }
}

//------------------------------------------------------------------------------
// Public: Show "Waiting" idle state
//------------------------------------------------------------------------------
void ui_show_waiting(void) {
  padcpy(display_line[0], " Waiting  ");
  padcpy(display_line[1], "");
  // Line 3 shows baud (set by display_baud)
  padcpy(display_line[3], "");
  lcd_push();
}

//------------------------------------------------------------------------------
// Splash Screen (5 seconds)
//------------------------------------------------------------------------------
void splash_screen(void) {
  lcd_clear_all();
  padcpy(display_line[0], " Project 8");
  padcpy(display_line[1], "  Serial  ");
  padcpy(display_line[2], "  NCSU #1 ");
  padcpy(display_line[3], "Loading...");
  lcd_push();

  Second_Count = 0;
  while (Second_Count < FIVE_SECONDS);

  lcd_clear_all();
  lcd_push();
}

//------------------------------------------------------------------------------
// Display Current Baud Rate on Line 3
//------------------------------------------------------------------------------
void display_baud(void) {
  padcpy(display_line[2], baud_460800 ? " 460800   " : " 115200   ");
  display_changed = TRUE;
  update_display = TRUE;
  Display_Update(0, 0, 0, 0);
}

//------------------------------------------------------------------------------
// Apply Baud Rate to Hardware
//------------------------------------------------------------------------------
static void apply_baud(void) {
  // Put both UARTs in reset
  UCA0CTLW0 |= UCSWRST;
  UCA1CTLW0 |= UCSWRST;

  if (baud_460800) {
    // 460800 baud: 8MHz / 460800 = 17.36
    UCA0BRW = 17;
    UCA1BRW = 17;
    UCA0MCTLW = 0x4A00;
    UCA1MCTLW = 0x4A00;
  } else {
    // 115200 baud: 8MHz / 115200 = 69.44
    UCA0BRW = 4;
    UCA1BRW = 4;
    UCA0MCTLW = 0x5551;
    UCA1MCTLW = 0x5551;
  }

  // Take UARTs out of reset
  UCA0CTLW0 &= ~UCSWRST;
  UCA1CTLW0 &= ~UCSWRST;
}

//------------------------------------------------------------------------------
// Toggle Baud Rate (Button 2 function)
//------------------------------------------------------------------------------
void toggle_baud(void) {
  // Toggle baud rate
  baud_460800 ^= 1;

  // Apply new baud rate
  apply_baud();

  // Update display to show new baud
  display_baud();
}

//------------------------------------------------------------------------------
// Main Menu Loop - Handles RX and Button Processing
//------------------------------------------------------------------------------
void menu(void) {
  static char rx_buf[11];
  static unsigned char rx_idx = 0;

  // Process incoming characters from AD2 (via UCA0)
  char *rx = USB_rx();
  if (rx) {
    char c = rx[0];

    // Check for message terminator or buffer full
    if (c == '\r' || c == '\n' || rx_idx >= 10) {
      if (rx_idx > 0) {  // Only process if we have data
        rx_buf[rx_idx] = '\0';

        // Save this as the last received message
        strcpy(last_received, rx_buf);

        // Update display: "Received" on line 1, message on line 4
        padcpy(display_line[0], " Received ");
        padcpy(display_line[1], "");
        padcpy(display_line[3], rx_buf);
        lcd_push();

        rx_idx = 0;  // Reset for next message
      }
    } else if (c >= 0x20 && c <= 0x7E) {  // Printable ASCII only
      rx_buf[rx_idx++] = c;
    }
  }

  //----------------------------------------------------------------------------
  // SW1 Pressed: Echo last received command back to AD2
  //----------------------------------------------------------------------------
  if (hw8_sw1_pressed) {
    hw8_sw1_pressed = 0;  // Clear flag

    if (last_received[0] != '\0') {  // Only transmit if we have something
      // Update display: "Transmit" on line 1, command on line 2
      padcpy(display_line[0], " Transmit ");
      padcpy(display_line[1], last_received);
      padcpy(display_line[3], "");  // Clear line 4
      lcd_push();

      // Actually transmit via UCA0 (AD2)
      AD2_tx(last_received);
      AD2_tx("\r\n");  // Add line ending

      // Small delay to show "Transmit" state
      volatile unsigned int delay;
      for (delay = 0; delay < 50000; delay++);

      // Return to waiting state
      ui_show_waiting();
    }
  }

  //----------------------------------------------------------------------------
  // SW2 Pressed: Toggle Baud Rate
  //----------------------------------------------------------------------------
  if (hw8_sw2_pressed) {
    hw8_sw2_pressed = 0;  // Clear flag
    toggle_baud();
  }
}
