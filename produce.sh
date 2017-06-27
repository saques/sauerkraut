#!/bin/bash
cd sklib
make
cd ..
make
./sauerkraut $1 -o  archivo.ll
if [ $? -ne 0 ]
then
	echo LLVM FAILED!!
	exit
fi
clang archivo.ll sklib/sklib.a -o a.out
if [ $? -ne 0 ]
then
	echo CLANG FAILED!!!
else
	echo RUNNING...
	./a.out
fi
