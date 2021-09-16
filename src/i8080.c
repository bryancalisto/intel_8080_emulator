#include "i8080.h"
#include "instructions.h"

void i8080_init(i8080 *p)
{
  p->a = 0;
  p->b = 0;
  p->c = 0;
  p->d = 0;
  p->e = 0;
  p->h = 0;
  p->l = 0;

  p->bp = 0;
  p->sp = 0;

  p->pc = 0;

  p->zf = 0;
  p->sf = 0;
  p->pf = 0;
  p->cf = 0;
  p->acf = 0;

  p->halted = 0;
  p->cycles = 0;
  p->interrupt_pending = false;

  // for (;;)
  // {
  /*
    1. Fetch opcode
    2. Update cycles according to opcode's cycles table value. In other words, substract the table value from cycles variable
    3. Decode and execute the opcode
    4. Check if cycles have finished. In other words, check if cycles is less or equal than 0
    4.1. YES: Check for interrupts and process them
    4.2. NO: Do nothing
    5. Repeat
    */
  // OpCode = Memory[PC++];
  // Counter -= Cycles[OpCode];

  // switch (OpCode)
  // {
  // case OpCode1:
  // case OpCode2:
  //   ...
  // }

  // if (Counter <= 0)
  // {
  //   /* Check for interrupts and do other */
  //   /* cyclic tasks here                 */
  //   ... Counter += InterruptPeriod;
  //   if (ExitRequired)
  //     break;
  // }

  //   i8080_step(p);
  // }
}

void i8080_step(i8080 *p)
{
  process_instruction(p);
}