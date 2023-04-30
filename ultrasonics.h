#ifndef ULTRASONICS_H_
#define ULTRASONICS_H_
#include <avr/io.h>

/* Read an arbitrary number of ultrasonic sensors using input capture.
 * Hardware resources used: Timer1 */

// Pin D8 (PB0, ICP1) is used to read the combined ECHO lines of all the sensors
// It can't be changed since it has unique hardware attached.
// The trigger pins can be defined arbitrarily
// Ultrasonic sensor definitions
static struct {
  const uint8_t pin;
  volatile uint8_t* const port;
  volatile uint8_t* const ddr;
  long range_mm;  // Storage for last read range (in mm)
} SONARS[] = {
  { PB1, &PORTB, &DDRB, -1 },
  { PB2, &PORTB, &DDRB, -1 },
  { PB3, &PORTB, &DDRB, -1 }
};
static constexpr uint8_t ultrasonics_count = sizeof(SONARS) / sizeof(SONARS[0]);

// Minimum time delay between ultrasonic pulses in milliseconds, datasheet recommends 60ms
static const uint16_t ULTRASONIC_RATE = 40;

void ultrasonics_init();

void ultrasonics_loop();

long ultrasonics_read(uint8_t ch);

#endif