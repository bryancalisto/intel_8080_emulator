/*
Intel 8080 emulator
*/

#ifndef i8080_H
#define i8080_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*According to the docs, all registers are 16 bit wide*/

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
  uint16_t (*read_word)(uint16_t);
  void (*write_word)(uint16_t, uint16_t);

  // I/O ops
  uint16_t (*port_in)(uint16_t);
  uint16_t (*port_out)(uint16_t, uint16_t);

  // Main registers
  uint16_t a, b, c, d;
  uint8_t al, ah, bl, bh, cl, ch, dl, dh;

  // Index registers
  uint16_t si, di, bp, sp;

  // Program counter (instruction pointer)
  uint16_t pc;

  // Segment register
  uint16_t cs, ds, es, ss;

  // Flags (overflow[11], direction[10], interrupt[9], trap[8], signed[7], zero[6], adjust[4], parity[2], carry[0])
  bool of, df, iif, tf, sf, zf, af, pf, cf;

  // Some other necessary state
  bool halted;
  bool locked;
  bool interrupt_waiting;
  uint16_t interrupt_vector;
  uint16_t interrupt_delay;
} i8080;

void i8080_init(i8080 *const p);
void i8080_step(i8080 *const p);

#endif // i8080_H