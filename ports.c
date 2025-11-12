/*
 * ports.c
 *
 * Created on: Sep 10, 2025
 * Author: Juan Contreras
 * Ports.c intizalies the ports we will be using in
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"
#include "ports.h"

void Init_Ports(void){ //This calls the rest of the initialization functions
    // Disable all port interrupts during initialization
    P1IE = 0;
    P2IE = 0;
    P3IE = 0;
    P4IE = 0;
    P5IE = 0;
    P6IE = 0;

Init_Port_1();
Init_Port_2();
Init_Port_3();
Init_Port_4();
Init_Port_5();
Init_Port_6();

}
void Init_Port_1(void){ //This function will initialize all pins in port 1.
 P1OUT = 0x00;
 P1DIR = 0x00;
 P1SEL0 &= ~RED_LED;    // Set P1.0 as GPIO
 P1SEL1 &= ~RED_LED;    // Set P1.0 as GPIO
 P1OUT &= ~RED_LED;     // Initialize RED_LED low
 P1DIR |= RED_LED;      // Set P1.0 as output

 P1SEL0 &= ~A1_SEEED;   // Set P1.1 as GPIO
 P1SEL1 &= ~A1_SEEED;   // Set P1.1 as GPIO
 P1OUT &= ~A1_SEEED;    // Initialize A1 low
 P1DIR |= A1_SEEED;     // Set P1.1 as output

// --- START: ADC PINS CONFIGURATION (V_DETECT_L, V_DETECT_R, V_THUMB) ---
 P1SEL0 |= V_DETECT_L;  // Set P1.2 to Analog
 P1SEL1 |= V_DETECT_L;  //
 P1OUT &= ~V_DETECT_L;  // Initial Value = Low (no pullup)
 P1DIR &= ~V_DETECT_L;  // Direction = input (for ADC)
 P1REN &= ~V_DETECT_L;  // Disable Pullup/Pulldown

 P1SEL0 |= V_DETECT_R;  // Set P1.3 to Analog
 P1SEL1 |= V_DETECT_R;  //
 P1OUT &= ~V_DETECT_R;  // Initial Value = Low (no pullup)
 P1DIR &= ~V_DETECT_R;  // Direction = input (for ADC)
 P1REN &= ~V_DETECT_R;  // Disable Pullup/Pulldown

 P1SEL0 &= ~A4_SEEED;   // Set P1.4 as GPIO
 P1SEL1 &= ~A4_SEEED;   // Set P1.4 as GPIO
 P1OUT &= ~A4_SEEED;    // Initialize A4 low
 P1DIR |= A4_SEEED;     // Set P1.4 as output

 P1SEL0 |= V_THUMB;     // Set P1.5 to Analog
 P1SEL1 |= V_THUMB;     //
 P1OUT &= ~V_THUMB;     // Initial Value = Low (no pullup)
 P1DIR &= ~V_THUMB;     // Direction = input (for ADC)
 P1REN &= ~V_THUMB;     // Disable Pullup/Pulldown
// --- END: ADC PINS CONFIGURATION ---
 P1SEL0 |= (BIT6 | BIT7);
 P1SEL1 &= ~(BIT6 | BIT7);
}

void Init_Port_2(void){ // Configure Port 2
//------------------------------------------------------------------------------
 P2OUT = 0x00; // P2 set Low
 P2DIR = 0x00; // Set P2 direction to output

 P2SEL0 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
 P2SEL1 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
 P2OUT &= ~SLOW_CLK; // Initial Value = Low / Off
 P2DIR |= SLOW_CLK; // Direction = output

 P2SEL0 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
 P2SEL1 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
 P2OUT &= ~CHECK_BAT; // Initial Value = Low / Off
 P2DIR |= CHECK_BAT; // Direction = output

 P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
 P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
 P2OUT |= IR_LED; // Initial Value = High / ON [CHANGE: Set HIGH as requested]
 P2DIR |= IR_LED; // Direction = output

 //------------------------------------------------------------------------------
 // Port 2 configuration
 //------------------------------------------------------------------------------
 P2SEL0 &= ~SW2;
 P2SEL1 &= ~SW2;
 P2OUT  |= SW2;
 P2DIR  &= ~SW2;
 P2REN  |= SW2;
 P2IES |= SW2;   // start detecting press (highlow)
 P2IFG  &= ~SW2;
 P2IE   |= SW2;


 P2SEL0 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
 P2SEL1 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
 P2OUT &= ~IOT_RUN_CPU; // Initial Value = Low / Off
 P2DIR |= IOT_RUN_CPU; // Direction = output

 P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
 P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
 P2OUT |= DAC_ENB; // Initial Value = High
 P2DIR |= DAC_ENB; // Direction = output

 P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
 P2SEL1 |= LFXOUT; // LFXOUT Clock operation
 P2SEL0 &= ~LFXIN; // LFXIN Clock operation
 P2SEL1 |= LFXIN; // LFXIN Clock operation
 //------------------------------------------------------------------------------
}

void Init_Port_3(void){
P3OUT = 0x00;
P3DIR = 0x00;
P3SEL0 &= ~TEST_PROBE;
P3SEL1 &= ~TEST_PROBE;
P3OUT &= ~TEST_PROBE;
P3DIR |= TEST_PROBE;

P3SEL0 &= ~DAC_CTRL_2;
P3SEL1 &= ~DAC_CTRL_2;
P3OUT &= ~DAC_CTRL_2;
P3DIR |= DAC_CTRL_2;

P3SEL0 &= ~OA2N;
P3SEL1 &= ~OA2N;
P3OUT &= ~OA2N;
P3DIR |= OA2N;

P3SEL0 &= ~OA2P;
P3SEL1 &= ~OA2P;
P3OUT &= ~OA2P;
P3DIR |= OA2P;

P3SEL0 &= ~SMCLK_OUT;
P3SEL1 &= ~SMCLK_OUT;
P3OUT &= ~SMCLK_OUT;
P3DIR |= SMCLK_OUT;

P3SEL0 &= ~DAC_CTRL_3;
P3SEL1 &= ~DAC_CTRL_3;
P3OUT &= ~DAC_CTRL_3;
P3DIR |= DAC_CTRL_3;

P3SEL0 &= ~IOT_LINK_CPU;
P3SEL1 &= ~IOT_LINK_CPU;
P3OUT &= ~IOT_LINK_CPU;
P3DIR |= IOT_LINK_CPU;

P3SEL0 &= ~IOT_RN_CPU;
P3SEL1 &= ~IOT_RN_CPU;
P3OUT &= ~IOT_RN_CPU;
P3DIR |= IOT_RN_CPU;

}

void Init_Port_4(void){ // Configure PORT 4
//------------------------------------------------------------------------------
 P4OUT = 0x00; // P4 set Low
 P4DIR = 0x00; // Set P4 direction to output
 P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
 P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
 P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
 P4DIR |= RESET_LCD; // Direction = output

 //------------------------------------------------------------------------------
 // Port 4 configuration
 //------------------------------------------------------------------------------
 P4SEL0 &= ~SW1;                         //SW1 G I/O operation
 P4SEL1 &= ~SW1;                         //SW1 G I/O operation
 P4OUT |= SW1;                           //Set SW1 High
 P4DIR &= ~SW1;                          //Set SW1 Input
 P4REN |= SW1;                           //Set SW1 Pull up resistor
 P4IES &= ~SW1;   // SW1 interrupt edge: low-to-high
 P4IE  |= SW1;                           //SW1 interrupt Enabled


 P4SEL0 |= (BIT2 | BIT3);
  P4SEL1 &= ~(BIT2 | BIT3);

 P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
 P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
 P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
 P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

 P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
 P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

 P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
 P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

 P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
 P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
 //------------------------------------------------------------------------------
}
void Init_Port_5(void){
// --- START: ADC PINS CONFIGURATION (V_BAT, V_5, V_DAC, V3_3) ---
P5SEL0 |= V_BAT;
P5SEL1 |= V_BAT;
P5OUT &= ~V_BAT;
P5DIR &= ~V_BAT;
P5REN &= ~V_BAT;

P5SEL0 |= V_5;
P5SEL1 |= V_5;
P5OUT &= ~V_5;
P5DIR &= ~V_5;
P5REN &= ~V_5;

P5SEL0 |= V_DAC;
P5SEL1 |= V_DAC;
P5OUT &= ~V_DAC;
P5DIR &= ~V_DAC;
P5REN &= ~V_DAC;

P5SEL0 |= V3_3;
P5SEL1 |= V3_3;
P5OUT &= ~V3_3;
P5DIR &= ~V3_3;
P5REN &= ~V3_3;
// --- END: ADC PINS CONFIGURATION ---

P5SEL0 &= ~IOT_BOOT_CPI;
P5SEL1 &= ~IOT_BOOT_CPI;
P5OUT &= ~IOT_BOOT_CPI;
P5DIR |= IOT_BOOT_CPI;
}

void Init_Port_6(void){
P6SEL0 &= ~L_FORWARD;
P6SEL1 &= ~L_FORWARD;
P6OUT &= ~L_FORWARD;
P6DIR |= L_FORWARD;
P6SEL0 &= ~R_FORWARD;
P6SEL1 &= ~R_FORWARD;
P6OUT &= ~R_FORWARD;
P6DIR |= R_FORWARD;
P6SEL0 &= ~L_REVERSE;
P6SEL1 &= ~L_REVERSE;
P6OUT &= ~L_REVERSE;
P6DIR |= L_REVERSE;
P6SEL0 &= ~R_REVERSE;
P6SEL1 &= ~R_REVERSE;
P6OUT &= ~R_REVERSE;
P6DIR |= R_REVERSE;
P6SEL0 &= ~LCD_BACKLITE;
P6SEL1 &= ~LCD_BACKLITE;
P6OUT |= LCD_BACKLITE;
P6DIR |= LCD_BACKLITE;
P6SEL0 &= ~P6_5;
P6SEL1 &= ~P6_5;
P6OUT &= ~P6_5;
P6DIR |= P6_5;
P6SEL0 &= ~GRN_LED;
P6SEL1 &= ~GRN_LED;
P6OUT &= ~GRN_LED;
P6DIR |= GRN_LED;
}
