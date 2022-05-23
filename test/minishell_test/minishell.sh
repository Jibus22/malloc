#!/bin/sh

##### COLORS #####
BLUE="\033[1;35m"
GREEN="\033[1;37m"
RED="\033[1;31m"
END="\033[0m"

##### PATH #####
curr_dir=$PWD
dir=${curr_dir##*/}

if [ $dir = 'correction' ]; then
	cd ../minishell_test
elif [ $dir = 'test' ]; then
	cd minishell_test
elif [ $dir = 'malloc' ]; then
	cd test/minishell_test
else
	echo "you are'nt in a good directory, go to malloc/";
	exit;
fi


HOSTTYPE=$(uname -m)_$(uname -s)
LIBPATH="../.."

##### CODE #####

prompt () {
	while true; do
		read -p "Do you wish to set "$1" ? " yesno
		case $yesno in
			[Yy]* )
				echo "You answered yes"
				export $1=1
				break
				;;
			[Nn]* )
				echo "You answered no"
				break
				;;
			* ) echo "Answer either yes or no";;
		esac
	done
}

prompt FtMallocErrorAbort
prompt FtMallocScribble

if [ $HOSTTYPE = "x86_64_Linux" ]; then
	export LD_LIBRARY_PATH=$LIBPATH
	export LD_INSERT_LIBRARIES=$LIBPATH/libft_malloc.so
	export LD_FORCE_FLAT_NAMESPACE=1
elif [ $HOSTTYPE = "x86_64_Darwin" ]; then
	export DYLD_LIBRARY_PATH=$LIBPATH
	export DYLD_INSERT_LIBRARIES=$LIBPATH/libft_malloc.so
	export DYLD_FORCE_FLAT_NAMESPACE=1
else
	exit
fi

make re && make clean && ./minishell
