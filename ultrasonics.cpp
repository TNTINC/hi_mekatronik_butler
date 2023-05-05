#include <Arduino.h>
#include "ultrasonics.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void ultrasonics_init(){
  // Max length of echo pulse is ~38000us
  // Best accuracy would be to run a 1us/count, but that would require a 1/16 prescaler
  // Closest available is 1/64 giving 4us per count, which translates to a resolution of 1.3mm
  // Stated sensor resolution is 3mm so this is more than sufficient
  // Also enable the input capture noise canceler
  TCCR1B = _BV(ICNC1) | _BV(CS11) | _BV(CS10);
  // Ensure timer is in normal mode
  TCCR1A = 0;
  
  // Set trigger pins to output
  for(uint8_t i = 0; i < ultrasonics_count; i++){
    *(SONARS[i].port) &= ~_BV(SONARS[i].pin);
    *(SONARS[i].ddr) |= _BV(SONARS[i].pin);
  }

  // Set power pin to output high
  DDRB |= _BV(PB4);
  PORTB |= _BV(PB4);
}

long ultrasonics_read(uint8_t ch){
  return SONARS[ch].range_mm;
}

// Private global state
namespace {
  enum UltrasonicState {
    BETWEEN_PULSES,
    PULSE_SENT,
    ECHO_ONGOING,
    ECHO_DONE
  };
  static volatile UltrasonicState state = BETWEEN_PULSES;
  static volatile uint16_t echo_begin = 0, echo_end = 0;
}

void ultrasonics_loop(){
  static uint8_t cur_sensor = 0;
  static long last_pulse = millis();
  static long echo_timeout_start = millis();
  switch(state){
    break; case ECHO_DONE:
      // Compute and store distance
      // 1 cnt = 4 us, 1 us = 1/1000000 s, 1 s = 340 m = 340000mm, so
      SONARS[cur_sensor].range_mm = ((long)(echo_end - echo_begin) * 4 * 34) / (100 * 2); // divided by two since the sound has to get to the target and back.
      state = BETWEEN_PULSES;
      /* Fallthrough */
    case BETWEEN_PULSES: 
      if ((long)millis() - last_pulse < ULTRASONIC_RATE){ return; } // Wait until time for next pulse
      // Setup ICR module to capture rising edge of ECHO
      TCCR1B |= _BV(ICES1); // Edge select rising
      TIFR1 |= _BV(ICF1); // Clear any pending interrupt
      TIMSK1 |= _BV(ICIE1); // Unmask ICP interrupt

      // Send pulse
      cur_sensor = (cur_sensor + 1) % ultrasonics_count;
      last_pulse = millis();
      *(SONARS[cur_sensor].port) |= _BV(SONARS[cur_sensor].pin);
      delayMicroseconds(10);
      *(SONARS[cur_sensor].port) &= ~_BV(SONARS[cur_sensor].pin);
      state = PULSE_SENT;
    break; 
    case PULSE_SENT:
      echo_timeout_start = millis();
      if (millis() - last_pulse < 5){return;}
      // If sensor doesn't set echo high in ~5ms then its likely disconnected
      TIMSK1 &= ~_BV(ICIE1); // Mask interrupt
      SONARS[cur_sensor].range_mm = -1; // Indicate error
      state = BETWEEN_PULSES;
    break; case ECHO_ONGOING: /* Waiting for interrupt */
      if (millis() - last_pulse < ECHO_TIMEOUT){ return; } // Do nothing if timeout hasn't elapsed
      // Timeout because the sensors will wait forever if they don't hear an echo (due to too far distance).
      TIMSK1 &= ~_BV(ICIE1); // Mask interrupt
      SONARS[cur_sensor].range_mm = 5000; // Indicate out-of-range
      // Try to fix a hung sensor by driving the power pin low for a moment.
      PORTB &= ~_BV(PB4);
      delayMicroseconds(100);
      PORTB |= _BV(PB4);
      // Wait to let the sensors recover
      //last_pulse = millis() - ULTRASONIC_RATE + 5;
      state = BETWEEN_PULSES;
  }
}

ISR(TIMER1_CAPT_vect) {
  switch(state){
    break; case PULSE_SENT:
      // We've picked up the beginning of the echo pulse
      echo_begin = ICR1; // Save the timestamp
      ICR1 = 0;
      // Setup to catch the end of the echo pulse
      TCCR1B &= ~_BV(ICES1); // Edge select falling
      state = ECHO_ONGOING;
    break; case ECHO_ONGOING:
      // We've picked up the end of the echo pulse
      echo_end = ICR1;
      state = ECHO_DONE;
      // Interrupt's responsibility is done so mask it
      TIMSK1 &= ~_BV(ICIE1);
    break;
    case BETWEEN_PULSES:
    case ECHO_DONE:
    break;
  }
}
