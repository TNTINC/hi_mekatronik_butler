#ifndef encoders_h_
#define encoders_h_

/* Read two quadrature encoders using a polling interrupt. 
 * Hardware resources used: Timer2 */

// Pin definitions. (See https://www.circuito.io/blog/arduino-uno-pinout/ for mapping from arduino pin numbers)
#define E1A PD2
#define E1A_DDR DDRD
#define E1A_PIN PIND

#define E1B PD3
#define E1B_DDR DDRD
#define E1B_PIN PIND

#define E2A PD4
#define E2A_DDR DDRD
#define E2A_PIN PIND

#define E2B PD5
#define E2B_DDR DDRD
#define E2B_PIN PIND

// Sets up timer2 and starts regularly reading encoder values.
void encoders_init();

// Reads encoder values into *e1 and *e2
void encoders_read(long* e1, long* e2);

// Resets the encoder counts to 0
void encoders_reset();

#endif