#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#include "malloc.h"

int main() {
  char *addr[10];

  addr[0] = malloc(20);
  addr[1] = malloc(40);
  addr[2] = malloc(4);
  addr[3] = malloc(15);
  addr[4] = malloc(42);
  addr[5] = malloc(89);
  addr[6] = malloc(589);
  addr[7] = malloc(400);
  addr[8] = malloc(400);
  printf("------------- s_a_m -------------\n");
  show_alloc_mem();

  free(addr[0]);
  /* free(addr[3]); */
  /* free(addr[5]); */
  /* free(addr[7]); */
  printf("------------- s_a_m -------------\n");
  show_alloc_mem();

  /* addr[0] = malloc(42); */
  /* addr[3] = malloc(5); */
  /* addr[5] = malloc(100); */
  /* printf("------------- s_a_m -------------\n"); */
  /* show_alloc_mem(); */

  /* strlcpy(addr[1], "Ceci est un test", 42); */
  /* printf("le contenu du 2eme 42: |%s|\n", addr[1]); */

  /* realloc(addr[1], 156); */
  /* printf("------------- s_a_m -------------\n"); */
  /* show_alloc_mem(); */

  /* printf("le contenu du 2eme 42 devenu 156: |%s|\n", addr[1]); */
  return 0;
}
