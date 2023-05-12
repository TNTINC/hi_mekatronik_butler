#include "motors.h"
#include "avr/sfr_defs.h"
#include "encoders.h"

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
  // Timeout stuff
  static long motors_last_command = 0;
  static long const MOTOR_TIMEOUT_MS = 1000;
  // PID stuff
  static int m1_setpoint = 0, m2_setpoint = 0;
  static long m1_enc_last = 0, m2_enc_last = 0;
  static long m1_I = 0, m2_I = 0;
  static constexpr long denom = 100;
  static constexpr long kf = 200 * denom;
  static constexpr long kp = 0;
  static constexpr long ki = 10000;
  static constexpr long max_cmd = 1024;

  static const long PID_RATE_MS = 100;
  static long last_pid = 0;
}
void motors_do_pid(){
  long m1_enc_cur, m2_enc_cur;
  encoders_read(&m1_enc_cur, &m2_enc_cur);
  long m1_vel = m1_enc_cur - m1_enc_last;
  long m2_vel = m2_enc_cur - m2_enc_last;
  m1_enc_last = m1_enc_cur;
  m2_enc_last = m2_enc_cur;

  long m1_error = m1_setpoint - m1_vel;
  long m1_raw = (m1_error * kp + m1_I + (long)m1_setpoint * kf) / denom;

  if(m1_raw >= max_cmd){
    m1_raw = max_cmd;
  } else if (m1_raw <= -max_cmd) {
    m1_raw = -max_cmd;
  }

  long m1_di = m1_error * ki;
  if((m1_di > 0 && !(m1_raw >= max_cmd))
  || (m1_di < 0 && !(m1_raw <= -max_cmd))){
    m1_I += m1_di;
  }

  motors_set_pwm(m1_raw, 0);

  Serial.print("vel: ");
  Serial.print(m1_vel);
  Serial.print(" err: ");
  Serial.print(m1_error);
  Serial.print(" I: ");
  Serial.print(m1_I);
  Serial.print(" cmd: ");
  Serial.println(m1_raw);
}

void motors_loop(){
  if(millis() - motors_last_command >= MOTOR_TIMEOUT_MS){ // Timeout, stop.
    motors_set_pwm(0, 0);
    //motors_set_pid(0, 0);
  }
  if(millis() - last_pid >= PID_RATE_MS){
    last_pid = millis();
    //motors_do_pid();
  }
}

void motors_set_pwm(int16_t m1_duty, int16_t m2_duty){
  motors_last_command = millis();
  // Flip motor 2 direction
  m2_duty *= -1;
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

void motors_set_pid(long m1, long m2){
  motors_last_command = millis();
  m1_setpoint = m1;
  m2_setpoint = m2;
}