/*
Intel 8086 emulator
*/

#ifndef I8086_H
#define I8086_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*According to the docs, all registers are 16 bit wide*/

typedef struct i8086
{
  // Memory ops
  uint8_t (*read_byte)(void *, uint16_t);
  void (*write_byte)(void *, uint16_t, uint8_t);
  uint16_t (*read_word)(void *, uint16_t);
  void (*write_word)(void *, uint16_t, uint16_t);

  // I/O ops
  uint16_t (*port_in)(void *, uint16_t);
  uint16_t (*port_out)(void *, uint16_t, uint16_t);

  // Main registers
  uint16_t a, b, c, d, h, l; // h=high, l=low

  // Index registers
  uint16_t si, di, bp, sp;

  // Program counter (instruction pointer)
  uint16_t pc;

  // Segment register
  uint16_t cs, ds, es, ss;

  // Flags (overflow[11], direction[10], interrupt[9], trap[8], signed[7], zero[6], adjust[4], parity[2], carry[0])
  bool of = 0, df = 0, iif = 0, tf = 0, sf = 0, zf = 0, af = 0, pf = 0, cf = 0;

  // Some other necessary state
  bool halted = 0;
  bool interrupt_waiting = 0;
  uint16_t interrupt_vector;
  uint16_t interrupt_delay;
} i8086;

void i8086_init(i8086 *const p);
void i8086_step(i8086 *const p);
void i8086_interrupt(i8086 *const p, uint8_t opcode);
void i8086_debug(i8086 *const p, bool show_asm);

#endif // I8086_H