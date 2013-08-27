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
	TARGETMD5=($(md5sum $TARGET))
	LOCALMD5=($(md5sum $LOCAL))

	if [ $TARGETMD5 != $LOCALMD5 ]
	then
		echo "cp -f $LOCAL $TARGET"
		cp -f $LOCAL $TARGET
	else
		echo "Current build already installed"
	fi
else
	echo "cp -f $LOCAL $TARGET"
	cp -f $LOCAL $TARGET
fi
