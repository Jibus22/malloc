#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#include "malloc.h"

int main() {
  int i = 0;
  char *addr[180];

  for (i = 0; i < 180; i++)
    addr[i] = malloc(42);
  for (i = 0; i < 180; i++)
    addr[i] = malloc(642 + (i % 4));
  for (i = 0; i < 180; i++)
    addr[i] = malloc(14292 + (i % 400));
  for (i = 0; i < 180; i++)
    free(addr[i]);
  addr[0] = malloc(420133);
  for (i = 0; i < 80; i++)
    addr[i] = malloc(3);
  show_alloc_mem();
  return 0;
}
