#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i8080.h"
#include "utils.h"

#define MEM_SIZE 65535

static uint8_t memory[MEM_SIZE] = {0};

static void clear_memory()
{
  memset(memory, 0, MEM_SIZE);
}

// The actual implementations of the i8080 structure function pointers
static uint8_t read_byte_implementation(uint16_t addr)
{
  return memory[addr];
}

static void write_byte_implementation(uint16_t addr, uint8_t val)
{
  memory[addr] = val;
}

static int setup(void **state)
{
  i8080 *cpu = malloc(sizeof(i8080));

  if (cpu == NULL)
  {
    return -1;
  }

  cpu->read_byte = &read_byte_implementation;
  cpu->write_byte = &write_byte_implementation;

  *state = cpu;

  return 0;
}

static int teardown(void **state)
{
  free(*state);
  return 0;
}

static void join_hl_ok(void **state)
{
  i8080 *cpu = *state;
  cpu->h = 0b11111111;
  cpu->l = 0b00000000;
  uint16_t hl = join_hl(cpu);
  assert_true(hl == 0b1111111100000000);
}

static void join_for_16_bit_ok(void **state)
{
  uint8_t a = 0b11111111;
  uint8_t b = 0b00000000;
  uint16_t res = join_for_16_bit(a, b);
  assert_true(res == 0b1111111100000000);
}

static void write_byte_ok(void **state)
{
  i8080 *p = *state;
  uint8_t data1 = 0xaf;
  uint16_t addr1 = 0x0;
  uint8_t data2 = 0x6d;
  uint16_t addr2 = 0xf14a;

  clear_memory();

  write_word(p, addr1, data1);
  write_word(p, addr2, data2);

  assert_true(memory[addr1] == 0xaf);
  assert_true(memory[addr2] == 0x6d);
}

static void write_word_ok(void **state)
{
  i8080 *p = *state;
  uint16_t data1 = 0xaf12;
  uint16_t addr1 = 0x0;
  uint16_t data2 = 0x6d90;
  uint16_t addr2 = 0xf14a;

  clear_memory();

  write_word(p, addr1, data1);
  write_word(p, addr2, data2);

  assert_true(memory[addr1] == 0x12);
  assert_true(memory[addr1 + 1] == 0xaf);
  assert_true(memory[addr2] == 0x90);
  assert_true(memory[addr2 + 1] == 0x6d);
}

static void read_byte_ok(void **state)
{
  i8080 *p = *state;
  uint8_t data1 = 0xaf;
  uint16_t addr1 = 0x0;
  uint8_t data2 = 0x6d;
  uint16_t addr2 = 0xffff;

  clear_memory();

  memory[addr1] = data1;
  memory[addr2] = data2;

  assert_true(read_byte(p, addr1) == data1);
  assert_true(read_byte(p, addr2) == data2);
}

static void read_word_ok(void **state)
{
  i8080 *p = *state;
  uint16_t data1 = 0xaf12;
  uint16_t addr1 = 0x0;
  uint16_t data2 = 0x6d90;
  uint16_t addr2 = 0xfff3;

  clear_memory();

  write_word(p, addr1, data1); // This was tested before,
  write_word(p, addr2, data2); // so trust it

  assert_true(read_word(p, addr1) == data1);
  assert_true(read_word(p, addr2) == data2);
}

static void parity_ok()
{
  assert_true(parity(0b10101010) == true);
  assert_true(parity(0b00000000) == true);
  assert_true(parity(0b11111110) == false);
}

static void update_acf_ok__sum(void **state)
{
  i8080 *p = *state;

  // Set
  update_acf(p, 0b00000101, 0b00000100, "add");
  assert_true(p->acf == 1);

  p->acf = 0;

  // Unset
  update_acf(p, 0b00000100, 0b00000011, "add");
  assert_true(p->acf == 0);
}

// static void update_z_s_p_ac__negatives__ok(void **state)
// {
//   i8080 *p = *state;
//   uint8_t value = 0b11111111; // 255

//   update_z_s_p_ac(p, value);

//   assert_true(p->zf == 0);
//   assert_true(p->sf == 1);
//   assert_true(p->pf == true);
//   assert_true(p->acf ==);
// }

int main(void)
{
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup_teardown(join_hl_ok, setup, teardown),
      cmocka_unit_test_setup_teardown(join_for_16_bit_ok, setup, teardown),
      cmocka_unit_test_setup_teardown(write_byte_ok, setup, teardown),
      cmocka_unit_test_setup_teardown(write_word_ok, setup, teardown),
      cmocka_unit_test_setup_teardown(read_byte_ok, setup, teardown),
      cmocka_unit_test_setup_teardown(read_word_ok, setup, teardown),
      cmocka_unit_test(parity_ok),
      cmocka_unit_test_setup_teardown(update_acf_ok__sum, setup, teardown),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}