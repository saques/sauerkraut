#!/bin/bash
cd sklib
make
cd ..
make
./sauerkraut $1 -o  archivo.ll
clang archivo.ll sklib/sklib.a -o a.out
if [ $? -ne 0 ]
then
	echo FAILED!!!
else
	echo RUNNING...
	./a.out
fi
