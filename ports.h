/*
 * ports.h
 *
 *  Created on: Sep 12, 2025
 *      Author: Juan Contreras
 *      PORT DEFINES USED FOR PORTS C.
 *      THE FOLLOWING ARE MADE WITH THE SCHEMATICS PROVIDED BY PROFESSOR CARLSON
 *
 */

#ifndef PORTS_H_
#define PORTS_H_

// Port 1 Pins
#define RED_LED    (0x01) // P1.0 Red LED
#define A1_SEEED   (0x02) // P1.1 A1 input
#define V_DETECT_L (0x04) // P1.2 Detect L
#define V_DETECT_R (0x08) // P1.3 Detect R
#define A4_SEEED   (0x10) // P1.4 A4 input
#define V_THUMB    (0x20) // P1.5 Thumbwheel
#define UCA0TXD    (0x40) // P1.6 UART TX
#define UCA0RXD    (0x80) // P1.7 UART RX

// Port 2 Pins
#define SLOW_CLK (0x01) // 0 SLOW_CLK
#define CHECK_BAT (0x02) // 1 Check ADC Voltages
#define IR_LED (0x04) // 2 IR_LED
#define SW2 (0x08) // 3 SW2
#define IOT_RUN_CPU (0x10) // 4 IOT_RUN_CPU
#define DAC_ENB (0x20) // 5 DAC_ENB
#define LFXOUT (0x40) // 6 XOUTR
#define LFXIN (0x80) // 7 XINR

// Port 3 Pins
#define TEST_PROBE    (0x01) // P3.0 Test probe
#define DAC_CTRL_2    (0x02) // P3.1 DAC control 2
#define OA2N          (0x04) // P3.2 OpAmp2- (IR LED)
#define OA2P          (0x08) // P3.3 OpAmp2+ (SW2)
#define SMCLK_OUT     (0x10) // P3.4 SMCLK out
#define DAC_CTRL_3    (0x20) // P3.5 DAC control 3
#define IOT_LINK_CPU  (0x40) // P3.6 IOT link
#define IOT_RN_CPU    (0x80) // P3.7 IOT reset

// Port 4 Pins
#define RESET_LCD (0x01) // 0 RESET_LCD
#define SW1 (0x02) // 1 SW1
#define UCA1RXD (0x04) // 2 Back Channel UCA1RXD
#define UCA1TXD (0x08) // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD (0x10) // 4 Chip Select
#define UCB1CLK (0x20) // 5 SPI mode - clock output—UCB1CLK
#define UCB1SIMO (0x40) // 6 UCB1SIMO
#define UCB1SOMI (0x80) // 7 UCB1SOMI

// Port 5 Pins
#define V_BAT (0x01) // P5.0 battery voltage
#define V_5 (0x02) // P5.1 5voltage
#define V_DAC (0x04) // P5.2 dac board voltage
#define V3_3 (0x08) // P5.3 3.3 voltage
#define IOT_BOOT_CPI (0x10) // P5.4

// Port 6 Pins
#define L_FORWARD (0x01) // P6.0
#define R_FORWARD (0x02) // P6.0
#define L_REVERSE (0x04) // P6.0
#define R_REVERSE (0x08) // P6.0
#define LCD_BACKLITE (0x10) // P6.0
#define P6_5 (0x20) // P6.0
#define GRN_LED (0x40) // P6.0

#endif /* PORTS_H_ */
