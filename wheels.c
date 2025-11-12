#include  "msp430.h"
#include "ports.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"


void stopall(void){
P6OUT &= ~L_FORWARD;
P6OUT &= ~R_FORWARD;
P6OUT &= ~L_REVERSE;
P6OUT &= ~R_REVERSE;
}

void moveforward(void){
stopall();
P6OUT |= L_FORWARD;
P6OUT |= R_FORWARD;
}

void movereverse(void){
stopall();
P6OUT |= L_REVERSE;
P6OUT |= R_REVERSE;
}

void spin_clockwise(void){
stopall();
P6OUT |= L_FORWARD;
P6OUT |= R_REVERSE;
}

void spin_counterclockwise(void){
stopall();
P6OUT |= R_FORWARD;
P6OUT |= L_REVERSE;
}
