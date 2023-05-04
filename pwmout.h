#ifndef PWMOUT_H_
#define PWMOUT_H_

#include <stdint.h>

void pwmout_init();

#define PWM_MAX_DUTY 4095
/* Set duty cycle of pwm channel. Max duty is 4095. */
void pwmout_set(uint8_t channel, uint16_t duty);

void cmd_pwmout(uint8_t argc, int* argv);

#endif