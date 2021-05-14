#ifndef OPCODE_PARSER_H
#define OPCODE_PARSER_H

#include <i8086.h>

uint8_t parse_mod(uint8_t);
uint8_t parse_reg(uint8_t);
uint8_t parse_r_m(uint8_t);

bool parse_w(uint8_t);
bool parse_d(uint8_t);
uint8_t parse_sw(uint8_t);

uint16_t *get_reg(i8086 *const, uint8_t, bool);
uint16_t *get_sreg(i8086 *const, uint8_t);
uint32_t get_ea_from_rm(i8086 *const, uint8_t, uint8_t);

#endif // OPCODE_PARSER_H