#ifndef MOTORS_H_
#define MOTORS_H_
/* Drive the motors using the PCA9685 to generate PWM signals fed to the G2 motor controller */

#include <Arduino.h>
#include "pwmout.h"
#include "avr/io.h"

#define M1_PWM_CH 0
#define M1_DIR_PIN PD6
#define M1_DIR_DDR DDRD
#define M1_DIR_PORT PORTD

#define M2_PWM_CH 1
#define M2_DIR_PIN PD7
#define M2_DIR_DDR DDRD
#define M2_DIR_PORT PORTD

/* Must be called _after_ pwmout_init() */
void motors_init();

/*
 * Set the direction and duty cycle of each motor.
 * m1 and m2 should be in the range -4095 to 4095.
 */
void motors_set_pwm(int16_t m1, int16_t m2);

/*
 * Set target direction and speed in ticks / cycle.
 */
void motors_set_pid(long m1, long m2);

/*
 * Call regularly, Ensures motors time out if no commands are received.
*/
void motors_loop();


#endif