#include "encoders.h"

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

namespace {
// Global encoder counts, no external access (interrupt in the middle of a 4-byte read could trash the value)
static long E1_VAL_ = 0;
static long E2_VAL_ = 0;
}


void encoders_init(){
  // Set pinmode on input pins
  E1A_DDR &= ~_BV(E1A);
  E1B_DDR &= ~_BV(E1B);
  E2A_DDR &= ~_BV(E2A);
  E2B_DDR &= ~_BV(E2B);
  
  // Setup timer in CTC mode (count up to OCR2A and reload)
  TCCR2A = _BV(WGM21);
  // Desired frequency 1khz, clock freq 16MHz.
  // 16MHZ * Prescaler 1/256 * Top 1/62 = 1008hz
  OCR2A = 62;
  TCCR2B = 0 | _BV(CS22) | _BV(CS20);
  // Unmask OCF2A interrupt
  TIMSK2 = _BV(OCIE2A);
}


void encoders_read(long* e1, long* e2){
  cli();
  *e1 = E1_VAL_;
  *e2 = E2_VAL_;
  sei();
}


void encoders_reset(){
  cli();
  E1_VAL_ = 0;
  E2_VAL_ = 0;
  sei();
}


/* Last two read states of both encoders, bitfield
 * Bit 0 is current E1A state, bit 1 is current E1B state, bit 3 is previous E1A state, bit 4 is previous E1B state.
 * Bits 5:7 are the same for E2 */
static uint8_t ENCODER_STATES = 0;
// Encoder lookup table. Maps 4-bit state transition to appropriate increment/decrement
static const int8_t ENC_TABLE[] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};
ISR(TIMER2_COMPA_vect) {
  register uint8_t state = (ENCODER_STATES << 2) & 0b11001100; // Current states are now previous states
  // Read input pins
  if(E1A_PIN & _BV(E1A)){ state |= _BV(0); }
  if(E1B_PIN & _BV(E1B)){ state |= _BV(1); }
  if(E2A_PIN & _BV(E2A)){ state |= _BV(4); }
  if(E2B_PIN & _BV(E2B)){ state |= _BV(5); }
  ENCODER_STATES = state;
  // Update encoder counts
  E1_VAL_ += ENC_TABLE[state & 0b1111];
  E2_VAL_ += ENC_TABLE[state >> 4];
}