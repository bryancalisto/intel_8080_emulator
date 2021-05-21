#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i8080.h"

#define MEMORY_SIZE 0x10000 // 65536 bytes
static uint8_t *memory = NULL;

// The actual implementations of the i8080 structure function pointers
/*Memory*/
static uint8_t read_b(uint16_t addr)
{
  return memory[addr];
}

static uint8_t read_w(uint16_t addr)
{
  return memory[addr];
}

static void write_b(uint16_t addr, uint8_t val)
{
  return memory[addr] = val;
}

static void write_w(uint16_t addr, uint16_t val)
{
  return memory[addr] = val;
}

int main()
{
  memory = (uint8_t *)calloc(MEMORY_SIZE, sizeof(uint8_t));

  if (memory == NULL)
  {
    printf("Could'not allocate enough memory (%x) for the test\n", MEMORY_SIZE);
    return -1;
  }

  i8080 cpu;

  free(memory);
  return 0;
}