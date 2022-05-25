#include "malloc.h"

/**
 * Create a zone of a size corresponding to which had been requested by the
 * client: tiny, small or large.
 * The memory is requested with mmap() and t_zone metadata added to its
 * begining. Its address is returned.
 */
static void *_create_zone(size_t size, e_zone alloc_type) {
  void *addr;
  size_t zonesize = _getZoneSize(alloc_type, size);

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

  if (alloc_type == large) return NULL;
  while (head) {
    if (head->type == alloc_type &&
        (head->vacant_max > (size + sizeof(t_alloc))))
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

static t_alloc *_setAlloc(t_alloc *head, t_alloc *prev, t_alloc *next,
                          unsigned int size) {
  t_alloc setalloc = {prev, next, (char *)(head + 1), 0, size};

  ft_memcpy(head, &setalloc, sizeof(*head));
  if (_getenv_cached(ENV_FTSCRIBBLE)) ft_memset(head + 1, 0xaa, size);
  return head;
}

/**
 * roam t_zone to find a place where there is enough space to set a new t_alloc,
 *  then set the new t_alloc and update the previous and next t_alloc so the
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
  if ((char *)head > ((char *)(zone + 1) + sizeof(t_alloc) + size)) {
    ptr = _setAlloc((t_alloc *)(zone + 1), NULL, head, size);
    zone->start = ptr;
    head->prev = ptr;
    return ptr;
  }
  while (head->next) {
    if ((char *)head->next >
        (char *)(head->payload + head->size + size + sizeof(t_alloc))) {
      ptr = _setAlloc((t_alloc *)(head->payload + head->size), head, head->next,
                      size);
      head->next->prev = ptr;
      head->next = ptr;
      return ptr;
    }
    head = head->next;
  }
  if (((char *)end >=
       (char *)(head->payload + head->size + sizeof(t_alloc) + size))) {
    ptr = (t_alloc *)(head->payload + head->size);
    head->next = ptr;
    return _setAlloc(ptr, head, NULL, size);
  }
  return NULL;
}

static void *_create_client_alloc(t_zone *zone, size_t size) {
  size_t zonesize = _getZoneSize(zone->type, size);
  t_alloc *head = _getAlloc(zone, size, (char *)zone + zonesize);

  if (!head) return NULL;
  _updateVacantMax(zone, zonesize);
  return head->payload;
}

EXPORT
void *malloc(size_t size) {
  t_zone *zone;
  e_zone alloc_type;
  void *client_alloc;

  if (!size) return NULL;
  size = (size + 15) & ~15;
  pthread_mutex_lock(&g_mutex);
  if (!g_mnode.tiny_smax) _mnode_init();
  _setAllocType(size, &alloc_type);
  zone = _getZone(size, alloc_type);
  if (!zone) {
    zone = _create_zone(size, alloc_type);
    if (!zone) {
      pthread_mutex_unlock(&g_mutex);
      return NULL;
    }
    _addZone(zone);
  }
  client_alloc = _create_client_alloc(zone, size);
  pthread_mutex_unlock(&g_mutex);
  return client_alloc;
}
