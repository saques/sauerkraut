#!/bin/bash

rm a.out
rm output.ll
./sauerkraut $1 -o output.ll && clang output.ll sklib/sklib.a && ./a.out