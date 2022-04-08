#include <limits.h>
#include <stdio.h>

#include "libft.h"
#include "malloc.h"

#define EXPORT __attribute__((visibility("default")))

static void _concat_address(char *dst, unsigned long int n) {
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

static void _concat_uint(char *dst, unsigned int n) {
  unsigned int e;
  int i = 1;

  for (e = n / 10; e; i++) e /= 10;
  while (i--) dst[i] = ((n / ft_pow(10, e++)) % 10) + 48;
}

static void _print_zone(void *zone_addr, e_zone type) {
  char zonetype[3][6] = {"TINY", "SMALL", "LARGE"};
  char msg[32];

  ft_bzero(msg, sizeof(msg));
  ft_strlcpy(msg, zonetype[type], sizeof(msg));
  ft_strlcat(msg, " : ", sizeof(msg));
  _concat_address(msg + ft_strlen(msg), (unsigned long)zone_addr);
  ft_strlcat(msg, "\n", sizeof(msg));
  write(1, msg, ft_strlen(msg));
}

static void _print_alloc(t_alloc *head) {
  char msg[96];

  ft_bzero(msg, sizeof(msg));
  _concat_address(msg, (unsigned long)head->payload);
  ft_strlcat(msg, " - ", sizeof(msg));
  _concat_address(msg + ft_strlen(msg),
                  (unsigned long)((char *)head->payload + head->size));
  ft_strlcat(msg, " : ", sizeof(msg));
  _concat_uint(msg + ft_strlen(msg), head->size);
  ft_strlcat(msg, " octets\n", sizeof(msg));
  write(1, msg, ft_strlen(msg));
}

static void _print_total(unsigned int total) {
  char msg[32];

  ft_bzero(msg, sizeof(msg));
  ft_strlcpy(msg, "Total : ", sizeof(msg));
  _concat_uint(msg + ft_strlen(msg), total);
  ft_strlcat(msg, " octets\n", sizeof(msg));
  write(1, msg, ft_strlen(msg));
}

static void _roam_talloc(t_zone *zone, unsigned int *total) {
  t_alloc *head = (t_alloc *)(zone + 1);

  while (head) {
    if (!head->is_free) {
      *total += head->size;
      _print_alloc(head);
    }
    head = head->next;
  }
}

static t_zone *_get_ascending_zone() {
  t_zone *zone = g_mnode.zone;
  static unsigned long min;
  unsigned long current, supp = UINTMAX_MAX;

  while (zone) {
    current = (unsigned long)zone;
    if (current > min && current < supp) supp = current;
    zone = zone->next;
  }
  min = supp;
  return ((t_zone *)(min * (min != UINTMAX_MAX)));
}

EXPORT
void show_alloc_mem() {
  unsigned int total = 0;
  t_zone *zone = _get_ascending_zone();

  while (zone) {
    _print_zone((void *)zone, zone->type);
    _roam_talloc(zone, &total);
    zone = _get_ascending_zone();
  }
  _print_total(total);
}
