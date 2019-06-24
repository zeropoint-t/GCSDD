
#include <fcntl.h>		// open
#include <unistd.h>		// read
#include <sys/types.h>		// read
#include <sys/uio.h>		// read
#include <stdio.h>		// fopen, fread
#include <sys/time.h>		// gettimeofday

#include <string.h>
#include <string>
#include <iostream>

#include "file.h"

using namespace std;


const char *PROG_NAME = "none";

void usage();


class Timer {
public:
  Timer(const string& message): message(message) {
    gettimeofday(&start, nullptr);
  }

  ~Timer() {
    struct timeval end;
    gettimeofday(&end, nullptr);
    double elapsed =
      ((double) end.tv_sec + (double) end.tv_usec / 1e6) -
      ((double) start.tv_sec + (double) start.tv_usec / 1e6);
    printf("%s elapsed time:\n\t%12.9f seconds\n", message.c_str(), elapsed);
  }

private:
  string message;
  struct timeval start;
};


class Unix_Reader {
public:
  Unix_Reader(char *filename) {
    // TODO(lab): open the file using the Unix system call; exit
    // program with failure if file cannot be opened.
    fd_ = open(filename, O_RDONLY);
    if(fd_ == -1)
      exit(EXIT_FAILURE);
  }

  ~Unix_Reader() {
    // TODO(lab): close the file.
   if(close(fd_ == -1))
      exit(EXIT_FAILURE);
  }

  int fd() {return fd_;}

private:
  int fd_;
};

class Stdio_Reader {
public:
  Stdio_Reader(char *filename) {
    // TODO(lab): open the file using the C standard I/O; exit
    // program with failure if file cannot be opened.
    f_ = new File(filename, "r");
    if(f_ == NULL)
      exit(EXIT_FAILURE);
  }

  ~Stdio_Reader() {
    // TODO(lab): close the file.
    delete f_;
  }

  File *f() {return f_;}

private:
  // FILE *f_;
  File *f_;
};

void usage() {
  fprintf(stderr,  "usage: %s filename chunk_size\n", PROG_NAME);
  fprintf(stderr, "   where chunk_size > 0\n");
  exit(EXIT_FAILURE);
}

int main( int argc, char *argv[] ) {

  PROG_NAME = argv[0];
  if ( argc != 3 ) {
    usage();
  }

  int chunk_size = atoi( argv[2] );
  if ( chunk_size < 1 ) {
    usage();
  }

  printf("%i ", chunk_size);
  // cout << chunk_size << " ";

  char *filename = argv[1];
  char *buf = new char[chunk_size];

  int total_bytes_read = 0;
  {
    // Unix I/O
    Unix_Reader r{filename};
    int fd = r.fd();
    Timer t(string("Unix system calls, chunk size ") + to_string(chunk_size));
    
    // TODO(lab): read the file in chunks of chunk_size using Unix
    // File I/O.  Count the number of bytes read.  Exit the loop on
    // EOF.  Print error message and terminate the program if there is
    // an error reading the file.
    ssize_t size = 0;
    while((size = read(fd, buf, chunk_size)) > 0){
      if(size < 0){
        printf("error while reading using unix reader: %s",strerror(errno));
        exit(EXIT_FAILURE);
      }
      total_bytes_read += size;
    }
  }
  // printf("\tbytes read: %d\n", total_bytes_read);

  total_bytes_read = 0;
  {
    // C standard library  I/O.
    Stdio_Reader r{filename};
    File *file = r.f();
    Timer t(string("C standard library I/O, block size ") + to_string(chunk_size));

    // TODO(lab): read the file in chunks of chunk_size using C
    // standard I/O.  If chunk_size is 1, use fgetc. Count the number
    // of bytes read.  Exit the loop on EOF.  Print error message and
    // terminate the program if there is an error reading the file.
    size_t size = 0;
    if(chunk_size == 1){
      while(!file->feof()){
        file->fgetc();

        total_bytes_read += 1;
        
        if(file->ferror()){
          printf("Error while reading using stdio reader");
          exit(EXIT_FAILURE);
        }
      }
    }else{
      while((size = file->fread(buf, 1, chunk_size)) > 0){
        total_bytes_read += size;

        if(file->feof())
          break;

        if(file->ferror()){
          printf("Error while reading using stdio reader");
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  // printf("\tbytes read: %d\n", total_bytes_read);

  cout << endl;
  exit(EXIT_SUCCESS);
}