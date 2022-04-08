#include <stdbool.h>
#include <stdio.h>

#include "malloc.h"

int main() {
  void *addr;
  int i = 0;

  addr = malloc(100);
  printf(" - i: %d\n", i);
  addr = malloc(42);
  printf(" - i: %d\n", i);
  addr = malloc(4);
  printf(" - i: %d\n", i);
  for (i = 0; i < 120; i++) {
    addr = malloc(42);
    printf(" - i: %d\n", i);
  }
  show_alloc_mem();
  return 0;
}
