#include <stdio.h>
#include <stdint.h>

int main()
{
  uint16_t uno = 255;
  uint8_t dos = 2;
  uint32_t tmp;
  uint32_t *ptr_uno = (uint32_t *)(((uint32_t)&uno) + 8);

  printf("addr uno: %p\n", &uno);
  printf("val uno: %d\n", uno);
  printf("addr uno[l]: %x\n", (uint32_t)(((uint32_t)&uno) + 8));
  printf("val uno[l]:  %x\n", *ptr_uno);
}