#include "malloc.h"

t_mnode g_mnode = {NULL, 0, 0};
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static void _ctrl_c_handler(int signal) {
  if (signal == SIGINT)
	{
    _show_leaks();
    exit(130);
	}
}

static void _mnode_init() {
  int pagesize = getpagesize();

  g_mnode.tiny_smax =
      ((TINY_FACTOR * pagesize - sizeof(t_zone)) / 100) - sizeof(t_alloc);
  g_mnode.small_smax =
      ((SMALL_FACTOR * pagesize - sizeof(t_zone)) / 100) - sizeof(t_alloc);
}

__attribute__((constructor)) static void _initializer(void) {
  _mnode_init();
  if (_getenv_cached(ENV_FTLEAKS)) {
    signal(SIGINT, _ctrl_c_handler);
  }
}
