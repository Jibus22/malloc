#ifndef MALLOC_H
#define MALLOC_H

/*
** each zone must be able to hold at least 100 allocations so we have to
** consider the max sized block of each zone.
**
** Calculus: (x * (4096 - 16) / 100) - 32
**
** x is the arbitrary factor we choose.
** 32 is the size of t_alloc metadata in Bytes (for each allocations).
** 16 is the size of t_zone metadata in Bytes (for each zone).
** 4096 is the value of getpagesize() on my system. Values are hard-coded here
** for the sake of the example, but not into code.
*/

/* 2 gives max tiny size as 49 */
#define TINY_FACTOR 2
/* 10 gives max small size as 376 */
#define SMALL_FACTOR 10

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
void show_alloc_mem();
void free(void *ptr);

void _updateVacantMax(t_zone *zone, char *end);

#endif
