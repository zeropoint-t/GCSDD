Assignment 3: Standard I/O Library

Author: Tetsuya Hayashi

I organized this assignment into 3 directories
1. lib - contains file.h, file.cc and a BUILD script to create file.o
2. imp1 - imitates lab3 using my I/O file impelentation. It uses file.o as a library.
3. imp2 - functional testing as a driver file(main.cc) to show functinal testing results with sample files(message1.txt 
            through message4.txt). It uses file.o as a library

At the root level, I created a RUN file which will run the 2 executables.
You should see the results of functional testing on a console, and GNU Plot window should pop up automatically graphing the
performance of Unix implementation vs my I/O implementation.

Please run this program by simply type "./RUN" to see the results.