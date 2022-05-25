#include "malloc.h"

t_mnode g_mnode = {NULL, 0, 0};
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static void _ctrl_c_handler(int signal) {
  if (signal == SIGINT) {
    _show_leaks();
    exit(130);
  }
}

__attribute__((constructor)) static void _initializer(void) {
  if (_getenv_cached(ENV_FTLEAKS)) {
    signal(SIGINT, _ctrl_c_handler);
  }
}
