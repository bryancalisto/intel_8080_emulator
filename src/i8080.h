/*
Intel 8080 emulator
*/

#ifndef i8080_H
#define i8080_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct i8080
{
  /* 
  Memory ops
  Parameters:
  1st = address
  2sd = value
  */
  uint8_t (*read_byte)(uint16_t);
  void (*write_byte)(uint16_t, uint8_t);

  // I/O ops
  uint8_t (*port_in)(uint8_t);
  uint8_t (*port_out)(uint8_t, uint8_t);

  // Main registers
  uint8_t a, b, c, d, e, h, l;

  // Index registers
  uint16_t bp, sp;

  // Program counter (instruction pointer)
  uint16_t pc;

  // Flags (zero, signed, parity, carry, auxiliary carry)
  bool zf, sf, pf, cf, acf;

  // Some other necessary state
  bool halted;
  bool interrupted;
} i8080;

void i8080_init(i8080 *const p);
void i8080_step(i8080 *const p);

#endif // i8080_H