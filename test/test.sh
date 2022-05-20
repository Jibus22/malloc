#!/bin/sh

HOSTTYPE=$(uname -m)_$(uname -s)
LIBPATH=$PWD

if [ $HOSTTYPE = "x86_64_Linux" ]; then
	echo $LIBPATH "linux"
	export LD_LIBRARY_PATH=$LIBPATH
	export LD_PRELOAD=libft_malloc.so
elif [ $HOSTTYPE = "x86_64_Darwin" ]; then
	echo "mac"
	export DYLD_LIBRARY_PATH=$LIBPATH
	export DYLD_INSERT_LIBRARIES=$LIBPATH/libft_malloc.so
	export DYLD_FORCE_FLAT_NAMESPACE=1
else
	echo "wrong host"
	exit
fi

$@
