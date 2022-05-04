#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#include "malloc.h"

int main() {
  int i = 0;
  char *addr[80];

  for (i = 0; i < 80; i++)
    addr[i] = malloc(42);
  for (i = 0; i < 80; i++)
    malloc(142 + (i % 4));
  for (i = 0; i < 80; i++)
    malloc(14292 + (i % 400));
  for (i = 0; i < 10; i++)
    free(addr[i]);
  malloc(42133);
  for (i = 0; i < 80; i++)
    malloc(3);
  show_alloc_mem();
  return 0;
}
