#include <stdbool.h>
#include <stdio.h>

#include "malloc.h"

int main() {
  void *addr;
  int i = 0;

  addr = malloc(100);
  addr = malloc(42);
  addr = malloc(4);
  for (i = 0; i < 120; i++)
    addr = malloc(42);
  for (i = 0; i < 220; i++)
    addr = malloc(142);
  for (i = 0; i < 12; i++) 
    addr = malloc(3000);
  for (i = 0; i < 120; i++)
    addr = malloc(45);
  for (i = 0; i < 120; i++)
    addr = malloc(30);
  for (i = 0; i < 220; i++)
    addr = malloc(290);
  printf("SHOW MEM\n");
  show_alloc_mem();
  return 0;
}
