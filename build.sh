#!/bin/bash

SRCDIR="src"
OBJDIR="obj"
BINDIR="bin"

SOURCES=(
	main.c
	maxhex.c
	)

OUTPUT=MaxHex
OBJECTS=()

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
			echo "gcc -c -Wall -O3 -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i"
			gcc -c -Wall -O3 -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i
			CHANGES=true
		fi
	else
		echo "gcc -c -Wall -O3 -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i"
		gcc -c -Wall -O3 -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i
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
	echo "gcc -Wall -o $BINDIR/$OUTPUT ${OBJECTS[@]} -lncursesw"
	gcc -Wall -o $BINDIR/$OUTPUT ${OBJECTS[@]} -lncursesw
fi
