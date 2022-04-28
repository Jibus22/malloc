#!/bin/sh
export DYLD_LIBRARY_PATH=../
export DYLD_INSERT_LIBRARIES=libft_malloc_x86_64_Darwin.so
export DYLD_FORCE_FLAT_NAMESPACE=1
$@
