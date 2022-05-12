#include "malloc.h"

#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>

#include "libft.h"

t_mnode g_mnode = {NULL, 0, 0};
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

/* static void _printShit(const char *msg) { */
/*   write(1, msg, ft_strlen(msg)); */
/*   fflush(stdout); */
/* } */

/* static void _concat_address(char *dst, unsigned long int n) { */
/*   unsigned long int e; */
/*   short int res; */
/*   int i = 1; */

/*   dst[0] = '0'; */
/*   dst[1] = 'x'; */
/*   for (e = n / 16; e; i++) e /= 16; */
/*   while (i--) { */
/*     res = ((n / ft_pow(16, e++)) % 16); */
/*     res = ((res + 48) * (res < 10)) + ((res + 55) * (res >= 10)); */
/*     dst[i + 2] = res; */
/*   } */
/* } */

/* static void _concat_uint(char *dst, unsigned int n) { */
/*   unsigned int e; */
/*   int i = 1; */

/*   for (e = n / 10; e; i++) e /= 10; */
/*   while (i--) dst[i] = ((n / ft_pow(10, e++)) % 10) + 48; */
/* } */

/* static void _print_addr(void *ptr, size_t size) { */
/*   char dst[96]; */

/*   ft_bzero(dst, sizeof(dst)); */
/*   ft_strlcpy(dst, "// malloc: ", sizeof(dst)); */
/*   _concat_address(dst + ft_strlen(dst), (unsigned long)ptr); */
/*   ft_strlcat(dst, " size: ", sizeof(dst)); */
/*   _concat_uint(dst + ft_strlen(dst), size); */
/*   ft_strlcat(dst, " //\n", sizeof(dst)); */
/*   write(1, dst, ft_strlen(dst)); */
/*   fflush(stdout); */
/* } */

/* static void _print_addr2(void *ptr, const char *msg) { */
/*   char dst[96]; */

/*   ft_bzero(dst, sizeof(dst)); */
/*   ft_strlcpy(dst, msg, sizeof(dst)); */
/*   _concat_address(dst + ft_strlen(dst), (unsigned long)ptr); */
/*   ft_strlcat(dst, " @@ ", sizeof(dst)); */
/*   write(1, dst, ft_strlen(dst)); */
/*   fflush(stdout); */
/* } */

/* static void _print_zone(t_zone *zone) { */
/*   char dst[96]; */

/*   ft_bzero(dst, sizeof(dst)); */
/*   ft_strlcpy(dst, "-- zone: ", sizeof(dst)); */
/*   _concat_uint(dst + ft_strlen(dst), zone->vacant_max); */
/*   ft_strlcat(dst, " \\", sizeof(dst)); */
/*   write(1, dst, ft_strlen(dst)); */
/*   fflush(stdout); */
/* } */

/**
 * Create a zone of a size corresponding to which had been requested by the
 * client: tiny, small or large.
 * The memory is requested with mmap() and t_zone metadata added to its
 * begining. Its address is returned.
 */
static void *_create_zone(size_t size, e_zone alloc_type) {
  /* _printShit("_create_zone "); */
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
  /* _printShit("_getZone "); */
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
  /* _printShit("_addZone "); */
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
  /* _printShit("_getAlloc "); */
  /* _print_zone(zone); */
  t_alloc *ptr, *head = zone->start;

  if (!head) {
    /* _printShit("insert in empty zone. "); */
    ptr = _setAlloc((t_alloc *)(zone + 1), NULL, NULL, size);
    zone->start = ptr;
    return ptr;
  }
  if ((char *)head > ((char *)(zone + 1) + sizeof(t_alloc) + size)) {
    /* _printShit("insert alloc at beginning. "); */
    ptr = _setAlloc((t_alloc *)(zone + 1), NULL, head, size);
    zone->start = ptr;
    head->prev = ptr;
    return ptr;
  }
  while (head->next) {
    /* _printShit("."); */
    if ((char *)head->next >
        (char *)(head->payload + head->size + size + sizeof(t_alloc))) {
      /* _printShit("insert alloc between 2 blocks. "); */
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
    /* _printShit("insert alloc at the end. "); */
    ptr = (t_alloc *)(head->payload + head->size);
    head->next = ptr;
    return _setAlloc(ptr, head, NULL, size);
  }
  /* _printShit("insert nothing. "); */
  return NULL;
}

static void *_create_client_alloc(t_zone *zone, size_t size) {
  /* _printShit("_create_client_alloc "); */
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

  pthread_mutex_lock(&g_mutex);
  if (!g_mnode.tiny_smax) _mnode_init();
  _setAllocType(size, &alloc_type);
  zone = _getZone(size, alloc_type);
  if (!zone) {
    zone = _create_zone(size, alloc_type);
    if (!zone) {
      return NULL;
      pthread_mutex_unlock(&g_mutex);
    }
    _addZone(zone);
  }
  client_alloc = _create_client_alloc(zone, size);
  /* _print_zone(zone); */
  /* _print_addr(client_alloc, size); */
  /* show_alloc_mem(); */
  /* fflush(stdout); */
  pthread_mutex_unlock(&g_mutex);
  return client_alloc;
}
