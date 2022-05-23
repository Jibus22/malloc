#!/bin/sh


D_NAME="test.out"

##### COLORS #####
BLUE="\033[1;35m"
GREEN="\033[1;37m"
RED="\033[1;31m"
END="\033[0m"

##### PATH #####
curr_dir=$PWD
dir=${curr_dir##*/}

if [ $dir = 'correction' ]; then
	cd ..
elif [ $dir = 'test' ]; then
	cd .
elif [ $dir = 'malloc' ]; then
	cd test
else
	echo "you are'nt in a good directory, go to malloc/";
	exit;
fi


ROOT=".."
SRCPATH="$ROOT/srcs"
OBJPATH="$SRCPATH/obj"
TESTPATH="."

##### LIB #####
HOSTTYPE=$(uname -m)_$(uname -s)
LIBFTMALLOC="libft_malloc_"$HOSTTYPE".so"
LIBMALLOC="ft_malloc"
LIBPATH=$ROOT
LIBFTPATH="$ROOT/libft"

##### INCLUDE #####
PATH_INCLUDE="$SRCPATH/includes"
PATH_INCLUDE2=$LIBFTPATH
INC="-I $PATH_INCLUDE"
INC=$INC" -I $LIBFTPATH"


##### COMPILER #####
CC="clang"
##### COMPILATION FLAG #####
CCTESTFLAGS="-Wall -Wextra -Werror"

##### FILES #####
TESTS="t1.c t2.c t3.c t4.c"



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

echo $BLUE"## RUN HOME TESTS WITH LIBFTMALLOC ##"$END

for tests in $TESTS
do echo $RED"run $tests:"$END && sleep 0.7 &&\
	$CC $CCTESTFLAGS $INC $TESTPATH/$tests -L$LIBPATH -l$LIBMALLOC\
	-o $TESTPATH/$D_NAME && $TESTPATH/$D_NAME
done

rm $D_NAME
