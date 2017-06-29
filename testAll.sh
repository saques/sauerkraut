#!/bin/bash

echo Compiling sklib
cd sklib
make > /dev/null
cd ..
echo Compiling sauerkraut
make clean > /dev/null
make > /dev/null

echo --------------- STARTING TESTS
echo
echo
echo ........................... Assert True ...........................
echo
echo
for f in examples/*.sk
do
  ./compile.sh $f > /dev/null 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo -********** TEST FAIL: $f **********-
    echo -n \>\>\>\>\>\>\>\>\>\>
    if [ ! -f a.out ]; then
        echo Could not compile $f
    else
        ./a.out > /dev/null
    fi
  else
    echo - TEST SUCCESFUL: $f
   fi
done
echo
echo
echo -e ........................... Assert False ...........................
echo
echo
for f in examples/error/*.sk
do
  ./compile.sh $f > /dev/null 2> /dev/null
  if [ $? -eq 0 ]
  then
    echo -********** TEST FAIL: $f ENDED CORRECTLY **********-
  else
    echo - TEST SUCCESFUL: $f FAILED
    echo -n \>\>\>\>\>\>\>\>\>\>
    if [ ! -f a.out ]; then
        echo Could not compile $f
    else
        ./a.out > /dev/null
    fi
   fi
done
echo --------------- END OF TESTS

