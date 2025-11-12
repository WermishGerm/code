#include "msp430.h"
#include "ports.h"
#include "functions.h"
#include "LCD.h"
#include "macros.h"

unsigned int ADC_Left_Detect = 0;
unsigned int ADC_Right_Detect = 0;
unsigned int ADC_Thumb = 0;

volatile unsigned int check_line_flag = 0;  // L/R updated
volatile unsigned char adc_stage = 0;        // 0 = Thumb, 1 = Right, 2 = Left

void Init_ADC(void){
    // Configure ADC
    ADCCTL0 = ADCSHT_2 | ADCON | ADCMSC;       // 16 clocks, on, multi-sample
    ADCCTL1 = ADCSHP | ADCCONSEQ_0;            // SAMPCON timer, single-conversion
    ADCCTL2 = ADCRES_2;                        // 12-bit
    ADCMCTL0 = ADCINCH_5 | ADCSREF_0;          // Start Thumb wheel
    ADCIE |= ADCIE0;
    ADCCTL0 |= ADCENC | ADCSC;                 // Start first conversion
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    ADCCTL0 &= ~ADCENC; // Disable conversion temporarily

    switch(adc_stage){
        case 0: // Thumb wheel
            ADC_Thumb = ADCMEM0;
            adc_stage = 1;
            ADCMCTL0 = (ADCMCTL0 & 0xF0) | ADCINCH_3; // Right sensor
            break;

        case 1: // Right sensor
            ADC_Right_Detect = ADCMEM0;
            adc_stage = 2;
            ADCMCTL0 = (ADCMCTL0 & 0xF0) | ADCINCH_2; // Left sensor
            break;

        case 2: // Left sensor
            ADC_Left_Detect = ADCMEM0;
            check_line_flag = 1; // Flag for line detection
            adc_stage = 0;
            ADCMCTL0 = (ADCMCTL0 & 0xF0) | ADCINCH_5; // Thumb wheel
            break;
    }

    ADCCTL0 |= ADCENC | ADCSC; // Start next conversion
}
