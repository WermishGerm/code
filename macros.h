


//------------------------------------------------------------------------------
//Masks
//------------------------------------------------------------------------------
#define TWO_BIT_MASK 0x03
//------------------------------------------------------------------------------
//Main macros
//------------------------------------------------------------------------------
#define ONE_SECOND 200
#define TWO_SECONDS 400
#define THREE_SECONDS 600
#define FOUR_SECONDS 800
#define FIVE_SECONDS 1000

#define WAIT 1
#define DRIVE 2
#define SPIN_BLACK_LINE 3
#define REVERSE_STATE 5
#define NAVIGATION 6
#define SPIN_CENTER 7
#define DRIVE_CETNER 8


#define BLACK_LINE 500

//------------------------------------------------------------------------------
//Navigation macros
//------------------------------------------------------------------------------
#define VLEFT_AVERAGE return_vleft_average()
#define VRIGHT_AVERAGE return_vright_average()

#define MIN_SPEED 0
#define MAX_SPEED_NAV 5000
#define BLACK_LINE_VALUE 621
#define WHITE_VALUE_MAX 150

#define OFF_SPEED 1600
#define RECOVERY_TIME 150

#define KP 25

#define WHITE_STATE 0
#define LEFT_STATE 1
#define RIGHT_STATE 2
#define LINE_STATE 3


//------------------------------------------------------------------------------
//ADC macros
//------------------------------------------------------------------------------
#define VRIGHT_CALIBRATION 3

//------------------------------------------------------------------------------
//DAC macros
//------------------------------------------------------------------------------
#define MIN_DAC 0x0fff
#define DAC_6V 1200

//------------------------------------------------------------------------------
//Timer macros
//------------------------------------------------------------------------------

#define MAX_SCREEN_CLOCK_VALUE 9998

//timer 0
#define TB0CCR0_INTERVAL 625
#define TB0CCR1_INTERVAL 62500
#define TB0CCR2_INTERVAL 62500
//timer 1
#define TB1CCR0_INTERVAL 4096
#define TB1CCR1_INTERVAL 4000
#define TB1CCR2_INTERVAL 819
//time 2
#define TB2CCR1_INTERVAL (4000)   // ~5 ms debounce delay


//------------------------------------------------------------------------------
//Drive macros
//------------------------------------------------------------------------------
#define FORWARD 1
#define REVERSE 0
#define CHANGE_COUNT 5
#define MAX_SPEED 10000

#define WHEEL_PERIOD 10000
#define WHEEL_OFF 0

//LCD Dispaly Macros
//------------------------------------------------------------------------------
#define LINE1 0
#define LINE2 1
#define LINE3 2
#define LINE4 3

#define LCD_OFF P6OUT &= ~LCD_BACKLITE
#define LCD_ON P6OUT |= LCD_BACKLITE
#define LCD_TOGGLE P6OUT ^= LCD_BACKLITE

#define MAX_LCD_LENGTH 11
#define CHAR_0 0

//------------------------------------------------------------------------------
#define ALWAYS                  (1)
#define RESET_STATE             (0)



//------------------------------------------------------------------------------
//from clocks.c
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//switch macros
//------------------------------------------------------------------------------
#define SWITCH_RESET_TIME 20
#define PRESSED  1
#define RELEASED 0

// General
#define ALWAYS          (1)
#define RESET_STATE     (0)
#define TRUE            (0x01)
#define FALSE           (0)
#define CLEAR_REGISTER  (0x0000)
#define RED_LED         (0x01)   // Red LED
#define GRN_LED         (0x40)   // Green LED
#define TEST_PROBE      (0x01)   // Test probe
#define MCLK_FREQ_MHZ   (8)      // MCLK = 8 MHz

//numerical defines
#define FOURTH          (4)
#define ELEVENTH        (11)

