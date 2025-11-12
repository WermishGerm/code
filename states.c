/*
 * states.c
 *
 *  Created on: Sep 29, 2025
 *      Author: Juan Contreras
 *
 *  List of #defines used across the project
 *  (LEDs, test pins, constants, etc.)
 */
#include  "msp430.h"
#include "ports.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"

unsigned char state = WAIT;
unsigned int time_change;
unsigned int segment_count;
unsigned int right_motor_count;
unsigned int left_motor_count;
unsigned int delay_start;
unsigned int cycle_time;
unsigned char event;


unsigned char side_count = 0;
unsigned char turning = 0;     // 0 = going straight, 1 = turning

unsigned char loop_side = 0;   // 0 = first circle (left turn), 1 = second circle (right turn)

void Run_Straight(void){
 switch(state){
 case WAIT: // Begin
 wait_case();
 break; //
 case START: // Begin
 start_case();
 break; //
 case RUN: // Run
 run_case();
 break; //
 case END: // End
 end_case();
 break; //
 default: break;
 }
}

void start_case(void){
 cycle_time = 0;
 right_motor_count = 0;
 left_motor_count = 0;
 moveforward();
 segment_count = 0;
 state = RUN;
}

void run_case(void){
 if(time_change){
 time_change = 0;
 if(segment_count <= TRAVEL_DISTANCE){
 if(right_motor_count++ >= RIGHT_COUNT_TIME){
 P6OUT &= ~R_FORWARD;
 }
 if(left_motor_count++ >= LEFT_COUNT_TIME){
 P6OUT &= ~L_FORWARD;
 }
 if(cycle_time >= WHEEL_COUNT_TIME){
 cycle_time = 0;
 right_motor_count = 0;
 left_motor_count = 0;
 segment_count++;
 moveforward();
 }
 }else{
 state = END;
 }
 }
}


void Run_Circle(void){
  switch(state){
    case WAIT:   wait_case();        break;
    case START:  start_case_circle();       break;
    case RUN:    run_case_circle();  break;
    case END:    end_case();         break;
    default:     break;
  }
}

void wait_case(void){
  if(time_change){
    time_change = 0;
    if(delay_start++ >= WAITING2START){
      delay_start = 0;
      state = START;
    }
  }
}

void start_case_circle(void){
  cycle_time = 0;
  right_motor_count = 0;
  left_motor_count = 0;
  moveforward();           // both wheels on at start
  segment_count = 0;
  state = RUN;
}

void run_case_circle(void){
  if(time_change){
    time_change = 0;

    if(segment_count <= TRAVEL_DISTANCE_C){
      // Slow down RIGHT wheel
      if(right_motor_count++ >= (RIGHT_COUNT_TIME_C)){   // make right slower
        P6OUT &= ~R_FORWARD;
      }
      // Left wheel normal
      if(left_motor_count++ >= LEFT_COUNT_TIME_C){
        P6OUT &= ~L_FORWARD;
      }

      if(cycle_time >= WHEEL_COUNT_TIME_C){
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        segment_count++;

        // Restart motors each cycle
       moveforward();   // right wheel pulses
      }
    } else {
      state = END;
    }
  }
}

void end_case(void){
  stopall();
  state = WAIT;
  event = NONE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

void Run_Triangle(void){
  switch(state){
    case WAIT:   wait_case();        break;
    case START:  start_case_triangle(); break;
    case RUN:    run_case_triangle();   break;
    case END:    end_case();            break;
    default:     break;
  }
}

void start_case_triangle(void){
  cycle_time = 0;
  right_motor_count = 0;
  left_motor_count = 0;
  moveforward();
  segment_count = 0;
  side_count = 0;
  turning = 0;
  state = RUN;
}

void run_case_triangle(void){
  if(time_change){
    time_change = 0;

    if(!turning){
      //  Straight section
      if(segment_count <= TRAVEL_DISTANCE_T){
        // Right wheel full speed
        if(right_motor_count++ >= LEFT_COUNT_TIME_T){
          P6OUT &= ~R_FORWARD;
        }
        // Left wheel full speed
        if(left_motor_count++ >= LEFT_COUNT_TIME_T){
          P6OUT &= ~L_FORWARD;
        }

        if(cycle_time >= WHEEL_COUNT_TIME_T){
          cycle_time = 0;
          right_motor_count = 0;
          left_motor_count = 0;
          segment_count++;

          // Restart motors each cycle
          moveforward();
        }
      } else {
        // Start turning
        segment_count = 0;
        turning = 1;
      }
    } else {
      // Turn section ----
      if(segment_count <= TURN_DISTANCE_T){
        // Slow right wheel to pivot left ~120°
        if(right_motor_count++ >= RIGHT_COUNT_TIME_T){
          P6OUT &= ~R_FORWARD;
        }
        if(left_motor_count++ >= LEFT_COUNT_TIME_T){
          P6OUT &= ~L_FORWARD;
        }

        if(cycle_time >= WHEEL_COUNT_TIME_T){
          cycle_time = 0;
          right_motor_count = 0;
          left_motor_count = 0;
          segment_count++;

          // Restart motors each cycle
         moveforward();
        }
      } else {
        // Done with one turn
        segment_count = 0;
        turning = 0;
        side_count++;

        if(side_count >= 6){
          state = END;   // Triangle complete
        }
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void Run_Figure8(void){
  switch(state){
    case WAIT:   wait_case();           break;
    case START:  start_case_figure8();  break;
    case RUN:    run_case_figure8();    break;
    case END:    end_case();            break;
    default:     break;
  }
}


void start_case_figure8(void){
  cycle_time = 0;
  right_motor_count = 0;
  left_motor_count = 0;
  moveforward();
  segment_count = 0;
  loop_side = 0;           // start with first loop
  state = RUN;
}



void run_case_figure8(void){
  if(time_change){
    time_change = 0;

    if(segment_count <= TRAVEL_DISTANCE_8){
      if(loop_side == 0|| loop_side == 2){
        // First loop: slow RIGHT wheel
        if(right_motor_count++ >= RIGHT_COUNT_TIME_8){
          P6OUT &= ~R_FORWARD;
        }
        if(left_motor_count++ >= LEFT_COUNT_TIME_8){
          P6OUT &= ~L_FORWARD;
        }
      }
      else {
        // Second loop: slow LEFT wheel
        if(left_motor_count++ >= RIGHT_COUNT_TIME_8){
          P6OUT &= ~L_FORWARD;
        }
        if(right_motor_count++ >= LEFT_COUNT_TIME_8){
          P6OUT &= ~R_FORWARD;
        }
      }

      if(cycle_time >= WHEEL_COUNT_TIME_8){
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        segment_count++;

        // Restart motors each cycle
        moveforward();
      }
    } else {
      if(loop_side == 3){
       state = END;
      } else {
                 loop_side++;
                 segment_count = 0;
                 cycle_time = 0;
                 right_motor_count = 0;
                 left_motor_count = 0;
                 moveforward();
      }
    }
  }
}
