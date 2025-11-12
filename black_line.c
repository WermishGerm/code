#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "LCD.h"
#include "ports.h"


extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int ADC_Thumb;
extern unsigned int delay_start;
extern char display_line[4][11];
extern volatile unsigned int check_line_flag;

extern volatile unsigned char display_changed;

unsigned char search_state = STOPALL;
volatile unsigned int reverse_count;
unsigned int detected;
unsigned int reverse_flag;


char adc_char[4];
int i;

void HEXtoBCD(int hex_value);

void blackline_state(void){
  switch (search_state) {
  case SEARCH: search_line();        break;
  case STOP:   stop_on_line();       break;
  case SECR:   reverse_to_line();    break;
  case STOPALL:
      stopall();
      strcpy(display_line[3], "  STOP     ");break;
    default: ;      break;
  }

  HEXtoBCD(ADC_Left_Detect);
  adc_line(3, 3);
  HEXtoBCD(ADC_Right_Detect);
  adc_line(3, 8);
}


void search_line(void) {
    strcpy(display_line[3], "  SEARCH   ");
    if(P2OUT & IR_LED){
    strcpy(display_line[0], "  IR: ON   ");
    } else {
    strcpy(display_line[0], "  IR: OFF  ");
    }
    if (delay_start > 0) {
        stopall();
        strcpy(display_line[3], " 1.0s DLY ");
        moveforward();
        return;
    }

    moveforward();

    if (check_line_flag) {
        check_line_flag = 0;
        P2OUT |= IR_LED;
        ADCCTL0 |= ADCSC;
    }

    if (ADC_Left_Detect >= LEFT_BLACK_THRESH || ADC_Right_Detect >= RIGHT_BLACK_THRESH) {
        search_state = STOP;
        delay_start = DETECT_WAIT_TICKS;
    }
}


void stop_on_line(void){
    stopall();
    strcpy(display_line[3], "  DETECTED ");
    if(IR_LED != 0){
        strcpy(display_line[0], "  IR: ON   ");
        } else {
        strcpy(display_line[0], "  IR: OFF  ");

        }
    if (delay_start > 0) {
        return;
    }

    search_state = SECR;
}


void reverse_to_line(void){
    strcpy(display_line[3], " ALIGNING  ");
    if(IR_LED != 0){
        strcpy(display_line[0], "  IR: ON   ");
        } else {
        strcpy(display_line[0], "  IR: OFF  ");

        }
    spin_clockwise();

    if (check_line_flag) {
        check_line_flag = 0;
        P2OUT |= IR_LED;
        ADCCTL0 |= ADCSC;
    }

    if (ADC_Left_Detect >= LEFT_BLACK_THRESH && ADC_Right_Detect >= RIGHT_BLACK_THRESH){
        search_state = STOPALL;
    }
}

void HEXtoBCD(int hex_value){
int value = 0;
for(i=0; i < 4; i++) {
adc_char[i] = '0';
}
while (hex_value > 999){
hex_value = hex_value - 1000;
value = value + 1;
adc_char[0] = 0x30 + value;
}
value = 0;
while (hex_value > 99){
hex_value = hex_value - 100;
value = value + 1;
adc_char[1] = 0x30 + value;
}
value = 0;
while (hex_value > 9){
hex_value = hex_value - 10;
value = value + 1;
adc_char[2] = 0x30 + value;
}
adc_char[3] = 0x30 + hex_value;
}

void adc_line(char line, char location){
int i;
unsigned int real_line;
real_line = line - 1;
for(i=0; i < 4; i++) {
display_line[real_line][i+location] = adc_char[i];
}
}

void Update_Blackline_Display(void){
    // Check if new ADC data is available (flag set by ADC ISR)
    if (check_line_flag) {
        check_line_flag = 0; // Clear the flag after processing
        int j;
        // --- 1. Update LEFT Detector Value (Line 2: display_line[1]) ---
        // Converts ADC value (e.g., 2048) into four BCD chars (e.g., {'2','0','4','8'})
        HEXtoBCD(ADC_Left_Detect);

        // Copy the 4 BCD characters into the display buffer (starting at index 3: " L: XXXX ")
        for( j=0; j<4; j++) {
            display_line[1][j+3] = adc_char[j];
        }

        // --- 2. Update RIGHT Detector Value (Line 3: display_line[2]) ---
        HEXtoBCD(ADC_Right_Detect);

        // Copy the 4 BCD characters into the display buffer (starting at index 3: " R: XXXX ")
        for( j=0; j<4; j++) {
            display_line[2][j+3] = adc_char[j];
        }

        // --- 3. Update Status (Line 4: display_line[3]) ---

        // Check for the initial 1.0 second delay status
        if (delay_start > 0) {
            strcpy(display_line[3], " 1.0s DLY "); // Status: 1.0s Delay
        }
        // If the delay is over, update the status based on the current state
        else {
            switch (search_state) {
                case SEARCH:
                    strcpy(display_line[3], "  SEARCH  "); // Status: Searching for line
                    break;
                case STOP:
                    strcpy(display_line[3], " DETECTED "); // Status: Black Line Detected
                    break;
                case SECR:
                    strcpy(display_line[3], "  ALIGN   "); // Status: Aligning to line
                    break;
                case STOPALL:
                    strcpy(display_line[3], "   STOP   "); // Status: Stopped
                    break;
                default:
                    // Use a blank string or a general status if needed
                    strcpy(display_line[3], "          ");
                    break;
            }
        }

        display_changed = TRUE; // Signal the main loop to run Display_Process
    }
}