// STATES ======================================================================
#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define TRIANGLE ('T')
#define FIGURE8 ('F')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define WHEEL_COUNT_TIME (20)
#define RIGHT_COUNT_TIME (10)
#define LEFT_COUNT_TIME (10)
#define TRAVEL_DISTANCE (10)
#define WAITING2START (50)

// Run Circle
#define WHEEL_COUNT_TIME_C  (45)
#define RIGHT_COUNT_TIME_C  (45)
#define LEFT_COUNT_TIME_C   (15)
#define TRAVEL_DISTANCE_C   (60)

// Run Triangle
#define WHEEL_COUNT_TIME_T  (15)//previous 10
#define RIGHT_COUNT_TIME_T  (0) //previous 0
#define LEFT_COUNT_TIME_T   (9)
#define TURN_DISTANCE_T     (18)// previous 40
#define TRAVEL_DISTANCE_T   (17) // previous 15

// Run Figure 8
#define WHEEL_COUNT_TIME_8  (30) // 30
#define RIGHT_COUNT_TIME_8  (30) //30 works
#define LEFT_COUNT_TIME_8   (4) // 5 worked
#define TRAVEL_DISTANCE_8   (35) //30 worked


#define PRESSED (0)
#define RELEASED (1)

#define OKAY (1)
#define NOT_OKAY (0)

#define DEBOUNCE_RESTART (0)
#define  DEBOUNCE_TIME (100)

// Project 5
#define TIMER_B0_CCR0_VECTOR TIMER0_B0_VECTOR
#define TIMER_B0_CCR1_2_OV_VECTOR TIMER0_B1_VECTOR
#define TIMER_B1_CCR0_VECTOR TIMER1_B0_VECTOR
#define TIMER_B1_CCR1_2_OV_VECTOR TIMER1_B1_VECTOR
#define TIMER_B2_CCR0_VECTOR TIMER2_B0_VECTOR
#define TIMER_B2_CCR1_2_OV_VECTOR TIMER2_B1_VECTOR
#define TIMER_B3_CCR0_VECTOR TIMER3_B0_VECTOR
#define TIMER_B3_CCR1_2_OV_VECTOR TIMER3_B1_VECTOR

#define PWM_PERIOD (TB3CCR0)
#define LEFT_FORWARD_SPEED (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_REVERSE_SPEED (TB3CCR3)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LCD_BACKLITE_DIMING (TB3CCR5)

#define TB0CCR0_INTERVAL (2500)
#define TB0CCR1_INTERVAL (2500)
#define TB0CCR2_INTERVAL (2500)

//-----------------------------------------------------------------------------
// PROJECT 06: BLACK LINE STATES
//-----------------------------------------------------------------------------
#define SEARCH      (5)
#define STOP        (6)
#define SECR        (7) // Line alignment/turning state
#define STOPALL     (8)

//-----------------------------------------------------------------------------
// ADC THRESHOLDS (Requires Calibration)
//-----------------------------------------------------------------------------
#define LEFT_BLACK_THRESH       (2000) // Placeholder: Calibrate on your car
#define RIGHT_BLACK_THRESH      (2000) // Placeholder: Calibrate on your car

//-----------------------------------------------------------------------------
// MOTOR SPEEDS (PWM DUTY CYCLES - Assuming Max is 1000)
//-----------------------------------------------------------------------------
#define FAST                    (1)
#define MED                     (0)

//-----------------------------------------------------------------------------
// TIMING CONSTANTS (Assuming 10ms Timer Tick)
//-----------------------------------------------------------------------------
#define BLACK_LINE_FOLLOW ('B')
// In macros.h (around line 70)

// TIMING CONSTANTS (Assuming 10ms Timer Tick)
#define TB1_1SEC                (100) // 1.0 second delay (100 * 10ms)
#define DELAY_START_TICKS       (100) // <--- ADD THIS: 1.0 second startup delay
#define DETECT_WAIT_TICKS       (300) // <--- ADD THIS: 3.0 second stop (Verification 7)
