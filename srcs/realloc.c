#include "malloc.h"

static bool _is_same_type(size_t size_c, size_t size_n) {
  e_zone alloc_type_c, alloc_type_n;

  if (!g_mnode.tiny_smax) _mnode_init();
  _setAllocType(size_c, &alloc_type_c);
  _setAllocType(size_n, &alloc_type_n);
  return alloc_type_c == alloc_type_n;
}

static char *_soft_realloc(t_zone *zone, t_alloc *alloc, size_t size,
                           size_t zonesize) {
  char *next;

  if (!_is_same_type(alloc->size, size)) return NULL;
  if (size < alloc->size) {
    if (_getenv_cached(ENV_FTSCRIBBLE))
      ft_memset(alloc->payload + size, 0x55, alloc->size - size);
  } else {
    next = (char *)(((unsigned long)alloc->next *
                     (unsigned long)(alloc->next != NULL)) +
                    ((unsigned long)((char *)zone + zonesize) *
                     (unsigned long)(!alloc->next)));
    if (next > alloc->payload + size) {
      if (_getenv_cached(ENV_FTSCRIBBLE))
        ft_memset(alloc->payload + alloc->size, 0xaa, size - alloc->size);
    } else
      return NULL;
  }
  alloc->size = size;
  _updateVacantMax(zone, zonesize);
  pthread_mutex_unlock(&g_mutex);
  return alloc->payload;
}

static char *_realloc(t_zone *zone, t_alloc *alloc, size_t size) {
  size_t zonesize = _getZoneSize(zone->type, zone->start->size);
  char *res = _soft_realloc(zone, alloc, size, zonesize);
  void *new;

  if (res) return res;
  pthread_mutex_unlock(&g_mutex);
  new = malloc(size);
  if (!new) return NULL;
  pthread_mutex_lock(&g_mutex);
  ft_memmove(new, alloc->payload, size > alloc->size ? alloc->size : size);
  pthread_mutex_unlock(&g_mutex);
  free(alloc->payload);
  return new;
}

EXPORT
void *realloc(void *ptr, size_t size) {
  t_zone *zone = g_mnode.zone;
  t_alloc *match;

  if (!ptr)
    return malloc(size);
  else if (!size) {
    free(ptr);
    return NULL;
  }
  pthread_mutex_lock(&g_mutex);
  zone = _find_zone(zone, ptr);
  match = _find_alloc(zone, ptr);
  size = (size + 15) & ~15;
  if (!match) {
    pthread_mutex_unlock(&g_mutex);
    _optional_abort("pointer being realloc'd was not allocated", ptr);
    return NULL;
  } else if (match->size == size) {
    pthread_mutex_unlock(&g_mutex);
    return ptr;
  }
  return _realloc(zone, match, size);
}
