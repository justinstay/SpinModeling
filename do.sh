#!/bin/bash

clear
make clean
if [ $? != 0 ]
then
    echo Script did not clean correctly!
    exit 1
fi

make
if [ $? != 0 ]
then
    echo Script did not compile correctly!
    exit 1
fi

make run
if [ $? != 0 ]
then
    echo Script did not execute correctly!
    exit 1
fi