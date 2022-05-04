#ifndef MALLOC_H
#define MALLOC_H

/*
** each zone must be able to hold at least 100 allocations so we have to
** consider the max sized block of each zone.
**
** Calculus: (x * (4096 - 32) / 100) - 32
**
** x is the arbitrary factor we choose.
** 32 is the size of t_alloc metadata in Bytes (for each allocations).
** 32 is the size of t_zone metadata in Bytes (for each zone).
** 4096 is the value of getpagesize() on my system. Values are hard-coded here
** for the sake of the example, but not into code.
**
** 2 gives max alloc size as 49.     (we can have 100 allocations of 48B)
** 10 gives max alloc size as 376.
** 50 gives max alloc size as 2000.
** 100 gives max alloc size as 4032.
** 150 gives max alloc size as 6064.
** 200 gives max alloc size as 8096.
** 
** a large memory block is automatically rounded to the higher multiple of
** getpagesize() - often = 4096 - so it makes sense to divide the capacity of
** these blocks as a large block doesn't waste to much memory.
** For example if small size limit is 376 and we ask 1000,
** it would waste 3096.(x3)
** But if small size limit is 8000, it would left nothing. And if 10.000 is
** asked, it would left 2288(x1/5). 8193 would left 4095(x1/2), this is a
** matter of proportion.
*/

#define TINY_FACTOR 10
#define SMALL_FACTOR 150

#define EXPORT __attribute__((visibility("default")))

#include <stddef.h>

typedef enum zone { tiny, small, large } e_zone;

/**
 * allocation's metadata
 */

typedef struct s_alloc {
  struct s_alloc *prev, *next;
  char *payload;
  unsigned int unusued;
  unsigned int size;
} t_alloc;

/**
 * zone's metadata
 */

typedef struct s_zone {
  struct s_zone *prev, *next;
  t_alloc *start; /* address of first allocation */
  e_zone type;
  unsigned int vacant_max; /* Size of page(s) - size of t_zone */
} t_zone;

/**
 * main global node for internal malloc, free, (...) memory managment
 */

typedef struct s_mnode {
  t_zone *zone;
  unsigned int tiny_smax;
  unsigned int small_smax;
} t_mnode;

extern t_mnode g_mnode;

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void show_alloc_mem();

void _mnode_init();
void _setAllocType(size_t size, e_zone *alloc_type);
size_t _getZoneSize(e_zone alloc_type, size_t size);
void _updateVacantMax(t_zone *zone, size_t zonesize);
void _optional_abort(const char *msg);

#endif
