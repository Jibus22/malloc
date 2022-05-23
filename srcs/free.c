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
      if (_getenv_cached(ENV_FTSCRIBBLE))
        ft_memset(head, 0x55, sizeof(t_alloc) + head->size);
      _updateVacantMax(zone, zonesize);
      if (zone->vacant_max == zonesize - sizeof(t_zone)
			  && _bro_zone_left(zone->type)) {
        /* update t_zone linked list */
        if (zone->prev)
          zone->prev->next = zone->next;
        else
          g_mnode.zone = zone->next;
        if (zone->next) zone->next->prev = zone->prev;
        if (munmap((void *)zone, zonesize) == -1)
          _optional_abort("free: munmap failed", ptr);
      }
      pthread_mutex_unlock(&g_mutex);
      return;
    }
    head = head->next;
  }
  pthread_mutex_unlock(&g_mutex);
  _optional_abort("pointer being freed was not allocated", ptr);
  return;
}

/* EXPORT */
void free(void *ptr) {
  t_zone *zone = g_mnode.zone;
  char *end;

  if (!ptr) return;
  pthread_mutex_lock(&g_mutex);
  while (zone) {
	  if (!zone->start) {
		  zone = zone->next;
		  continue;
	  }
    end = (char *)zone + _getZoneSize(zone->type, zone->start->size);
    if (ptr >= (void *)((t_alloc *)(zone + 1) + 1) && (char *)ptr < end)
      return _roam_talloc(zone, ptr);
    zone = zone->next;
  }
  pthread_mutex_unlock(&g_mutex);
  if (!zone) _optional_abort("pointer being freed was not allocated", ptr);
}
