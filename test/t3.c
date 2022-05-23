#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#include "malloc.h"

void _dPrint(const char *msg) {
  printf("%s\n", msg);
  fflush(stdout);
}

int main() {
  char *addr;

  addr = malloc(3);
  free(addr);
  addr = malloc(32);
  free(addr);
  addr = malloc(239);
  free(addr);
  addr = malloc(83490);
  free(addr);
  addr = malloc(2490);
  free(addr);
  addr = malloc(290);
  free(addr);
  addr = malloc(3434895);
  free(addr);
  addr = malloc(290);
  free(addr);
  show_alloc_mem();
  return 0;
}
