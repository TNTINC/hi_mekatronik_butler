#include "pwmout.h"
#include "src/Adafruit-PWM-Servo-Driver-Library/Adafruit_PWMServoDriver.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
void pwmout_init() {
  pwm.begin();
  pwm.setPWMFreq(440);
}

void pwmout_set(uint8_t channel, uint16_t duty) {
  // PCA duty is 12-bit 0-4095
  pwm.setPin(channel, duty);
}