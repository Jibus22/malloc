#include "malloc.h"

__attribute__((destructor)) static void _finalizer(void) {
  if (_getenv_cached(ENV_FTLEAKS)) _show_leaks();
}
