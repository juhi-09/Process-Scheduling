#!bin/bash
g++ 1.cpp --std=c++11
./a.out < $1 > $2
