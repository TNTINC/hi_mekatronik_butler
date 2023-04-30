#ifndef PWMOUT_H_
#define PWMOUT_H_

#include <stdint.h>

void pwmout_init();

void pwmout_set(uint8_t channel, uint8_t duty);

void cmd_pwmout(uint8_t argc, int* argv);

#endif