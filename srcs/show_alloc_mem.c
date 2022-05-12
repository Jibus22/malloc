#include <limits.h>
#include <stdio.h>

#include "libft.h"
#include "malloc.h"

static void _concat_uint(char *dst, unsigned int n) {
  unsigned int e;
  int i = 1;

  for (e = n / 10; e; i++) e /= 10;
  while (i--) dst[i] = ((n / ft_pow(10, e++)) % 10) + 48;
}

static void _print_zone(void *zone_addr, e_zone type) {
  char zonetype[3][9] = {"TINY : ", "SMALL : ", "LARGE : "};
  char msg[32];

  ft_bzero(msg, sizeof(msg));
  ft_strlcpy(msg, zonetype[type], sizeof(msg));
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
  t_alloc *head = zone->start;

  while (head) {
    *total += head->size;
    _print_alloc(head);
    head = head->next;
  }
}

static t_zone *_get_ascending_zone(unsigned long *min) {
  t_zone *zone = g_mnode.zone;
  unsigned long current, supp = UINTMAX_MAX;

  while (zone) {
    current = (unsigned long)zone;
    if (current > *min && current < supp) supp = current;
    zone = zone->next;
  }
  *min = supp;
  return ((t_zone *)(*min * (*min != UINTMAX_MAX)));
}

EXPORT
void show_alloc_mem() {
  unsigned int total = 0;
  unsigned long min = 0;
  t_zone *zone = _get_ascending_zone(&min);

  write(1, "### __ ###\n", 12); /*TODO: delete*/
  while (zone) {
    _print_zone((void *)zone, zone->type);
    _roam_talloc(zone, &total);
    zone = _get_ascending_zone(&min);
  }
  _print_total(total);
}
