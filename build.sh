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

for i in ${SOURCES[@]}
do
	gcc -c -Wall -O3 -o $OBJDIR/${i/%.*/.o} $SRCDIR/$i
	OBJECTS+=($OBJDIR/${i/%.*/.o})
done

if [ ! -d "bin" ]
then
	mkdir -p bin
fi

gcc -Wall -o $BINDIR/$OUTPUT ${OBJECTS[@]} -lncursesw
