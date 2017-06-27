#!/bin/bash

./sauerkraut $1 -o output.ll
clang output.ll sklib/sklib.a