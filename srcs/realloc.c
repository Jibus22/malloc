#include <stdbool.h>
#include <stddef.h>
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

/* trouver la zone où l'adresse match
 * trouver l'allocation correspondante
 * si size < curr_size il suffit de changer la size dand t_alloc et recalculer
 * vacant_max
 * autrement regarder si il y a de la place pour rajouter la size là où c'est
 * déjà alloué ou bien allouer ailleur avec un malloc des familles (si ça fail
 * on garde l'ancienne alloc, autrement ensuite on free l'ancienne alloc)
 */
static char *_realloc(t_zone *zone, void *ptr, size_t size) {
  char *end, *nxt, *new = ptr;
  t_alloc *match = NULL;
  size_t zonesize;

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
    _optional_abort("realloc error: pointer being realloc'd was not allocated");
    pthread_mutex_unlock(&g_mutex);
    return new;
  }
  nxt = (char *)(((unsigned long)match->next *
                  (unsigned long)(match->next != NULL)) +
                 ((unsigned long)end * (unsigned long)(!match->next)));
  if (_is_same_type(match->size, size) &&
      (size <= match->size || match->payload + size < nxt)) {
    match->size = size;
    zonesize = _getZoneSize(zone->type, size);
    _updateVacantMax(zone, zonesize);
  } else {
    pthread_mutex_unlock(&g_mutex);
    new = malloc(size);
    pthread_mutex_lock(&g_mutex);
    if (new) {
      ft_memcpy(new, ptr, match->size);
      pthread_mutex_unlock(&g_mutex);
      free(ptr);
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
