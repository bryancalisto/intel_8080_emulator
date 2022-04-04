#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "i8080.h"
#include "instructions.h"

static int setup(void **state)
{
  i8080 *processor = malloc(sizeof(i8080));

  if (processor == NULL)
  {
    return -1;
  }

  *state = processor;

  return 0;
}

static int teardown(void **state)
{
  free(*state);
  return 0;
}

static void dummy_test(void **state)
{
  assert_true(1);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup_teardown(dummy_test, setup, teardown),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}