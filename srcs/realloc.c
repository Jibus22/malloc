#include <stdio.h>

#include "libft.h"
#include "malloc.h"

static t_alloc *_roam_talloc(t_zone *zone, void *ptr) {
  t_alloc *head = zone->start;

  while (head) {
    if (head->payload == ptr) return head;
    head = head->next;
  }
  return NULL;
}

static bool _is_same_type(size_t size_c, size_t size_n) {
  e_zone alloc_type_c, alloc_type_n;

  if (!g_mnode.tiny_smax) _mnode_init();
  _setAllocType(size_c, &alloc_type_c);
  _setAllocType(size_n, &alloc_type_n);
  return alloc_type_c == alloc_type_n;
}

static char *_realloc(t_zone *zone, void *ptr, size_t size) {
  char *end, *nxt, *new = ptr;
  t_alloc *match = NULL;
  size_t zonesize, matchsize;

  pthread_mutex_lock(&g_mutex);
  while (zone) {
    end = (char *)zone + _getZoneSize(zone->type, zone->start->size);
    if (ptr >= (void *)((t_alloc *)(zone + 1) + 1) && (char *)ptr < end) {
      match = _roam_talloc(zone, ptr);
      break;
    }
    zone = zone->next;
  }
  if (!match) {
    _optional_abort("pointer being realloc'd was not allocated", ptr);
    pthread_mutex_unlock(&g_mutex);
    return new;
  }
  nxt = (char *)(((unsigned long)match->next *
                  (unsigned long)(match->next != NULL)) +
                 ((unsigned long)end * (unsigned long)(!match->next)));
  if (_is_same_type(match->size, size) &&
      (size <= match->size && match->payload + size < nxt)) {
    if (_getenv_cached(ENV_FTSCRIBBLE))
      ft_memset(match->payload + size, 0x55, match->size - size);
    match->size = size;
    zonesize = _getZoneSize(zone->type, size);
    _updateVacantMax(zone, zonesize);
  } else {
    matchsize = match->size;
    pthread_mutex_unlock(&g_mutex);
    new = malloc(size);
    pthread_mutex_lock(&g_mutex);
    if (new) {
      ft_memcpy(new, ptr, matchsize);
      pthread_mutex_unlock(&g_mutex);
      if (new != ptr) free(ptr);
      pthread_mutex_lock(&g_mutex);
    } else
      new = ptr;
  }
  pthread_mutex_unlock(&g_mutex);
  return new;
}

EXPORT
void *realloc(void *ptr, size_t size) {
  t_zone *zone = g_mnode.zone;

  if (!ptr)
    return malloc(size);
  else if (!size) {
    free(ptr);
    return ptr;
  }
  return _realloc(zone, ptr, size);
}
