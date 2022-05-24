#include "malloc.h"

static bool _bro_zone_left(e_zone type) {
  t_zone *zone = g_mnode.zone;
  int i = 0;

  if (type == large) return true;
  while (zone->next) {
    i = i + (1 * (zone->type == type));
    zone = zone->next;
  }
  return (i > 1);
}

static void _free(t_zone *zone, t_alloc *alloc) {
  size_t zonesize = _getZoneSize(zone->type, zone->start->size);

  /* update t_alloc linked list */
  if (alloc->prev)
    alloc->prev->next = alloc->next;
  else
    zone->start = alloc->next;
  if (alloc->next) alloc->next->prev = alloc->prev;
  if (_getenv_cached(ENV_FTSCRIBBLE))
    ft_memset(alloc, 0x55, sizeof(t_alloc) + alloc->size);
  _updateVacantMax(zone, zonesize);
  if (zone->vacant_max == zonesize - sizeof(t_zone) &&
      _bro_zone_left(zone->type)) {
    /* update t_zone linked list */
    if (zone->prev)
      zone->prev->next = zone->next;
    else
      g_mnode.zone = zone->next;
    if (zone->next) zone->next->prev = zone->prev;
    if (munmap((void *)zone, zonesize) == -1)
      _optional_abort("free: munmap failed", alloc->payload);
  }
  pthread_mutex_unlock(&g_mutex);
  return;
}

EXPORT
void free(void *ptr) {
  t_zone *zone = g_mnode.zone;
  t_alloc *match;

  if (!ptr) return;
  pthread_mutex_lock(&g_mutex);
  zone = _find_zone(zone, ptr);
  match = _find_alloc(zone, ptr);
  if (!zone) {
    pthread_mutex_unlock(&g_mutex);
    _optional_abort("pointer being freed was not allocated", ptr);
    return;
  }
  return _free(zone, match);
}
