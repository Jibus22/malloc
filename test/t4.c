#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "malloc.h"

void _dPrint(const char *msg) {
  printf("%s\n", msg);
  fflush(stdout);
}

int main() {
  char *addr, *addr2;

  addr = malloc(19);
  addr2 = malloc(42);
  free(addr);
  addr = malloc(39);
  addr = malloc(3);
  addr = malloc(2);
  free(addr);
  show_alloc_mem();
  return 0;
}
