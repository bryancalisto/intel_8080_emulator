#include "i8080.h"
#include "opcode_parser.h"

/* GENERIC MEMORY OPS */
// Read byte from memory
static inline uint8_t i8080_rb(i8080 *const p, uint16_t addr)
{
  return p->read_byte(addr);
}

// Write byte to memory
static inline void i8080_wb(i8080 *const p, uint16_t addr, uint8_t data)
{
  p->write_byte(addr, data);
}

static inline uint8_t i8080_next_b(i8080 *const p)
{
  uint16_t data = i8080_rb(p, p->pc);
  p->pc += 2;
  return data;
}

/* STACK OPS */
static inline void i8080_push_stack(i8080 *const p, uint8_t data)
{
  p->sp -= 2;
  i8080_ww(p, p->sp, data);
}

static inline uint8_t i8080_pop_stack(i8080 *const p)
{
  uint8_t data = i8080_rw(p, p->sp);
  p->sp += 2;
  return data;
}

/* GENERAL HELPERS */

static inline bool parity(uint8_t data)
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

/* Util to set flags that are commonly set together: Zero, signed, parity*/
void set_ZSP(i8080 *const p, uint16_t val)
{
  p->zf = val == 0;
  p->sf = (val >> 15);
  p->pf = parity(val);
}

/* Opcode helpers*/

/*ARITHMETIC*/
// THIS FUNCTION REQUIRES REFACTORING BECAUSE IT'S NOT ADAPTED TO 16 BIT PROCESSOR
static inline void i8080_add(i8080 *const p, uint16_t *const reg, uint16_t val, bool cy)
{
  uint16_t result = *reg + val + cy;
  p->cf = carry(8, *reg, val, cy);
  SET_ZSP(p, result);
  *reg = result;
}

// Executes one instruction
void i8080_exec(i8080 *const p, uint8_t opcode)
{
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
    break;
  case 0xC7: // word
    break;
  case 0x8C:
    break;
  case 0x8E:
    break;

  /*ARITHMETIC*/
  /*LOGIC*/
  /*STRING MANIPULATION*/
  case 0xF2: // REPNE (aka REPNZ)
    // Read next instruction and execute it while condition is true
    while (p->c != 0 || p->zf == 0)
    {
      p->c--;
    }
    break;
  case 0xF3: // REP (aka REPE or REPZ)
             // Read next instruction and execute it while condition is true
    uint8_t next_inst = i8080_next_b(p);
    while (p->c == 0 || p->zf != 0)
    {
      switch (next_inst)
      {
      case 0x6C:
        break;
      case 0x6D:
        break;
      }
      p->c--;
    }
    break;
  /*CONTROL TRANSFER*/

  /*PROCESSOR CONTROL*/

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
  case 0xFC:
    break;
  // Set direction
  case 0xFD:
    break;
  // Clear interrupt
  case 0xFA:
    break;
  // Set interrupt
  case 0xFB:
    break;
  // Halt
  case 0xF4:
    p->halted = 1;
    break;
  // Wait
  case 0x9B:
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