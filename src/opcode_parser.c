#include <i8086.h>

/*OPCODE DECODE HELPERS*/

// mod reg r/m
uint8_t parse_mod(uint8_t byte)
{
  return (byte >> 6) & 0b11;
}

uint8_t parse_reg(uint8_t byte)
{
  return byte & 0b111;
}

uint8_t parse_r_m(uint8_t byte)
{
  return (byte >> 3) & 0b111;
}

// 'w' bit. This defines if the operators are word (true) or byte (false)
bool parse_w(uint8_t byte)
{
  return byte & 0b1;
}

// 'd' bit. This defines if the 'reg' is the destiny (true) or the source (false)
bool parse_d(uint8_t byte)
{
  return (byte >> 1) & 0b1;
}

// 'sw' bits. This defines signed word expansion behavior in some instructions
uint8_t parse_sw(uint8_t byte)
{
  return byte && 0b11;
}

// Returns the address of a processor 16 bit general purpose register based on 'reg' 3 bits
uint16_t *get_reg(i8086 *const p, uint8_t reg, bool word)
{
  if (word)
  {
    switch (reg)
    {
    case 0:
      return &p->a;
    case 1:
      return &p->c;
    case 2:
      return &p->d;
    case 3:
      return &p->b;
    case 4:
      return &p->sp;
    case 5:
      return &p->bp;
    case 6:
      return &p->si;
    case 7:
      return &p->di;
    default:
      printf("PROBLEM MAY BE HERE: get_reg_w()");
      exit(-1);
    }
  }
  else
  {
    switch (reg)
    {
    case 0:
      return &p->al;
    case 1:
      return &p->dl;
    case 2:
      return &p->cl;
    case 3:
      return &p->bl;
    case 4:
      return &p->ah;
    case 5:
      return &p->ch;
    case 6:
      return &p->dh;
    case 7:
      return &p->bh;
    default:
      printf("PROBLEM MAY BE HERE: get_reg_b()");
      exit(-1);
    }
  }
}

// Returns a pointer to the corresponding processor segment register, based on 'reg' 2 bits
uint16_t *get_sreg(i8086 *const p, uint8_t reg)
{
  switch (reg)
  {
  case 0:
    return &p->es;
  case 1:
    return &p->cs;
  case 2:
    return &p->ss;
  case 3:
    return &p->ds;
  default:
    printf("PROBLEM MAY BE HERE: get_sreg()");
    exit(-1);
  }
}

// Returns the EA (efective address) based on r/m field
uint32_t get_ea_from_rm(i8086 *const p, uint8_t rm, uint8_t mod)
{
  uint8_t left = 0;
  uint8_t right = 0;
  uint32_t disp = 0; // mod => 00

  if (mod == 0b01) // 8 bit displacement (sign extended)
  {
    disp = p->read_byte(p->pc++);
  }
  else if (mod == 0b10) // 16 bit displacement
  {
    left = p->read_byte(p->pc++);
    right = p->read_byte(p->pc++);
    disp = ((uint32_t)left << 8) | right;
  }

  switch (rm)
  {
  case 0:
    return p->b + p->si + disp;
  case 1:
    return p->b + p->di + disp;
  case 2:
    return p->bp + p->si + disp;
  case 3:
    return p->bp + p->di + disp;
  case 4:
    return p->si + disp;
  case 5:
    return p->di + disp;
  case 6:
    if (mod == 0) // Acording to intel docs
    {
      left = p->read_byte(p->pc++);
      right = p->read_byte(p->pc++);
      return ((uint32_t)left << 8) | right;
    }
    return p->bp + disp;
  case 7:
    return p->b + disp;
  default:
    printf("PROBLEM MAY BE HERE: get_ea_from_rm()");
    exit(-1);
  }
}