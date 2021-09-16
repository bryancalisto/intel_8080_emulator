#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i8080.h"

#define MEMORY_SIZE 0x10000 // 65536 bytes
static uint8_t *memory = NULL;

// The actual implementations of the i8080 structure function pointers
static uint8_t read_byte(uint16_t addr)
{
  return memory[addr];
}

static void write_byte(uint16_t addr, uint8_t val)
{
  memory[addr] = val;
}

int main()
{
  memory = (uint8_t *)calloc(MEMORY_SIZE, sizeof(uint8_t));

  if (memory == NULL)
  {
    printf("Could not allocate enough memory (%x) for the test\n", MEMORY_SIZE);
    return -1;
  }

  i8080 cpu;
  cpu.read_byte = &read_byte;
  cpu.write_byte = &write_byte;

  // JUST TO TEST THAT IT WORKS  - STARTS
  i8080_init(&cpu);
  cpu.c = 0x69;
  printf("B before MOV B,C (C is 0x69): %d\n", cpu.b);
  memory[0] = 0x41; // MOV B,C
  i8080_step(&cpu);
  printf("B after MOV B,C: %d\n", cpu.b);
  if (cpu.b == cpu.c)
  {
    printf("OK!");
  }
  else
  {
    printf("ERROR");
  }
  // JUST TO TEST THAT IT WORKS  - END

  free(memory);
  return 0;
}