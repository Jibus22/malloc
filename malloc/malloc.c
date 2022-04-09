#include "malloc.h"

#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "libft.h"

#define EXPORT __attribute__((visibility("default")))

t_mnode g_mnode = {NULL, 0, 0};

static void _mnode_init() {
  int pagesize = getpagesize();

  g_mnode.tiny_smax =
      (TINY_FACTOR * ((pagesize - sizeof(t_zone)) / 100) - sizeof(t_alloc));
  g_mnode.small_smax =
      (SMALL_FACTOR * ((pagesize - sizeof(t_zone)) / 100) - sizeof(t_alloc));
}

static void _setAllocType(size_t size, e_zone *alloc_type) {
  *alloc_type =
      (0 * (size <= g_mnode.tiny_smax)) +
      (1 * (size > g_mnode.tiny_smax) && (size <= g_mnode.small_smax)) +
      (2 * (size > g_mnode.small_smax));
}

/**
 * Create a zone of a size corresponding to which had been requested by the
 * client: tiny, small or large.
 * The memory is requested with mmap() and t_zone metadata added to its
 * begining. Its address is returned.
 */

static void *_create_zone(size_t size, e_zone alloc_type) {
  void *addr;
  int pagesize = getpagesize(),
      zonesize = ((pagesize * TINY_FACTOR) * (alloc_type == tiny)) +
                 ((pagesize * SMALL_FACTOR) * (alloc_type == small)) +
                 (size * (alloc_type == large));
  t_zone new_zone = {NULL, NULL, NULL, alloc_type, zonesize - sizeof(t_zone)};

  addr = mmap(NULL, zonesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE,
              -1, 0);
  if (addr == MAP_FAILED) return NULL;
  ft_memcpy(addr, &new_zone, sizeof(new_zone));
  return addr;
}

/**
 * returns a zone according to the allocation size requested and which has
 * enough space inside, or NULL
 */

static t_zone *_getZone(size_t size, e_zone alloc_type) {
  t_zone *head = g_mnode.zone;

  while (head) {
    if (head->type == alloc_type &&
        (head->vacant_max >= (size + sizeof(t_alloc))))
      return head;
    head = head->next;
  }
  return head;
}

/**
 * add a new zone to the list pointed by g_mnode.zone
 */

static void _addZone(t_zone *zone) {
  t_zone *head = g_mnode.zone;

  if (!head) {
    g_mnode.zone = zone;
    return;
  }
  while (head->next) head = head->next;
  head->next = zone;
  zone->prev = head;
}

static unsigned int _getZoneSize(const t_zone *zone, size_t size) {
  return (getpagesize() * TINY_FACTOR * (zone->type == tiny)) +
         (getpagesize() * SMALL_FACTOR * (zone->type == small)) +
         (size * (zone->type == large));
}

static t_alloc *_setAlloc(t_alloc *head, t_alloc *prev, t_alloc *next,
                          unsigned int size) {
  t_alloc setalloc = {prev, next, (char *)(head + 1), 0, size};

  ft_memcpy(head, &setalloc, sizeof(*head));
  return head;
}

/**
 * roam t_zone to find a place where there is enough space to set a new t_alloc,
 *  then set the new t_alloc as well as the previous and next t_alloc so the
 * linked list of t_alloc stays consistent.
 * Returns the newly created t_alloc.
 */

static t_alloc *_getAlloc(t_zone *zone, size_t size, void *end) {
  t_alloc *ptr, *head = zone->start;

  if (!head) {
    ptr = _setAlloc((t_alloc *)(zone + 1), NULL, NULL, size);
    zone->start = ptr;
    return ptr;
  }
  if ((char *)head > size + (char *)(zone + 1)) {
    ptr = _setAlloc((t_alloc *)(zone + 1), NULL, head, size);
    zone->start = ptr;
    head->prev = ptr;
    return ptr;
  }
  while (head->next) {
    if (((char *)head->next > size + (head->payload + head->size))) {
      ptr = _setAlloc((t_alloc *)(head->payload + head->size), head, head->next,
                      size);
      head->next->prev = ptr;
      head->next = ptr;
      return ptr;
    }
    head = head->next;
  }
  if (((char *)end >= size + (head->payload + head->size + sizeof(t_alloc)))) {
    ptr = (t_alloc *)(head->payload + head->size);
    head->next = ptr;
    return _setAlloc(ptr, head, NULL, size);
  }
  return NULL;
}

/**
 * roam the t_zone to calculate space into each t_alloc and find and sets the
 * biggest free memory area so that the vacant_max data of the current zone
 * get updated
 */

void _updateVacantMax(t_zone *zone, char *end) {
  t_alloc *head = zone->start;
  unsigned int new_vacant = 0;
  ptrdiff_t diff;

  while (head->next) {
    diff = (char *)head->next - (head->payload + head->size);
    new_vacant =
        (new_vacant * (diff <= new_vacant)) + (diff * (diff > new_vacant));
    head = head->next;
  }
  diff = end - ((char *)head->payload + head->size);
  new_vacant =
      (new_vacant * (diff <= new_vacant)) + (diff * (diff > new_vacant));
  zone->vacant_max = new_vacant;
}

static void *_create_client_alloc(t_zone *zone, size_t size) {
  unsigned int zonesize = _getZoneSize(zone, size);
  t_alloc *head = _getAlloc(zone, size, (char *)zone + zonesize);

  if (!head) return NULL;
  _updateVacantMax(zone, (char *)zone + zonesize);
  return head->payload;
}

EXPORT
void *malloc(size_t size) {
  t_zone *zone;
  e_zone alloc_type;
  void *client_alloc;

  if (!g_mnode.tiny_smax) _mnode_init();
  _setAllocType(size, &alloc_type);
  zone = _getZone(size, alloc_type);
  if (!zone) {
    zone = _create_zone(size, alloc_type);
    if (!zone) return NULL;
    _addZone(zone);
  }
  client_alloc = _create_client_alloc(zone, size);
  return client_alloc;
}
