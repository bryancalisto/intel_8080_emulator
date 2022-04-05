#ifndef UTILS_H
#define UTILS_H
#include "i8080.h"

void non_implem_error(uint8_t opcode);

// Joins registers h and l to form a 16 bit address
uint16_t join_hl(i8080 *p);

// Joins two registers to form a 16 bit address
uint16_t join_for_16_bit(uint8_t high, uint8_t low);

uint8_t read_byte(i8080 *p, uint16_t addr);

uint16_t read_word(i8080 *p, uint16_t addr);

bool parity(uint8_t value);

bool auxiliary_carry(uint8_t a, uint8_t b);

void update_z_s_p_ac(i8080 *p, uint8_t value);

void update_cf(i8080 *p, uint8_t val_1, uint8_t val_2);

void write_byte(i8080 *p, uint16_t addr, uint8_t data);

void write_word(i8080 *p, uint16_t addr, uint16_t data);

void update_zf_sf(i8080 *p);

void add_byte(i8080 *p, uint8_t to_add, uint8_t carry);

uint8_t sub_byte(i8080 *p, uint8_t subt, uint8_t borrow);

void and_byte(i8080 *p, uint8_t to_and);

void xor_byte(i8080 *p, uint8_t to_xor);

void or_byte(i8080 *p, uint8_t to_or);

void cmp_byte(i8080 *p, uint8_t to_cmp);

void stack_push(i8080 *p, uint16_t to_push);

uint16_t stack_pop(i8080 *p);

void ret(i8080 *p);

void call(i8080 *p, uint16_t addr);

#endif // UTILS_H