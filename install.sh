#!/bin/bash

LOCAL=bin/MaxHex
TARGET=~/bin/MaxHex

if [ ! -f $LOCAL ]
then
	echo "ERROR: You must build the program before installing it!"
	exit -1
fi

if [ ! -d ~/bin ]
then
	echo "mkdir ~/bin"
	mkdir ~/bin
fi

if [ -f $TARGET ]
then
	TARGET_DATE=`stat -c %Y $TARGET`
	LOCAL_DATE=`stat -c %Y $LOCAL`

	if [ $TARGET_DATE -lt $LOCAL_DATE ]
	then
		echo "cp -f $LOCAL $TARGET"
		cp -f $LOCAL $TARGET
	else
		echo "Latest build already installed"
	fi
else
	echo "cp -f $LOCAL $TARGET"
	cp -f $LOCAL $TARGET
fi
