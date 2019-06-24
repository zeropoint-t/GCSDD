//
// file.h
//
// Implement the C standard I/O library in C++ style.
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.


#if !defined(FILE_H)
#define FILE_H


// Avoid including the system header.
// This is only known to work with gcc/g++
// typedef unsigned long _Addr;
// typedef _Addr size_t;
#include <sys/types.h>
#include <cstddef>

class File {
public:
  enum BufferMode {
    NO_BUFFER,
    LINE_BUFFER,
    FULL_BUFFER
  };

  enum Whence {
    SEEK_SET_,
    SEEK_CUR_,
    SEEK_END_
  };

  static const int bufsiz = 8192;
  static const int eof = -1;

  // Open a file.
  // Mode can be "r", "r+", "w", "w+", "a", and "a+"
  // Use default buffering: FULL_BUFFER.
  File(const char *name, const char *mode = "r");

  // Close the file.  Make sure any buffered data is written to disk,
  // and free the buffer if there is one.
  ~File();

  // Return non-zero value if the file is in an error state.
  // When the file is in an error state, I/O operations are not
  // performed and appropriate values are returned.
  int ferror();

  // Return true if end-of-file is reached.  This is not an error
  // condition.  Reading past eof is an error.
  bool feof();

  // Add a user-defined buffer and set the buffering mode.  If
  // non-null, the buffer must have been created by malloc and will be
  // freed by the destructor (or by another call to setvbuf).
  // LINE_BUFFER mode need not be supported.
  int setvbuf(char *buf, BufferMode mode, size_t size);

  // Clear any buffered data (without writing to disk).  Reset the
  // file pointer so that that the position is as the user would
  // expect (i.e. as if there were no bufering).
  int fpurge();

  // Like fpurge but if buffered data is outgoing, write data to
  // disk.  Reset the file pointer so it behaves the way the user
  // would expect.
  int fflush();

  // If the amount of data to be read or written exceeds the buffer,
  // avoid double-buffering by reading/writing data directly to/from
  // the source/destination.
  size_t fread(void *ptr, size_t size, size_t nmemb);
  size_t fwrite(const void *ptr, size_t size, size_t nmemb);

  int fgetc();
  int fputc(int c);

  char *fgets(char *s, int size);
  int fputs(const char *str);

  // Flush any buffered data and reset the file pointer.
  int fseek(long offset, Whence whence);

  // Stripped-down version: only implements %d and %s format codes.
  int fprintf(const char *format, ...);

private:
  char *rw_mode;
  int fd_;
  char *buf;//pointer to the first char in a buffer
  char *rd_ptr;//pointer to the next read char in a buffer
  char *wt_ptr;//pointer to the next write char in a buffer
  ssize_t size = 0;//how many bytes are read into a buffer
  int errorFlag = 0;
  // Disallow copy & assignment.
  File(File const&) = delete;
  File& operator=(File const&) = delete;
};


#endif