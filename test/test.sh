#!/bin/sh

HOSTTYPE=$(uname -m)_$(uname -s)

if [ $HOSTTYPE = "x86_64_Linux" ]; then
	echo $PWD "linux"
	export LD_LIBRARY_PATH=$PWD
	export LD_PRELOAD=libft_malloc.so
elif [ $HOSTTYPE = "x86_64_Darwin" ]; then
	echo "mac"
	export DYLD_LIBRARY_PATH=$PWD
	export DYLD_INSERT_LIBRARIES=$PWD/libft_malloc.so
	export DYLD_FORCE_FLAT_NAMESPACE=1
else
	echo "wrong host"
	exit
fi

$@
