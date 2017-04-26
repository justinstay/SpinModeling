#!/bin/bash

clear

echo Copying files to folder
cp doSimple.sh ./SpinModelingLibrary
cp output.txt ./SpinModelingLibrary
cp SpinModeling.c ./SpinModelingLibrary
cp SpinModeling.h ./SpinModelingLibrary
cp main.c ./SpinModelingLibrary
cp makefile ./SpinModelingLibrary

echo Zipping folder and files
rm SpinModelingLibrary.zip

zip -r SpinModelingLibrary.zip SpinModelingLibrary