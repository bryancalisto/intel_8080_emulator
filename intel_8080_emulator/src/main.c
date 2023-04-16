#include <stdio.h>
#include "i8080.h"

uint8_t memory[] = {0x6, 0x1, 0x4, 0x4, 0x4}; // Puts 1 in register B and increases register B 3 times
const size_t memory_length = 5;

static uint8_t read_byte_from_memory(uint16_t addr)
{
  return memory[addr];
}

static void write_byte_to_memory(uint16_t addr, uint8_t val)
{
  memory[addr] = val;
}

int main()
{
  i8080 proc;

  i8080_init(&proc);
  proc.read_byte = &read_byte_from_memory;
  proc.write_byte = &write_byte_to_memory;

  while (proc.pc < memory_length)
  {
    i8080_step(&proc);
  }

  printf("Register B => %d\n", proc.b);
}
