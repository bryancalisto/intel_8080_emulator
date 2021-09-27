#include "i8080.h"
#include "instructions.h"

// Joins registers h and l to form a 16 bit address
static inline uint16_t join_hl(i8080 *p)
{
  return (p->h << 8) | p->l;
}

// Joins two registers to form a 16 bit address
static inline uint16_t join_for_16_bit(uint8_t high, uint8_t low)
{
  return (high << 8) | low;
}

static inline uint8_t read_byte(i8080 *p, uint16_t addr)
{
  return p->read_byte(addr);
}

static inline bool parity(uint8_t value)
{
  uint8_t ones = 0;

  for (int i = 0; i < 8; i++)
  {
    ones += ((value >> i) & 1);
  }

  return (ones & 1) == 0;
}

static inline void update_z_s_p_ac(i8080 *p, uint8_t value)
{
  p->zf = value == 0;
  p->sf = (value >> 7);
  p->pf = parity(value);
  p->acf = (value & 0xf) == 0;
}

static inline void update_cf(i8080 *p, uint8_t val_1, uint8_t val_2)
{
  uint16_t sum = val_1 + val_2 + p->cf;
  p->cf = (sum >> 8) > 0;
}

static inline void write_byte(i8080 *p, uint16_t addr, uint8_t data)
{
  p->write_byte(addr, data);
}

