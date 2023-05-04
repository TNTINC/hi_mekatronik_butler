#include "motors.h"
#include "avr/sfr_defs.h"

void motors_init(){
  // Set 0 pwm
  pwmout_set(M1_PWM_CH, 0);
  pwmout_set(M2_PWM_CH, 0);
  // Set directon <pins as outputs
  M1_DIR_PORT &= ~_BV(M1_DIR_PIN);
  M2_DIR_PORT &= ~_BV(M2_DIR_PIN);
  M1_DIR_DDR |= _BV(M1_DIR_PIN);
  M2_DIR_DDR |= _BV(M2_DIR_PIN);
}

namespace {
  static motors_last_command = 0;
  static const MOTOR_TIMEOUT_MS = 1000;
}
void motors_loop(){
  if(millis() - motors_last_command >= MOTOR_TIMEOUT_MS){ // Timeout, stop.
    motors_set_pwm(0, 0);
  }
}

void motors_set_pwm(int16_t m1_duty, int16_t m2_duty){
  motors_last_command = millis();
  if(m1_duty < 0){ 
    M1_DIR_PORT |= _BV(M1_DIR_PIN);
    m1_duty *= -1;    
 } else { M1_DIR_PORT &= ~_BV(M1_DIR_PIN); }
  if(m2_duty < 0){ 
    M2_DIR_PORT |= _BV(M2_DIR_PIN);
    m2_duty *= -1;
  } else { M2_DIR_PORT &= ~_BV(M2_DIR_PIN); }
  pwmout_set(M1_PWM_CH, m1_duty);
  pwmout_set(M2_PWM_CH, m2_duty);
}
