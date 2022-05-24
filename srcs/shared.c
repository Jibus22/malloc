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

bool _getenv_cached(e_env env) {
  static int env_cache;
  static bool init;

  if (!init) {
    if (getenv("FtMallocErrorAbort")) env_cache |= ENV_FTMALLOCERRORABORT;
    if (getenv("FtMallocScribble")) env_cache |= ENV_FTSCRIBBLE;
    init = true;
  }
  return (env & env_cache);
}

void _optional_abort(const char *msg, void *ptr) {
  char buf[128];

  if (!_getenv_cached(ENV_FTMALLOCERRORABORT)) return;
  ft_bzero(buf, sizeof(buf));
  ft_strlcpy(buf, "malloc: *** error for object ", sizeof(buf));
  _concat_address(buf + ft_strlen(buf), (unsigned long int)ptr);
  ft_strlcat(buf, ": ", sizeof(buf));
  ft_strlcat(buf, msg, sizeof(buf));
  ft_strlcat(buf, "\n", sizeof(buf));
  write(1, buf, ft_strlen(buf));
  abort();
}

void _mnode_init() {
  int pagesize = getpagesize();

  g_mnode.tiny_smax =
      ((TINY_FACTOR * pagesize - sizeof(t_zone)) / 100) - sizeof(t_alloc);
  g_mnode.small_smax =
      ((SMALL_FACTOR * pagesize - sizeof(t_zone)) / 100) - sizeof(t_alloc);
}

void _setAllocType(size_t size, e_zone *alloc_type) {
  *alloc_type =
      (0 * (size <= g_mnode.tiny_smax)) +
      (1 * (size > g_mnode.tiny_smax) && (size <= g_mnode.small_smax)) +
      (2 * (size > g_mnode.small_smax));
}

/**
 * return the full size of zone, from start to the end, without substracting any
 * dope vector. The bitwise trick for large type is made to get a size which
 * is a multiple of pagesize. It rounds up the value.
 */
size_t _getZoneSize(e_zone alloc_type, size_t large_alloc_size) {
  int pagesize = getpagesize();
  return ((pagesize * TINY_FACTOR) * (alloc_type == tiny)) +
         ((pagesize * SMALL_FACTOR) * (alloc_type == small)) +
         ((((large_alloc_size + sizeof(t_zone) + sizeof(t_alloc)) +
            (pagesize - 1)) &
           ~(pagesize - 1)) *
          (alloc_type == large));
}

/**
 * roam the t_zone to calculate space into each t_alloc and find and sets the
 * biggest free memory area so that the vacant_max data of the current zone
 * get updated.
 * zonesize must be the true zonesize given by _getZoneSize() with the size of
 * a block.
 */
void _updateVacantMax(t_zone *zone, size_t zonesize) {
  t_alloc *head = zone->start;
  unsigned int new_vacant = 0;
  ptrdiff_t diff;

  if (!head) {
    zone->vacant_max = zonesize - sizeof(t_zone);
    return;
  }
  while (head->next) {
    diff = ((char *)head->next) - ((char *)(head->payload + head->size));
    new_vacant =
        (new_vacant * (diff <= new_vacant)) + (diff * (diff > new_vacant));
    head = head->next;
  }
  diff = ((char *)zone + zonesize) - ((char *)head->payload + head->size);
  new_vacant =
      (new_vacant * (diff <= new_vacant)) + (diff * (diff > new_vacant));
  zone->vacant_max = new_vacant;
}

t_alloc *_find_alloc(t_zone *zone, void *ptr) {
  t_alloc *head;

  if (!zone) return NULL;
  head = zone->start;
  while (head) {
    if (head->payload == ptr) return head;
    head = head->next;
  }
  return NULL;
}

t_zone *_find_zone(t_zone *zone, void *ptr) {
  char *end;

  while (zone) {
	if (!zone->start) {
	  zone = zone->next;
	  continue;
	}
    end = (char *)zone + _getZoneSize(zone->type, zone->start->size);
    if (ptr >= (void *)((t_alloc *)(zone + 1) + 1) && (char *)ptr < end)
		return zone;
    zone = zone->next;
  }
  return NULL;
}