void process_instruction(i8080 *p)
{
  uint16_t opcode = p->read_byte(p->pc);
  uint16_t tmp_16;
  uint8_t tmp_8;

  switch (opcode)
  {
  case 0x00: // NOP
    break;
  case 0x01: // LXI B,D16
    p->c = p->read_byte(p->pc);
    p->b = p->read_byte(p->pc);
    break;
  case 0x02: // STAX B
    p->write_byte(join_for_16_bit(p->b, p->c), p->a);
    break;
  case 0x03: // INX B
    tmp_16 = join_for_16_bit(p->b, p->c);
    tmp_16++;
    p->b = (uint8_t)tmp_16 >> 8;
    p->c = (uint8_t)tmp_16;
    break;
  case 0x04: // INR B
    p->b++;
    update_z_s_p_ac(p, p->b);
    break;
  case 0x05: // DCR B
    p->b--;
    update_z_s_p_ac(p, p->b);
    break;
  case 0x06: // MVI B, D8
    p->b = p->read_byte(p->pc);
    break;
  case 0x07: // RLC
    tmp_8 = p->a;
    p->a = (p->a << 1) | (tmp_8 >> 7);
    p->cf = (tmp_8 >> 7);
    break;
  case 0x08: // Undocumented
    break;
  case 0x09:
    uint16_t hl = join_for_16_bit(p->h, p->l);
    uint16_t bc = join_for_16_bit(p->b, p->c);
    tmp_16 = hl + bc;
    p->h = (uint8_t)tmp_16 >> 8;
    p->l = (uint8_t)tmp_16;
    update_cf(p, hl, bc);
    break;
  case 0x0a: // LDAX B
    p->b = p->read_byte(join_for_16_bit(p->b, p->c));
    break;
  case 0x0b: // DCR B
    tmp_16 = join_for_16_bit(p->b, p->c);
    tmp_16--;
    p->b = (uint8_t)tmp_16 >> 8;
    p->c = (uint8_t)tmp_16;
    break;
  case 0x0c: // INR C
    p->c++;
    update_z_s_p_ac(p, p->c);
    break;
  case 0x0d: // DCR C
    p->c--;
    update_z_s_p_ac(p, p->c);
    break;
  case 0x0e: // MVI C, D8
    p->c = p->read_byte(p->pc);
    break;
  case 0x0f: // RRC
    tmp_8 = p->a;
    p->a = (p->a >> 1) | (tmp_8 << 7);
    p->cf = (tmp_8 & 1);
    break;
  case 0x10: // Undocumented
    break;
  case 0x11: // LXI D,D16
    p->e = p->read_byte(p->pc);
    p->d = p->read_byte(p->pc);
    break;
  case 0x12: // STAX D
    p->write_byte(join_for_16_bit(p->d, p->e), p->a);
    break;
  case 0x13: // INX D
    tmp_16 = join_for_16_bit(p->d, p->e);
    tmp_16++;
    p->d = (uint8_t)tmp_16 >> 8;
    p->e = (uint8_t)tmp_16;
    break;
  case 0x14: // INR D
    p->d++;
    update_z_s_p_ac(p, p->d);
    break;
  case 0x15: // DCR D
    p->d--;
    update_z_s_p_ac(p, p->d);
    break;
  case 0x16: // MVI D, D8
    p->d = p->read_byte(p->pc);
    break;
  case 0x17: // RAL
    tmp_8 = p->a;
    p->a = (p->a << 1) | p->cf;
    p->cf = (tmp_8 >> 7);
    break;
  case 0x18: // Undocumented
    break;
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
  case 0x4a: // MOV C,D
    p->c = p->d;
    break;
  case 0x4b: // MOV C,E
    p->c = p->e;
    break;
  case 0x4c: // MOV C,H
    p->c = p->h;
    break;
  case 0x4d: // MOV C,L
    p->c = p->l;
    break;
  case 0x4e: // MOV C,M
    p->c = read_byte(p, join_hl(p));
    break;
  case 0x4f: // MOV C,A
    p->c = p->a;
    break;
  case 0x50: // MOV, D,B
    p->d = p->b;
    break;
  case 0x51: // MOV D,C
    p->d = p->c;
    break;
  case 0x52: // MOV D,D
    p->d = p->d;
    break;
  case 0x53: // MOV D,E
    p->d = p->e;
    break;
  case 0x54: // MOV D,H
    p->d = p->h;
    break;
  case 0x55: // MOV D,L
    p->d = p->l;
    break;
  case 0x56: // MOV D,M
    p->d = read_byte(p, join_hl(p));
    break;
  case 0x57: // MOV D,A
    p->d = p->a;
    break;
  case 0x58: // MOV E,B
    p->e = p->b;
    break;
  case 0x59: // MOV E,C
    p->e = p->c;
    break;
  case 0x5a: // MOV E,D
    p->e = p->d;
    break;
  case 0x5b: // MOV E,E
    p->e = p->e;
    break;
  case 0x5c: // MOV E,H
    p->e = p->h;
    break;
  case 0x5d: // MOV E,L
    p->e = p->l;
    break;
  case 0x5e: // MOV E,M
    p->e = read_byte(p, join_hl(p));
    break;
  case 0x5f: // MOV E,A
    p->e = p->a;
    break;
  case 0x60: // MOV H,B
    p->h = p->b;
    break;
  case 0x61: // MOV H,C
    p->h = p->c;
    break;
  case 0x62: // MOV H,D
    p->h = p->d;
    break;
  case 0x63: // MOV H,E
    p->h = p->e;
    break;
  case 0x64: // MOV H,H
    p->h = p->h;
    break;
  case 0x65: // MOV H,L
    p->h = p->l;
    break;
  case 0x66: // MOV H,M
    p->h = read_byte(p, join_hl(p));
    break;
  case 0x67: // MOV H,A
    p->h = p->a;
    break;
  case 0x68: // MOV L,B
    p->l = p->b;
    break;
  case 0x69: // MOV L,C
    p->l = p->c;
    break;
  case 0x6a: // MOV L,D
    p->l = p->d;
    break;
  case 0x6b: // MOV L,E
    p->l = p->e;
    break;
  case 0x6c: // MOV L,H
    p->l = p->h;
    break;
  case 0x6d: // MOV L,L
    p->l = p->l;
    break;
  case 0x6e: // MOV L,M
    p->l = read_byte(p, join_hl(p));
    break;
  case 0x6f: // MOV L,A
    p->l = p->a;
    break;
  case 0x70: // MOV M,B
    write_byte(p, join_hl(p), p->b);
    break;
  case 0x71: // MOV M,C
    write_byte(p, join_hl(p), p->c);
    break;
  case 0x72: // MOV M,D
    write_byte(p, join_hl(p), p->d);
    break;
  case 0x73: // MOV M,E
    write_byte(p, join_hl(p), p->e);
    break;
  case 0x74: // MOV M,H
    write_byte(p, join_hl(p), p->h);
    break;
  case 0x75: // MOV M,L
    write_byte(p, join_hl(p), p->l);
    break;
  case 0x76: // HLT
             // IMPLEMENTATION PENDING
    break;
  case 0x77: // MOV M,A
    write_byte(p, join_hl(p), p->a);
    break;
  case 0x78: // MOV A,B
    p->a = p->b;
    break;
  case 0x79: // MOV A,C
    p->a = p->c;
    break;
  case 0x7a: // MOV A,D
    p->a = p->d;
    break;
  case 0x7b: // MOV A,E
    p->a = p->e;
    break;
  case 0x7c: // MOV A,H
    p->a = p->h;
    break;
  case 0x7d: // MOV A,L
    p->a = p->l;
    break;
  case 0x7e: // MOV A,M
    p->a = read_byte(p, join_hl(p));
    break;
  case 0x7f: // MOV a,A
    p->a = p->a;
    break;
  default:
    break;
  }
}