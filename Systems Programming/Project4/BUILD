#! /bin/bash

#clean up a directory
rm -f "test.txt"
rm -f "newfile.txt"

#create a test text file
touch test.txt 
echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ" >> test.txt

#build a server
g++ -std=c++11 -Wall -Werror -pedantic -O3 Server.cc -o server

#build a client
g++ -std=c++11 -Wall -Werror -pedantic Client.cc RemoteFileSystem.cc -o client