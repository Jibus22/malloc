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
  t_zone new_zone = {NULL, alloc_type, zonesize - sizeof(t_zone)};

  addr = mmap(NULL, zonesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE,
              -1, 0);
  printf(
      "----- create new zone ----- addr: %p - zonesize: %d - alloc_type: %d\n",
      addr, zonesize, alloc_type);
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
}

static unsigned int _getZoneSize(const t_zone *zone, size_t size) {
  return (getpagesize() * TINY_FACTOR * (zone->type == tiny)) +
         (getpagesize() * SMALL_FACTOR * (zone->type == small)) +
         (size * (zone->type == large));
}

static t_alloc *_setAlloc(t_alloc *head, t_alloc *prev, t_alloc *next,
                          void *payload, unsigned int is_free,
                          unsigned int size) {
  t_alloc setalloc = {prev, next, payload, is_free, size};

  ft_memcpy(head, &setalloc, sizeof(*head));
  return head;
}

/**
 * roam t_zone to find a place where there is enough space to set a new t_alloc,
 *  then set the new t_alloc as well as the previous and next t_alloc so the
 * linked list of t_alloc stays consistent.
 * Returns the newly created t_alloc.
 */

static t_alloc *_getAlloc(t_alloc *head, long size, void *end) {
  t_alloc *ptr;

  if (head->payload != head + 1 || head->prev || head->size < 0)
    return _setAlloc(head, NULL, NULL, head + 1, 0, size);
  while (head->next) {
    if (head->is_free && ((void *)head->next - head->payload) >= size)
      return _setAlloc(head, head->prev, head->next, head + 1, 0, size);
    if (((char *)head->next - ((char *)(head + 1) + head->size)) >= size) {
      ptr = _setAlloc(
          (t_alloc *)((char *)head->payload + head->size), head, head->next,
          (void *)((char *)head->payload + head->size + sizeof(t_alloc)), 0,
          size);
      head->next = ptr;
      head->next->prev = ptr;
      return ptr;
    }
    head = head->next;
  }
  if (head->is_free && (end - head->payload) >= size &&
      head + 1 == head->payload)
    return _setAlloc(head, head->prev, head->next, head + 1, 0, size);
  else if (!(head->is_free) &&
           (end - (void *)((char *)head->payload + head->size +
                           sizeof(t_alloc))) >= size) {
    ptr = (t_alloc *)((char *)head->payload + head->size);
    head->next = ptr;
    return _setAlloc(
        ptr, head, NULL,
        (void *)((char *)head->payload + head->size + sizeof(t_alloc)), 0,
        size);
  } else
    return NULL;
}

/**
 * roam the t_zone to calculate space into each t_alloc and find and sets the
 * biggest free memory area so that the vacant_max data of the current zone
 * get updated
 */

static void _updateVacantMax(t_zone *zone, char *end) {
  t_alloc *head = (t_alloc *)(zone + 1);
  unsigned int new_vacant = 0;
  ptrdiff_t diff;

  while (head->next) {
    diff = (char *)head->next - ((char *)head->payload + head->size);
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
  t_alloc *head =
      _getAlloc((t_alloc *)(zone + 1), size, (char *)zone + zonesize);

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
  printf("zoneaddr: %p - next: %p - type: %d - vacant_max: %d\n    addr: %p",
         zone, zone->next, zone->type, zone->vacant_max, client_alloc);
  return client_alloc;
}
