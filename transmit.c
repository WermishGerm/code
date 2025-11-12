/*
 * transmit.c
 *  Project 8 UI behavior:
 *    - Line 1: "Waiting" / "Received" / "Transmit"
 *    - Line 3: Baud value
 *    - On RX complete (10 chars or CR): show message on Line 4
 *    - SW1: echo last message to AD2 (UCA0), move it to Line 2
 *    - SW2: toggle baud (115200 <-> 460800) and update Line 3
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

// persistent baud state
static unsigned char baud_460800 = 0;

// local lcd helpers
static void padcpy(char *dest, const char *src){
  const unsigned int LCDW = 10;
  unsigned int i=0;
  // copy up to LCDW chars
  while(i<LCDW && src && src[i] && src[i]!='\0'){ dest[i]=src[i]; i++; }
  // pad with spaces
  while(i<LCDW){ dest[i++]=' '; }
  dest[LCDW]='\0';
}

static void lcd_push(void){ Display_Update(0,0,0,0); }
static void lcd_clear_all(void){ int i; for(i=0;i<4;i++) padcpy(display_line[i],""); }

// public: show "Waiting" idle label
void ui_show_waiting(void){
  padcpy(display_line[0], " Waiting ");
  padcpy(display_line[1], "");
  // line3 gets set by display_baud
  padcpy(display_line[3], "");     // clear line4
  lcd_push();
}

// Splash (unchanged timing, new strings)
void splash_screen(void){
  lcd_clear_all();
  padcpy(display_line[1],"  NCSU  #1 ");
  padcpy(display_line[2],"   Splash  ");
  padcpy(display_line[3],"  Loading..");
  lcd_push();

  Second_Count = 0;
  while (Second_Count < FIVE_SECONDS);
  lcd_clear_all();
  lcd_push();
}

// Baud display + toggle
void display_baud(void){
  padcpy(display_line[2]," Baud:");
  padcpy(display_line[3], baud_460800 ? " 460,800 " : " 115,200 ");
  display_changed = TRUE;      // <-- tell Display_Process LCD data changed
  update_display = TRUE;       // <-- allow Display_Process to run once
  Display_Update(0,0,0,0);     // or just call lcd_push() after this
}

static void apply_baud(void){
  if (baud_460800){
    UCA0BRW=17; UCA1BRW=17;      // 460,800
    UCA0MCTLW=0x4A00; UCA1MCTLW=0x4A00;
  } else {
    UCA0BRW=4;  UCA1BRW=4;       // 115,200
    UCA0MCTLW=0x5551; UCA1MCTLW=0x5551;
  }
}

void toggle_baud(void){
  baud_460800 ^= 1;
  apply_baud();
  UCA0CTLW0 |= UCSWRST;
  UCA1CTLW0 |= UCSWRST;

  if (baud_460800) {
    UCA0BRW = 17; UCA1BRW = 17;
    UCA0MCTLW = 0x4A00; UCA1MCTLW = 0x4A00;
  } else {
    UCA0BRW = 4;  UCA1BRW = 4;
    UCA0MCTLW = 0x5551; UCA1MCTLW = 0x5551;
  }

  UCA0CTLW0 &= ~UCSWRST;
  UCA1CTLW0 &= ~UCSWRST;

  // ---- show "Transmitting" on LCD ----
  padcpy(display_line[0], " Transmit ");
  padcpy(display_line[1], "NCSU  #1");
  display_changed = TRUE;
  update_display  = TRUE;
  Display_Update(0,0,0,0);

  // ---- actually send the string ----
  USB_tx("NCSU  #1\r\n");   // add CRLF for readability on PC side

  // ---- refresh baud display below ----
  display_baud();
}


//------------------------------------------------------------------------------
// Foreground loop: accumulate PC input and drive UI
//------------------------------------------------------------------------------
void menu(void){
  static char rx_buf[11];
  static char last_msg[11];
  static unsigned char idx = 0;
  static unsigned char init_done = 0;
  Clear_Variables();

  // Handle inbound chars from PC (USB backchannel)
  char *rx = USB_rx();
  if (rx){
    char c = rx[0];
    if (c == '\r' || idx >= 10){
      // complete message
      rx_buf[idx] = '\0';

      // UI: show "Received" and put message on line 4
      padcpy(display_line[0], " Received ");   // line 1
      padcpy(display_line[1], "          ");
      padcpy(display_line[3], rx_buf);         // line 4
      padcpy(last_msg, rx_buf);                // save for echo
      lcd_push();

      // return to idle "Waiting" after short activity? optional; we keep "Received" until next event

      idx = 0; // reset for next message
    } else if (c != '\n'){
      rx_buf[idx++] = c;
    }
  }

  // SW1 -> echo to AD2 (UCA0), show "Transmit", move command to line 2
  if (hw8_sw1_pressed){
      hw8_sw1_pressed = 0;
       USB_tx("NCSU #1");  // your transmit function
    }


  // SW2 -> toggle baud + update line 3
  if (hw8_sw2_pressed){
    hw8_sw2_pressed = 0;
    toggle_baud();
  }
}
