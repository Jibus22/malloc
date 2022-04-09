#include <sys/mman.h>
#include <unistd.h>

#include "malloc.h"

/**
 * return the full size of zone, from start to the end, without substracting any
 * dope vector
 */

static unsigned int _getZoneSize(const t_zone *zone) {
  return (getpagesize() * TINY_FACTOR * (zone->type == tiny)) +
         (getpagesize() * SMALL_FACTOR * (zone->type == small)) +
         ((((t_alloc *)(zone + 1))->size + sizeof(t_zone) + sizeof(t_alloc)) *
          (zone->type == large));
}

static void _optional_abort() {}

static void _roam_talloc(t_zone *zone, void *ptr, char *end) {
  t_alloc *head = zone->start;

  while (head) {
    if (head->payload == ptr) {
      /* update t_alloc linked list */
      if (head->prev)
        head->prev->next = head->next;
      else
        zone->start = head->next;
      if (head->next) head->next->prev = head->prev;
      _updateVacantMax(zone, end);
      if (zone->vacant_max == _getZoneSize(zone) - sizeof(t_zone)) {
        /* update t_zone linked list */
        if (zone->prev)
          zone->prev->next = zone->next;
        else
          g_mnode.zone = zone->next;
        if (zone->next) zone->next->prev = zone->prev;
        munmap(zone, _getZoneSize(zone));
      }
      return;
    }
    head = head->next;
  }
  _optional_abort();
  return;
}

void free(void *ptr) {
  t_zone *zone = g_mnode.zone;
  char *end;

  if (!ptr) return;
  while (zone) {
    end = (char *)zone + _getZoneSize(zone);
    if (ptr >= (void *)((t_alloc *)(zone + 1) + 1) && (char *)ptr < end)
      return _roam_talloc(zone, ptr, end);
    zone = zone->next;
  }
  if (!zone) _optional_abort();
}
