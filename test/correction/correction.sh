#!/bin/bash


##### COLORS #####
BLUE="\033[1;35m"
GREEN="\033[1;37m"
RED="\033[1;31m"
END="\033[0m"

##### PATH #####
curr_dir=$PWD
dir=${curr_dir##*/}

if [ $dir = 'correction' ]; then
	cd .
elif [ $dir = 'test' ]; then
	cd correction
elif [ $dir = 'malloc' ]; then
	cd test/correction
else
	echo "you are'nt in a good directory, go to malloc/";
	exit;
fi


ROOT="../.."
SRCPATH="$ROOT/srcs"
OBJPATH="$SRCPATH/obj"
TESTPATH="."

##### LIB #####
HOSTTYPE=$(uname -m)_$(uname -s)
LIBFTMALLOC="libft_malloc_"$HOSTTYPE".so"
LIBMALLOC="ft_malloc_"$HOSTTYPE
LIBPATH="."

##### INCLUDE #####
PATH_INCLUDE="$SRCPATH/includes"
PATH_INCLUDE2=$LIBFTPATH
INC="-I $PATH_INCLUDE"


##### COMPILER #####
CC="clang"
##### COMPILATION FLAG #####
CCTESTFLAGS="-Wall -Wextra -Werror -std=c90"

TESTS=("test0.c" "test1.c" "test2.c" "test3_0.c" "test3_1.c" "test4.c" "test5.c")


if [ -e $LIBFTMALLOC ]; then
	rm $LIBFTMALLOC
fi
cp $ROOT/$LIBFTMALLOC $TESTPATH


D_NAME="test.out"

echo $BLUE"## RUN TESTS WITH LIBFTMALLOC ##"$END
for tests in "${TESTS[@]}"
do echo $RED"run $tests:"$END &&\
	$CC $CCTESTFLAGS $INC $TESTPATH/$tests -L$LIBPATH -l$LIBMALLOC\
	-o $TESTPATH/$D_NAME && $TESTPATH/$D_NAME
done

echo $BLUE"## RUN TESTS WITH LIBC MALLOC ##"$END
for tests in "${TESTS[@]}"
do echo $RED"run $tests:"$END &&\
	$CC $CCTESTFLAGS $INC $TESTPATH/$tests -o $TESTPATH/$D_NAME\
	&& $TESTPATH/$D_NAME
done
rm $D_NAME
