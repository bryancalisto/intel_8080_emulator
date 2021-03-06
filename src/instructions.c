#include "i8080.h"
#include "instructions.h"
#include "utils.h"
#include <stdlib.h>

void process_instruction(i8080 *p)
{
  uint8_t opcode = p->read_byte(p->pc++);
  uint16_t tmp_16;
  uint8_t tmp_8;

  switch (opcode)
  {
  case 0x00: // NOP
    break;
  case 0x01: // LXI B,D16
    p->c = p->read_byte(p->pc);
    p->b = p->read_byte(++p->pc);
    break;
  case 0x02: // STAX B
    p->write_byte(join_for_16_bit(p->b, p->c), p->a);
    break;
  case 0x03: // INX B
    tmp_16 = join_for_16_bit(p->b, p->c);
    tmp_16++;
    p->b = (uint8_t)tmp_16 >> 8;
    p->c = (uint8_t)tmp_16 & 0xff;
    break;
  case 0x04: // INR B
    update_acf(p, p->b, 1, "add");
    p->b++;
    update_z_s_p(p, p->b);
    break;
  case 0x05: // DCR B
    update_acf(p, p->b, 1, "sub");
    p->b--;
    update_z_s_p(p, p->b);
    break;
  case 0x06: // MVI B, D8
    p->b = p->read_byte(p->pc++);
    break;
  case 0x07: // RLC
    tmp_8 = p->a;
    p->a = (p->a << 1) | (tmp_8 >> 7);
    p->cf = (tmp_8 >> 7);
    break;
  case 0x08: // Undocumented
    break;
  case 0x09:
  {
    uint16_t hl = join_for_16_bit(p->h, p->l);
    uint16_t bc = join_for_16_bit(p->b, p->c);
    tmp_16 = hl + bc;
    p->h = (uint8_t)tmp_16 >> 8;
    p->l = (uint8_t)tmp_16 & 0xff;
    update_cf(p, hl, bc);
    break;
  }
  case 0x0a: // LDAX B
    p->b = p->read_byte(join_for_16_bit(p->b, p->c));
    break;
  case 0x0b: // DCR B
    tmp_16 = join_for_16_bit(p->b, p->c);
    tmp_16--;
    p->b = (uint8_t)tmp_16 >> 8;
    p->c = (uint8_t)tmp_16 & 0xff;
    break;
  case 0x0c: // INR C
    update_acf(p, p->c, 1, "add");
    p->c++;
    update_z_s_p(p, p->c);
    break;
  case 0x0d: // DCR C
    update_acf(p, p->c, 1, "sub");
    p->c--;
    update_z_s_p(p, p->c);
    break;
  case 0x0e: // MVI C, D8
    p->c = p->read_byte(p->pc++);
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
    p->d = p->read_byte(++p->pc);
    break;
  case 0x12: // STAX D
    p->write_byte(join_for_16_bit(p->d, p->e), p->a);
    break;
  case 0x13: // INX D
    tmp_16 = join_for_16_bit(p->d, p->e);
    tmp_16++;
    p->d = (uint8_t)tmp_16 >> 8;
    p->e = (uint8_t)tmp_16 & 0xff;
    break;
  case 0x14: // INR D
    update_acf(p, p->d, 1, "add");
    p->d++;
    update_z_s_p(p, p->d);
    break;
  case 0x15: // DCR D
    update_acf(p, p->d, 1, "sub");
    p->d--;
    update_z_s_p(p, p->d);
    break;
  case 0x16: // MVI D, D8
    p->d = p->read_byte(p->pc++);
    break;
  case 0x17: // RAL
    tmp_8 = p->a;
    p->a = (p->a << 1) | p->cf;
    p->cf = (tmp_8 >> 7);
    break;
  case 0x18: // Undocumented
    break;
  case 0x19: // DAD D
  {
    tmp_16 = join_for_16_bit(p->h, p->l) + join_for_16_bit(p->d, p->e);
    p->h = (uint8_t)tmp_16 >> 8;
    p->l = (uint8_t)tmp_16 & 0xff;
    // update carry flag
    uint32_t sum = tmp_16 + p->cf;
    p->cf = (sum >> 16);
    break;
  }
  case 0x1a: // LDAX D
    p->a = read_byte(p, join_for_16_bit(p->d, p->e));
    break;
  case 0x1b: // DCX D
    tmp_16 = join_for_16_bit(p->d, p->e);
    tmp_16--;
    p->d = (uint8_t)tmp_16 >> 8;
    p->e = (uint8_t)tmp_16 & 0xff;
    break;
  case 0x1c: // INR E
    update_acf(p, p->e, 1, "add");
    p->e++;
    update_z_s_p(p, p->e);
    break;
  case 0x1d: // DCR E
    update_acf(p, p->e, 1, "sub");
    p->e--;
    update_z_s_p(p, p->e);
    break;
  case 0x1e: // MVI E, D8
    p->e = p->read_byte(p->pc++);
    break;
  case 0x1f: // RAR
    tmp_8 = p->a;
    p->a = (p->a >> 1) | (tmp_8 & 0b10000000);
    p->cf = tmp_8 & 1;
    break;
  case 0x20:
    break;
  case 0x21: // LXI H,D16
    p->l = p->read_byte(p->pc);
    p->h = p->read_byte(++p->pc);
    break;
  case 0x22: // SHLD  addr
  {
    uint16_t addr = read_word(p, p->pc);
    p->pc += 2;
    p->write_byte(addr, p->l);
    p->write_byte(addr + 1, p->h);
    break;
  }
  case 0x23: // INX H
    tmp_16 = join_for_16_bit(p->h, p->l);
    tmp_16++;
    p->h = (uint8_t)tmp_16 >> 8;
    p->l = (uint8_t)tmp_16 & 0xff;
    break;
  case 0x24: // INR H
    update_acf(p, p->h, 1, "add");
    p->h++;
    update_z_s_p(p, p->h);
    break;
  case 0x25: // DCR H
    update_acf(p, p->h, 1, "sub");
    p->h--;
    update_z_s_p(p, p->h);
    break;
  case 0x26: // MVI H, D8
    p->h = p->read_byte(p->pc++);
    break;
  case 0x27: // DAA (especial). Not sure what to implement here yet
    if (((p->a & 0x0F) > 9) || (p->acf))
    {
      p->a += 0x06;
      p->acf = 1;
    }
    else
    {
      p->acf = 0;
    }

    if ((p->a > 0x9F) || (p->cf))
    {
      p->a += 0x60;
      p->cf = 1;
    }
    else
    {
      p->cf = 0;
    }

    update_zf_sf(p);
    break;
  case 0x28: // Undocumented
    break;
  case 0x29: // DAD H
  {
    tmp_16 = join_for_16_bit(p->h, p->l) + join_for_16_bit(p->h, p->l);
    p->h = (uint8_t)tmp_16 >> 8;
    p->l = (uint8_t)tmp_16 & 0xff;
    // update carry flag
    uint32_t sum = tmp_16 + p->cf;
    p->cf = (sum >> 16);
    break;
  }
  case 0x2a: // LHLD D
  {
    int16_t addr = read_word(p, p->pc);
    p->pc += 2;
    p->l = p->read_byte(addr);
    p->h = p->read_byte(addr + 1);
    break;
  }
  case 0x2b: // DCX H
    tmp_16 = join_for_16_bit(p->h, p->l);
    tmp_16--;
    p->h = (uint8_t)(tmp_16 >> 8);
    p->l = (uint8_t)(tmp_16 & 0xff);
    break;
  case 0x2c: // INR L
    update_acf(p, p->l, 1, "add");
    p->l++;
    update_z_s_p(p, p->l);
    break;
  case 0x2d: // DCR L
    update_acf(p, p->l, 1, "sub");
    p->l--;
    update_z_s_p(p, p->l);
    break;
  case 0x2e: // MVI L, D8
    p->l = p->read_byte(p->pc++);
    break;
  case 0x2f: // CMA
    p->a = !p->a;
    break;
  case 0x30: // Undocumented
    break;
  case 0x31: // LXI SP,D16
  {
    uint8_t low = p->read_byte(p->pc);
    uint8_t high = p->read_byte(++p->pc);
    p->sp = (high << 8) | low;
    break;
  }
  case 0x32: // STA  addr
  {
    uint16_t addr = read_word(p, p->pc);
    p->pc += 2;
    p->write_byte(addr, p->a);
    break;
  }
  case 0x33: // INX SP
    p->sp++;
    break;
  case 0x34: // INR M
  {
    tmp_16 = join_hl(p);
    uint8_t val = read_byte(p, tmp_16);
    val++;
    write_byte(p, tmp_16, val);
    update_z_s_p(p, val);
    break;
  }
  case 0x35: // DCR M
    tmp_16 = join_hl(p);
    uint8_t val = read_byte(p, tmp_16);
    val--;
    write_byte(p, tmp_16, val);
    update_z_s_p(p, val);
    break;
  case 0x36: // MVI M, D8
    write_byte(p, join_hl(p), p->read_byte(p->pc++));
    break;
  case 0x37: // STC
    p->cf = 1;
    break;
  case 0x38: // Undocumented
    break;
  case 0x39: // DAD SP
  {
    tmp_16 = join_for_16_bit(p->h, p->l) + p->sp;
    p->h = (uint8_t)(tmp_16 >> 8);
    p->l = (uint8_t)(tmp_16 & 0xff);
    // update carry flag
    uint32_t sum = tmp_16 + p->cf;
    p->cf = sum >> 16;
    break;
  }
  case 0x3a: // LDA addr
  {
    int16_t addr = read_word(p, p->pc);
    p->pc += 2;
    p->a = p->read_byte(addr);
    break;
  }
  case 0x3b: // DCX SP
    p->sp--;
    break;
  case 0x3c: // INR A
    update_acf(p, p->a, 1, "add");
    p->a++;
    update_z_s_p(p, p->a);
    break;
  case 0x3d: // DCR A
    update_acf(p, p->a, 1, "sub");
    p->a--;
    update_z_s_p(p, p->a);
    break;
  case 0x3e: // MVI A, D8
    p->a = p->read_byte(p->pc++);
    break;
  case 0x3f: // CMC
    p->cf = !p->cf;
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
  case 0x7f: // MOV A,A
    p->a = p->a;
    break;
  case 0x80: // ADD B
    add_byte(p, p->b, 0);
    break;
  case 0x81: // ADD C
    add_byte(p, p->c, 0);
    break;
  case 0x82: // ADD D
    add_byte(p, p->d, 0);
    break;
  case 0x83: // ADD E
    add_byte(p, p->e, 0);
    break;
  case 0x84: // ADD H
    add_byte(p, p->h, 0);
    break;
  case 0x85: // ADD L
    add_byte(p, p->l, 0);
    break;
  case 0x86: // ADD M
    add_byte(p, read_byte(p, join_hl(p)), 0);
    break;
  case 0x87: // ADD A
    add_byte(p, p->a, 0);
    break;
  case 0x88: // ADC B
    add_byte(p, p->b, p->cf);
    break;
  case 0x89: // ADC C
    add_byte(p, p->c, p->cf);
    break;
  case 0x8a: // ADC D
    add_byte(p, p->d, p->cf);
    break;
  case 0x8b: // ADC E
    add_byte(p, p->e, p->cf);
    break;
  case 0x8c: // ADC H
    add_byte(p, p->h, p->cf);
    break;
  case 0x8d: // ADC L
    add_byte(p, p->l, p->cf);
    break;
  case 0x8e: // ADC M
    add_byte(p, read_byte(p, join_hl(p)), p->cf);
    break;
  case 0x8f: // ADC A
    add_byte(p, p->a, p->cf);
    break;
  case 0x90: // SUB B
    p->a = sub_byte(p, p->b, 0);
    break;
  case 0x91: // SUB C
    p->a = sub_byte(p, p->c, 0);
    break;
  case 0x92: // SUB D
    p->a = sub_byte(p, p->d, 0);
    break;
  case 0x93: // SUB E
    p->a = sub_byte(p, p->e, 0);
    break;
  case 0x94: // SUB H
    p->a = sub_byte(p, p->h, 0);
    break;
  case 0x95: // SUB L
    p->a = sub_byte(p, p->l, 0);
    break;
  case 0x96: // SUB M
    p->a = sub_byte(p, read_byte(p, join_hl(p)), 0);
    break;
  case 0x97: // SUB A
    p->a = sub_byte(p, p->a, 0);
    break;
  case 0x98: // SBB B
    p->a = sub_byte(p, p->b, p->cf);
    break;
  case 0x99: // SBB C
    p->a = sub_byte(p, p->c, p->cf);
    break;
  case 0x9a: // SBB D
    p->a = sub_byte(p, p->d, p->cf);
    break;
  case 0x9b: // SBB E
    p->a = sub_byte(p, p->e, p->cf);
    break;
  case 0x9c: // SBB H
    p->a = sub_byte(p, p->h, p->cf);
    break;
  case 0x9d: // SBB L
    p->a = sub_byte(p, p->l, p->cf);
    break;
  case 0x9e: // SBB M
    p->a = sub_byte(p, read_byte(p, join_hl(p)), p->cf);
    break;
  case 0x9f: // SBB A
    p->a = sub_byte(p, p->a, p->cf);
    break;
  case 0xa0: // ANA B
    and_byte(p, p->b);
    break;
  case 0xa1: // ANA C
    and_byte(p, p->c);
    break;
  case 0xa2: // ANA D
    and_byte(p, p->d);
    break;
  case 0xa3: // ANA E
    and_byte(p, p->e);
    break;
  case 0xa4: // ANA H
    and_byte(p, p->h);
    break;
  case 0xa5: // ANA L
    and_byte(p, p->l);
    break;
  case 0xa6: // ANA M
    and_byte(p, read_byte(p, join_hl(p)));
    break;
  case 0xa7: // ANA A
    and_byte(p, p->a);
    break;
  case 0xa8: // XRA B
    xor_byte(p, p->b);
    break;
  case 0xa9: // XRA C
    xor_byte(p, p->c);
    break;
  case 0xaa: // XRA D
    xor_byte(p, p->d);
    break;
  case 0xab: // XRA E
    xor_byte(p, p->e);
    break;
  case 0xac: // XRA H
    xor_byte(p, p->h);
    break;
  case 0xad: // XRA L
    xor_byte(p, p->l);
    break;
  case 0xae: // XRA M
    xor_byte(p, read_byte(p, join_hl(p)));
    break;
  case 0xaf: // XRA A
    xor_byte(p, p->a);
    break;
  case 0xb0: // ORA B
    or_byte(p, p->b);
    break;
  case 0xb1: // ORA C
    or_byte(p, p->c);
    break;
  case 0xb2: // ORA D
    or_byte(p, p->d);
    break;
  case 0xb3: // ORA E
    or_byte(p, p->e);
    break;
  case 0xb4: // ORA H
    or_byte(p, p->h);
    break;
  case 0xb5: // ORA L
    or_byte(p, p->l);
    break;
  case 0xb6: // ORA M
    or_byte(p, read_byte(p, join_hl(p)));
    break;
  case 0xb7: // ORA A
    or_byte(p, p->a);
    break;
  case 0xb8: // CMP B
    cmp_byte(p, p->b);
    break;
  case 0xb9: // CMP C
    cmp_byte(p, p->c);
    break;
  case 0xba: // CMP D
    cmp_byte(p, p->d);
    break;
  case 0xbb: // CMP E
    cmp_byte(p, p->e);
    break;
  case 0xbc: // CMP H
    cmp_byte(p, p->h);
    break;
  case 0xbd: // CMP L
    cmp_byte(p, p->l);
    break;
  case 0xbe: // CMP M
    cmp_byte(p, read_byte(p, join_hl(p)));
    break;
  case 0xbf: // CMP A
    cmp_byte(p, p->a);
    break;
  case 0xc0: // RNZ
    if (!p->zf)
    {
      ret(p);
    }
    break;
  case 0xc1: // POP B
  {
    uint16_t val = stack_pop(p);
    p->b = val >> 8;
    p->c = val & 0xff;
    break;
  }
  case 0xc2: // JNZ adr
    if (!p->zf)
    {
      p->pc = read_word(p, p->pc);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xc3: // JMP adr
    p->pc = read_word(p, p->pc);
    break;
  case 0xc4: // CNZ adr
    if (!p->zf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc += 2;
      call(p, addr);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xc5: // PUSH B
    stack_push(p, (p->b << 8) | p->c);
    break;
  case 0xc6: // ADI D8
    add_byte(p, read_byte(p, p->pc), 0);
    p->pc++;
    break;
  case 0xc7: // RST 0
    call(p, 0);
    break;
  case 0xc8: // RZ
    if (p->zf)
    {
      ret(p);
    }
    break;
  case 0xc9: // RET
    ret(p);
    break;
  case 0xca: // JZ adr
    if (p->zf)
    {
      p->pc = read_word(p, p->pc);
    }
    p->pc += 2;
    break;
  case 0xcb: // Undocumented
    break;
  case 0xcc: // CZ adr
    if (p->zf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc += 2;
      call(p, addr);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xcd: // CALL addr
  {
    uint16_t addr = read_word(p, p->pc);
    p->pc += 2;
    call(p, addr);
    break;
  }
  case 0xce: // ACI D8
    add_byte(p, read_byte(p, p->pc), p->cf);
    p->pc++;
    break;
  case 0xcf: // RST 1
    call(p, 0x8);
    break;
  case 0xd0: // RNC
    if (!p->cf)
    {
      ret(p);
    }
    break;
  case 0xd1: // POP D
  {
    uint16_t val = read_word(p, p->sp);
    p->d = val >> 8;
    p->e = val & 0xff;
    p->sp += 2;
    break;
  }
  case 0xd2: // JNC addr
    if (!p->cf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc = addr;
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xd3: // OUT D8 (Especial)
    non_implem_error(opcode);
    break;
  case 0xd4: // CNC addr
    if (!p->cf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc += 2;
      call(p, addr);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xd5: // PUSH D
    stack_push(p, p->d << 8 | p->e);
    break;
  case 0xd6: // SUI D8
    p->a = sub_byte(p, read_byte(p, p->pc), 0);
    p->pc++;
    break;
  case 0xd7: // RST 2
    call(p, 0x10);
    break;
  case 0xd8: // RC
    if (p->cf)
    {
      ret(p);
    }
    break;
  case 0xd9: // Undocumented
    break;
  case 0xda: // JC addr
    if (p->cf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc = addr;
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xdb: // IN D8 (Especial)
    non_implem_error(opcode);
    break;
  case 0xdc: // CC addr
    if (p->cf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc += 2;
      call(p, addr);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xdd: // Undocumented
    break;
  case 0xde: // SBI D8
    p->a = sub_byte(p, read_byte(p, p->pc), p->cf);
    p->pc++;
    break;
  case 0xdf: // RST 3
    call(p, 0x18);
    break;
  case 0xe0: // RPO
    if (!p->pf)
    {
      ret(p);
    }
    break;
  case 0xe1: // POP H
  {
    uint16_t val = stack_pop(p);
    p->h = val >> 8;
    p->l = val & 0xff;
    break;
  }
  case 0xe2: // JPO addr
    if (!p->pf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc = addr;
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xe3: // XTHL
  {
    uint16_t val = read_word(p, p->sp);
    write_word(p, p->sp, join_hl(p));
    p->h = val >> 8;
    p->l = val & 0xff;
    break;
  }
  case 0xe4: // CPO addr
    if (!p->pf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc += 2;
      call(p, addr);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xe5: // PUSH H
    stack_push(p, (p->h << 8) | p->l);
    break;
  case 0xe6: // ANI D8
    and_byte(p, read_byte(p, p->pc));
    p->cf = 0;
    update_z_s_p(p, p->a);
    p->pc++;
    break;
  case 0xe7: // RST 4
    call(p, 0x20);
    break;
  case 0xe8: // RPE
    if (p->pf)
    {
      ret(p);
    }
    break;
  case 0xe9: // PCHL
    p->pc = (p->h << 8) | p->l;
    break;
  case 0xea: // JPE addr
    if (p->pf)
    {
      p->pc = read_word(p, p->pc);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xeb: // XCHG
  {
    uint8_t tmp = p->h;
    p->h = p->d;
    p->d = tmp;
    tmp = p->l;
    p->l = p->e;
    p->e = tmp;
    break;
  }
  case 0xec: // CPE addr
    if (p->pf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc += 2;
      call(p, addr);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xed: // Undocumented
    break;
  case 0xee: // XRI D8
    xor_byte(p, read_byte(p, p->pc));
    p->cf = 0;
    update_z_s_p(p, p->a);
    p->pc++;
    break;
  case 0xef: // RST 5
    call(p, 0x28);
    break;
  case 0xf0: // RP
    if (p->sf)
    {
      ret(p);
    }
    break;
  case 0xf1: // POP PSW
  {
    uint16_t flags = stack_pop(p);
    p->a = flags >> 8;
    uint8_t psw = flags & 0xFF;

    p->sf = (psw >> 7) & 1;
    p->zf = (psw >> 6) & 1;
    p->acf = (psw >> 4) & 1;
    p->pf = (psw >> 2) & 1;
    p->cf = (psw >> 0) & 1;
    break;
  }
  case 0xf2: // JP addr
    if (!p->sf)
    {
      p->pc = read_byte(p, p->pc);
    }
    break;
  case 0xf3: // Especial
    break;
  case 0xf4: // CP addr
    if (p->sf)
    {
      p->pc = read_byte(p, p->pc);
    }
    break;
  case 0xf5: // PUSH PSW
  {
    uint8_t psw = 0;
    psw |= p->sf << 7;
    psw |= p->zf << 6;
    psw |= p->acf << 4;
    psw |= p->pf << 2;
    psw |= 1 << 1;
    psw |= p->cf << 0;
    stack_push(p, (p->a << 8) | psw);
    break;
  }
  case 0xf6: // ORI D8
    or_byte(p, read_byte(p, p->pc));
    p->cf = 0;
    update_z_s_p(p, p->a);
    p->pc++;
    break;
  case 0xf7: // RST 6
    call(p, 0x30);
    break;
  case 0xf8: // RP
    if (p->sf)
    {
      ret(p);
    }
    break;
  case 0xf9: // SPHL
    p->sp = join_hl(p);
    break;
  case 0xfa: // JM addr
    if (p->sf)
    {
      p->pc = read_byte(p, p->pc);
    }
    break;
  case 0xfb: // Especial
    break;
  case 0xfc:
    if (p->sf)
    {
      uint16_t addr = read_word(p, p->pc);
      p->pc += 2;
      call(p, addr);
    }
    else
    {
      p->pc += 2;
    }
    break;
  case 0xfd: // Undocumented
    break;
  case 0xfe: // CPI D8
    sub_byte(p, read_byte(p, p->pc), 0);
    p->pc++;
    break;
  case 0xff: // RST 7
    call(p, 0x38);
    break;
  default:
    fprintf(stderr, "Unrecognized opcode: %x\n", opcode);
    exit(-1);
    break;
  }
}