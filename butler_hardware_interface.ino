/* HI Mechatronics dept. Robot Butler hardware interface
 *
 * Author: Fridfinnur M. <fmt1@hi.is>
 * Based on https://github.com/joshnewans/ros_arduino_bridge
 */

#include "encoders.h"


/* Settings */
#define CMD_MAXLEN 32
#define N_ARGS 3

/* Parses a command like "m 50 100"
 * (modifies) cmd_buf: string to parse
 * (out) argc: Out param for number of parsed arguments
 * (out) argv: Out param for parsed arguments
 * Returns: If successful parse, first char of cmd_buf (command)
 *          Returns '\0' to indicate failed parse. */
char parse_command(char* cmd_buf, uint8_t& argc, int* argv);

/* Parses and runs the given command if possible */
void run_command(char* cmd_buf);

/* Debugging command, just prints argc and argv */
void cmd_print(uint8_t argc, int* argv);


void setup() {
  Serial.begin(115200);
  encoders_init();
}

void loop() {
  uint8_t cb_idx = 0;       // Current index in command buffer
  char cmd_buf[CMD_MAXLEN]; // Command buffer
  while(true){ // Main loop

    while(Serial.available()) { // Read serial data if possible
      char cur = Serial.read();
      switch(cur) {
        break; case '\b': // Backspace
          cmd_buf[cb_idx] = '\0'; // New end of string
          if(cb_idx > 0){ --cb_idx; } // Move back one, unless that would underflow

        break; case '\r': // CR
          cmd_buf[cb_idx] = '\0'; // End of string
          cb_idx = 0; // Prepare index for next command
          run_command(cmd_buf);
        

        break; case '\n': // LF: ignore

        break; default: // Normal char
          cmd_buf[cb_idx++] = cur;
          if(cb_idx >= CMD_MAXLEN){
            cb_idx = 0;
            Serial.println("Buffer overflow");
          }
      }
    }

    

  }
}


void run_command(char* cmd_buf){
  static uint8_t argc = 0;         // Number of parsed arguments
  static int argv[N_ARGS];         // Buffer for parsed arguments

  char cmd = parse_command(cmd_buf, argc, argv);
  if(cmd == '\0') {
      Serial.println("Failed to parse");
      return;
  }

  switch(cmd) {
    break; case 'p': cmd_print(argc, argv); // Print, for debugging
    break; case 'e': { // Encoders, print encoder values
      long e1, e2;
      encoders_read(&e1, &e2);
      Serial.print(e1);
      Serial.print(" ");
      Serial.println(e2);
    }
    break; default:
      Serial.println("Unknown command");
  }
}


char parse_command(char* cmd_buf, uint8_t& argc, int* argv) {
  char* sp;
  char* curtok;

  char* cmd = strtok_r(cmd_buf, " ", &sp);
  if(cmd == nullptr || strlen(cmd) != 1){ return '\0'; } // Invalid command
  
  argc = 0;
  while(argc < N_ARGS && (curtok = strtok_r(nullptr, " ", &sp)) != nullptr) {
    argv[argc++] = atoi(curtok);
  }

  return cmd[0];
}


void cmd_print(uint8_t argc, int* argv){
  Serial.print(" argc: ");
  Serial.print(argc);
  for(uint8_t i = 0; i < argc; ++i){
    Serial.print(" arg");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(argv[i]);
  }
  Serial.println("\nOK");
}