#include "malloc.h"

EXPORT
void *calloc(size_t count, size_t size) {
  size_t total_size;
  void *res;

  total_size = size * count;
  if (!count || !size || (total_size / count != size)) return (NULL);
  total_size = (total_size + 15) & ~15;
  res = malloc(total_size);
  pthread_mutex_lock(&g_mutex);
  if (res) ft_bzero(res, total_size);
  pthread_mutex_unlock(&g_mutex);
  return (res);
}
