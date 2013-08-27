#!/bin/bash

CC=gcc

SRCDIR="src"
OBJDIR="obj"
BINDIR="bin"

SOURCES=(main.c maxhex.c)
OBJECTS=()
OUTPUT=MaxHex

CFLAGS="-Wall -O3"
LDFLAGS="-lncursesw"

if [ ! -d "obj" ]
then
	mkdir -p obj
fi

CHANGES=false

for i in ${SOURCES[@]}
do
	if [ -f $OBJDIR/${i/%.*/.o} ]
	then
		OBJDATE=`stat -c %Y $OBJDIR/${i/%.*/.o}`
		SRCDATE=`stat -c %Y $SRCDIR/$i`

		if [ $OBJDATE -lt $SRCDATE ]
		then
			echo "$CC -c $CFLAGS -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i"
			$CC -c $CFLAGS -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i

			if [ $? -ne 0 ]
			then
				exit -1;
			fi

			CHANGES=true
		fi
	else
		echo "$CC -c $CFLAGS -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i"
		$CC -c $CFLAGS -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i

		if [ $? -ne 0 ]
		then
			exit -1;
		fi

		CHANGES=true
	fi

	OBJECTS+=($OBJDIR/${i/%.*/.o})
done

if [ ! -d "bin" ]
then
	mkdir -p bin
fi

if [ $CHANGES = false -a -f $BINDIR/$OUTPUT ]
then
	echo "Nothing to do"
else
	echo "$CC $CFLAGS -o $BINDIR/$OUTPUT ${OBJECTS[@]} $LDFLAGS"
	$CC -o $BINDIR/$OUTPUT ${OBJECTS[@]} $LDFLAGS
fi
