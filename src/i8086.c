/*
Intel 8086 processor emulator
*/

#include "i8086.h"

/*To facilitate the parsing of mod_reg_r/m byte*/
typedef struct mod_reg_rm
{
  uint8_t mod;
  uint8_t reg;
  uint8_t r_m;
} mod_reg_rm;

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

/* Util to set flags that are commonly set together: Zero, signed, parity*/
void set_ZSP(i8086 *const p, uint16_t val)
{
  p->zf = val == 0;
  p->sf = (val >> 15);
  p->pf = parity(val);
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

/* Opcode helpers*/
/*
Checks if last byte of opcode ('w', according to the intel manual) is 1 or 0. 
If 1, then it's a word operation, else it's a byte operation
*/
static inline bool is_word_op(uint8_t byte)
{
  return (byte & 0b1);
}

static inline bool is_destiny_to(uint8_t byte)
{
  return (byte & 0b10);
}

static inline uint8_t get_mod(uint8_t byte)
{
  // Mod is defined by the two highest bits
  return (byte >> 6) & 0b11;
}

static inline uint8_t get_r_m(uint8_t byte)
{
  return (byte >> 3) & 0b111;
}

/*Helper to decode the second byte of an instruction*/
void decodeSecondOp(mod_reg_rm *mrrm, uint8_t op)
{
  mrrm->mod = (op >> 6) & 0b11;
  mrrm->r_m = (op >> 3) & 0b111;
  mrrm->reg = op & 0b111;
}

/*ARITHMETIC*/
static inline void i8086_add(i8086 *const p, uint8_t *const reg, uint8_t val, bool cy)
{
  uint8_t result = *reg + val + cy;
  p->cf = carry(8, *reg, val, cy);
  SET_ZSP(p, result);
  *reg = result;
}

void i8086_exec(i8086 *const p, mod_reg_rm *m_r_rm, uint8_t opcode)
{
  uint8_t mod;
  uint8_t next_b_1;
  uint8_t next_b_2;
  uint16_t next_w_1;
  uint16_t next_w_2;

  switch (opcode)
  {
  /*DATA TRANSFER*/
  /*Register/Memory to/from Register*/
  case 0x88:
    break;
  case 0x89:
    break;
  case 0x8A:
    break;
  case 0x8B:
    break;
  case 0xA0:
    break;
  case 0xA1:
    break;
  case 0xA2:
    break;
  case 0xA3:
    break;
  case 0xB0:
    break;
  case 0xB1:
    break;
  case 0xB2:
    break;
  case 0xB3:
    break;
  case 0xB4:
    break;
  case 0xB5:
    break;
  case 0xB6:
    break;
  case 0xB7:
    break;
  case 0xB8:
    break;
  case 0xB9:
    break;
  case 0xC0:
    break;
  case 0xC1:
    break;
  case 0xC2:
    break;
  case 0xC3:
    break;
  case 0xC4:
    break;
  case 0xC5:
    break;
    /*Immediate to register/memory*/
  case 0xC6: // byte
    uint8_t op = i8086_next_byte(p);
    decodeSecondOp(m_r_rm, op);
    uint8_t val = i8086_next_byte(p);
    if (m_r_rm->mod == 0b11) // immediate -> register
    {
    }
    break;
  case 0xC7: // word
    break;
  case 0x8C:
    break;
  case 0x8E:
    break;

  /*Processor control*/

  // Clear carry
  case 0xF8:
    p->cf = 0;
    break;
  // Complement carry
  case 0xF5:
    p->cf = !p->cf;
    break;
  // Set carry
  case 0xF9:
    p->cf = 1;
    break;
  // Clear direction
  case 0xFC:
    p->df = 0;
    break;
  // Set direction
  case 0xFD:
    p->df = 1;
    break;
  // Clear interrupt
  case 0xFA:
    p->iif = 0;
    break;
  // Set interrupt
  case 0xFB:
    p->iif = 1;
    break;
  // Halt
  case 0xF4:
    p->halted = 1;
    break;
  // Wait
  case 0x9B:
    p->interrupt_waiting = 1;
    break;
  // Escape (to external device)
  // NOT IMPLEMENTED YET
  case 0xD8:
    break;
  case 0xD9:
    break;
  case 0xDA:
    break;
  case 0xDB:
    break;
  case 0xDC:
    break;
  case 0xDD:
    break;
  case 0xDF:
    break;
  // Bus lock prefix
  // NOT IMPLEMENTED YET
  case 0xF0:
    break;

  default:
    break;
  }
}