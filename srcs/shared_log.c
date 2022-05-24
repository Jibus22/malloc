#include "malloc.h"

void _concat_address(char *dst, unsigned long int n) {
  unsigned long int e;
  short int res;
  int i = 1;

  dst[0] = '0';
  dst[1] = 'x';
  for (e = n / 16; e; i++) e /= 16;
  while (i--) {
    res = ((n / ft_pow(16, e++)) % 16);
    res = ((res + 48) * (res < 10)) + ((res + 55) * (res >= 10));
    dst[i + 2] = res;
  }
}

void _concat_uint(char *dst, unsigned int n) {
  unsigned int e;
  int i = 1;

  for (e = n / 10; e; i++) e /= 10;
  while (i--) dst[i] = ((n / ft_pow(10, e++)) % 10) + 48;
}

void _print_addr(void *ptr, size_t size, const char *title) {
  char dst[96];

  ft_bzero(dst, sizeof(dst));
  ft_strlcpy(dst, title, sizeof(dst));
  _concat_address(dst + ft_strlen(dst), (unsigned long)ptr);
  ft_strlcat(dst, " size: ", sizeof(dst));
  _concat_uint(dst + ft_strlen(dst), size);
  ft_strlcat(dst, " ; ", sizeof(dst));
  write(1, dst, ft_strlen(dst));
}

void _resume(int block_nb, int total_leak) {
  char dst[128];

  ft_bzero(dst, sizeof(dst));
  ft_strlcpy(dst, "malloc: *** leaks:   ", sizeof(dst));
  _concat_uint(dst + ft_strlen(dst), block_nb);
  ft_strlcat(dst, " blocks and ", sizeof(dst));
  _concat_uint(dst + ft_strlen(dst), total_leak);
  ft_strlcat(dst, " bytes\n", sizeof(dst));
  write(1, dst, ft_strlen(dst));
}

void _show_leaks() {
  t_zone *zone = g_mnode.zone;
  t_alloc *alloc;
  int block_nb = 0, total_leak = 0;

  while (zone) {
    if (zone->start) {
      alloc = zone->start;
      while (alloc) {
        block_nb++;
        total_leak += alloc->size;
        alloc = alloc->next;
      }
    }
    zone = zone->next;
  }
  _resume(block_nb, total_leak);
}
