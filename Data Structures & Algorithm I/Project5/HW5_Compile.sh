### to run: copy this file to the folder where your source code is located. In terminal, navigate to the file directory.To run this file, type the command 'sh HW4_compile.sh'.

#this originally was three commands, however I have found that it can be done in one...

#g++ -c main.cpp complex.cpp
#g++ -o HW1 main.o complex.o
#rm main.o complex.o

#the below is taking only files from a single directory, though it could be appended with other folder locations to compile .cpp files together from multiple file locations.
#main.cpp must be called out before calling the rest of the .cpp files, otherwise the dependency tree won't be able to link everything together.
echo pwd
echo "Enter output filename:"
read filename
g++ -o $filename -I . -I ./main.cpp # path/to/example/*.cpp