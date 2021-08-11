#include "i8080.h"
#include "instructions.h"

// Joins registers h and l to form a 16 bit address
static inline uint16_t join_hl(i8080 *p)
{
  return (p->h << 8) | p->l;
}

static inline uint8_t read_byte(i8080 *p, uint16_t addr)
{
  return p->read_byte(addr);
}

static inline void write_byte(i8080 *p, uint16_t addr, uint8_t data)
{
  p->write_byte(addr, data);
}

void step_instruction(i8080 *p)
{
  uint16_t opcode = p->read_byte(p->pc);

  switch (opcode)
  {
  /*DATA TRANSFER*/
  // Move register
  case 0x40: // MOV B,B
    p->b = p->b;
    break;
  case 0x41: // MOV B,C
    p->b = p->c;
    break;
  case 0x42: // MOV B,D
    p->b = p->d;
    break;
  case 0x43: // MOV B,E
    p->b = p->e;
    break;
  case 0x44: // MOV B,H
    p->b = p->h;
    break;
  case 0x45: // MOV B,L
    p->b = p->l;
    break;
  case 0x46: // MOV B,M
    p->b = read_byte(p, join_hl(p));
    break;
  case 0x47: // MOV B,A
    p->b = p->a;
    break;
  case 0x48: // MOV C,B
    p->c = p->b;
    break;
  case 0x49: // MOV C,C
    p->c = p->c;
    break;
  default:
    break;
  }
}