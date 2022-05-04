#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "libft.h"
#include "malloc.h"

/* void _printZone(t_zone *zone) { */
/*   printf("type: %d, vacant_max: %u, start: %p\n", zone->type, zone->vacant_max, zone->start); */
/*   fflush(stdout); */
/* } */

/* static void _concat_address(char *dst, unsigned long int n) { */
/*   unsigned long int e; */
/*   short int res; */
/*   int i = 1; */

/*   dst[0] = '0'; */
/*   dst[1] = 'x'; */
/*   for (e = n / 16; e; i++) e /= 16; */
/*   while (i--) { */
/*     res = ((n / ft_pow(16, e++)) % 16); */
/*     res = ((res + 48) * (res < 10)) + ((res + 55) * (res >= 10)); */
/*     dst[i + 2] = res; */
/*   } */
/* } */

/* static void _print_addr(void *ptr) { */
/*   char dst[96]; */

/*   ft_bzero(dst, sizeof(dst)); */
/*   ft_strlcpy(dst, "***** free: ", sizeof(dst)); */
/*   _concat_address(dst + ft_strlen(dst), (unsigned long)ptr); */
/*   ft_strlcat(dst, " *****\n", sizeof(dst)); */
/*   write(1, dst, ft_strlen(dst)); */
/*   fflush(stdout); */
/* } */

void _optional_abort(const char *msg) {
  printf("--- optionnal_abort ---|%s|\n", msg);
  fflush(stdout);
}

static void _roam_talloc(t_zone *zone, void *ptr) {
  t_alloc *head = zone->start;
  size_t zonesize = _getZoneSize(zone->type, zone->start->size);

  while (head) {
    if (head->payload == ptr) {
      /* update t_alloc linked list */
      if (head->prev)
        head->prev->next = head->next;
      else
        zone->start = head->next;
      if (head->next) head->next->prev = head->prev;
      _updateVacantMax(zone, zonesize);
      if (zone->vacant_max == zonesize - sizeof(t_zone)) {
        /* update t_zone linked list */
        if (zone->prev)
          zone->prev->next = zone->next;
        else
          g_mnode.zone = zone->next;
        if (zone->next) zone->next->prev = zone->prev;
        if (munmap(zone, zonesize) == -1)
          _optional_abort("free: munmap failed");
      }
      return;
    }
    head = head->next;
  }
  _optional_abort("free: address not found");
  return;
}

EXPORT
void free(void *ptr) {
  t_zone *zone = g_mnode.zone;
  char *end;

  /* _print_addr(ptr); */
  if (!ptr) return;
  while (zone) {
    end = (char *)zone + _getZoneSize(zone->type, zone->start->size);
    if (ptr >= (void *)((t_alloc *)(zone + 1) + 1) && (char *)ptr < end)
      return _roam_talloc(zone, ptr);
    zone = zone->next;
  }
  if (!zone) _optional_abort("free: zone not found");
}
