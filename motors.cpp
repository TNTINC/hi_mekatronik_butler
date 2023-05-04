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


void motors_set_pwm(int16_t m1_duty, int16_t m2_duty){
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
