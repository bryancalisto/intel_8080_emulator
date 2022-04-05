#include "i8080.h"
#include "instructions.h"
#include <stdlib.h>
#include <string.h>

void non_implem_error(uint8_t opcode)
{
  fprintf(stderr, "Not implemented: %x\n", opcode);
  exit(-1);
}

// Joins registers h and l to form a 16 bit address
uint16_t join_hl(i8080 *p)
{
  return (p->h << 8) | p->l;
}

// Joins two registers to form a 16 bit address
uint16_t join_for_16_bit(uint8_t high, uint8_t low)
{
  return (high << 8) | low;
}

void write_byte(i8080 *p, uint16_t addr, uint8_t data)
{
  p->write_byte(addr, data);
}

void write_word(i8080 *p, uint16_t addr, uint16_t data)
{
  p->write_byte(addr + 1, data >> 8);
  p->write_byte(addr, data & 0xff);
}

uint8_t read_byte(i8080 *p, uint16_t addr)
{
  return p->read_byte(addr);
}

uint16_t read_word(i8080 *p, uint16_t addr)
{
  uint16_t hi = p->read_byte(addr + 1) << 8;
  return hi | p->read_byte(addr);
}

bool parity(uint8_t value)
{
  uint8_t ones = 0;

  for (int i = 0; i < 8; i++)
  {
    ones += ((value >> i) & 1);
  }
  return ones % 2 == 0;
}

// Updates the auxiliary carry flag
void update_acf(i8080 *p, uint8_t a, uint8_t b, char *mode)
{
  if (strcmp(mode, "add") == 0)
  {
    uint8_t sum = (a & 0xf) + (b & 0xf);
    p->acf = sum >> 3;
  }
  else if (strcmp(mode, "sub") == 0)
  {
    uint8_t sub = (a & 0xf) - (b & 0xf);
    p->acf = sub & 0x10;
  }
  else if (strcmp(mode, "or") == 0)
  {
    uint8_t sub = (a & 0xf) - (b & 0xf);
    p->acf = sub & 0x10;
  }
  else if (strcmp(mode, "and") == 0)
  {
    uint8_t sub = (a & 0xf) - (b & 0xf);
    p->acf = sub & 0x10;
  }
  else if (strcmp(mode, "xor") == 0)
  {
    uint8_t sub = (a & 0xf) - (b & 0xf);
    p->acf = sub & 0x10;
  }
  else
  {
    fprintf(stderr, "Unsupported mode: %s\n", mode);
    exit(-1);
  }
}

void update_z_s_p(i8080 *p, uint8_t value)
{
  p->zf = value == 0;
  p->sf = (value >> 7);
  p->pf = parity(value);
}

void update_cf(i8080 *p, uint8_t val_1, uint8_t val_2)
{
  uint16_t sum = val_1 + val_2 + p->cf;
  p->cf = (sum >> 8) > 0;
}

void update_zf_sf(i8080 *p)
{
  p->zf = p->a == 0;
  p->sf = p->a & 0x80;
}

void add_byte(i8080 *p, uint8_t to_add, uint8_t carry)
{
  uint16_t value = (p->a + to_add + carry);
  p->acf = (p->a ^ to_add ^ value) & 0x10;
  p->a = (uint8_t)value;
  p->cf = value > 255;
  update_zf_sf(p);
}

uint8_t sub_byte(i8080 *p, uint8_t subt, uint8_t borrow)
{
  uint8_t subt_ones_comp = (~subt);
  // One's complement way
  uint16_t res = p->a + subt_ones_comp + (borrow ? 0 : 1);
  p->cf = !(res & 0x100);
  p->acf = ((p->a & 0xF) + (subt_ones_comp & 0xF) + (borrow ? 0 : 1)) & 0x10;
  return res & 0xff;
}

void and_byte(i8080 *p, uint8_t to_and)
{
  p->a = p->a & to_and;
}

void xor_byte(i8080 *p, uint8_t to_xor)
{
  p->a = p->a ^ to_xor;
}

void or_byte(i8080 *p, uint8_t to_or)
{
  p->a = p->a | to_or;
}

void cmp_byte(i8080 *p, uint8_t to_cmp)
{
  uint8_t res = p->a - to_cmp;
  update_acf(p, p->a, to_cmp, "sub");
  update_z_s_p(p, res);
}

void stack_push(i8080 *p, uint16_t to_push)
{
  p->sp -= 2;
  write_word(p, p->sp, to_push);
}

uint16_t stack_pop(i8080 *p)
{
  uint16_t val = read_word(p, p->sp);
  p->sp += 2;
  return val;
}

void ret(i8080 *p)
{
  p->pc = stack_pop(p);
  p->sp += 2;
}

void call(i8080 *p, uint16_t addr)
{
  stack_push(p, p->pc);
  p->pc = addr;
}