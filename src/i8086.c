/*
Intel 8086 processor emulator
*/

#include "i8086.h"

/* GENERIC MEMORY OPS */

// Read byte from memory
static inline uint8_t i8086_rb(i8086 *const p, uint16_t addr)
{
  return p->read_byte(p->user_data, addr);
}

// Write byte to memory
static inline void i8086_wb(i8086 *const p, uint16_t addr, uint8_t data)
{
  p->write_byte(p->user_data, addr, data);
}

// Read word from memory
static inline uint16_t i8086_rw(i8086 *const p, uint16_t addr)
{
  return p->read_word(p->user_data, addr);
}

// Write word to memory
static inline void i8086_ww(i8086 *const p, uint16_t addr, uint16_t data)
{
  p->write_word(p->user_data, addr, data);
}

// Read next byte and update program counter
static inline uint8_t i8086_next_byte(i8086 *const p)
{
  return i8086_rb(p, p->pc++);
}

// Read next word and update program counter
static inline uint16_t i8086_next_word(i8086 *const p)
{
  uint16_t data = i8086_rw(p, p->pc);
  p->pc += 2;
  return data;
}

/* STACK OPS */

static inline void i8086_push_stack(i8086 *const p, uint16_t data)
{
  p->sp -= 2;
  i8086_ww(p, p->sp, data);
}

static inline uint16_t i8086_pop_stack(i8086 *const p)
{
  uint16_t data = i8086_rw(p, p->sp);
  p->sp += 2;
  return data;
}

/* GENERAL HELPERS */

static inline bool parity(uint16_t data)
{
  uint8_t one_bits = 0;
  for (int i = 0; i < 16; i++)
  {
    one_bits += ((data >> i) & 1);
  }

  return (one_bits & 1) == 0;
}

/* 
'bits' parameter is used to define the size of the summands in bits (this carry detector 
could be used for 4, 8 and 16 bit numbers).
*/
static inline bool carry(uint8_t bits, uint16_t a, uint16_t b, bool carr)
{
  int16_t result = a + b + carr;
  int16_t carry = result ^ a ^ b;
  return carry & (1 << bits);
}