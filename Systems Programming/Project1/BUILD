#! /bin/bash

rm -rf data.txt

python generate_data.py 10000000 > data.txt

g++ -std=c++11 -Wall -Werror -pedantic -O3 -D FORK -D GRAHAM *.cc -o main
