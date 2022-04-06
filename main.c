#include <stdio.h>
#include <stdbool.h>
#include "malloc.h"

int main() {
  void *addr;
  addr = malloc(4);
  printf("addr: %p\n", addr);
  return 0;
}
