//
// file.cc
//
// Implement the C standard I/O library in C++ style.
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.


#include "file.h"

#include <cassert>
#include <cstdarg>

#include <fcntl.h>		// open
#include <sys/types.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>

//returns a length of c string
size_t lenstr(const char *s)
{
  int len = 0; 
  while(*s++ != '\0') 
    len++;
  return len;
}

//compare 2 c strings
bool cmpstr(const char *s1, const char *s2){
  int len1 = lenstr(s1);
  int len2 = lenstr(s2);
  if(len1 != len2)
    return false;

  //s1 & s2 has the same length
  for(int i = 0; i < len1; i++)
  {
    if(*s1 != *s2) return false;
    s1++;
    s2++;
  }
  return true;
}

//found this code on https://cboard.cprogramming.com/c-programming/134775-copy-void*-void*.html
//and this seems to be more efficient than simply iterate and copy
int copy( void *to, const void *from, size_t n )
{
    unsigned char *t = (unsigned char*)to;
    unsigned char *f = (unsigned char*)from;
    size_t x = 0;
    int r = t && f;
    if( r )
    {
        for( x = 0; x < n; x++ )
            t[ x ] = f[ x ];
 
        return 0;
    }
    return !r;
}

File::File(const char *name, const char *mode) {

  //store the mode
  rw_mode = (char *) malloc(sizeof(char)*3);

  if(cmpstr(mode,"r")){//read only
    char c[2] = "r";
    copy(rw_mode, c, lenstr(c));
    fd_ = open(name, O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH);

  }else if(cmpstr(mode,"r+")){//read/write
    char c[3] = "r+";
    copy(rw_mode, c, lenstr(c));
    fd_ = open(name, O_RDWR, S_IRUSR | S_IRGRP | S_IROTH);

  }else if(cmpstr(mode,"w")){//write only(create & truncate)
    char c[2] = "w";
    copy(rw_mode, c, lenstr(c));
    fd_ = open(name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IROTH);

  }else if(cmpstr(mode,"w+")){//read/write(create & truncate)
    char c[3] = "w+";
    copy(rw_mode, c, lenstr(c));
    fd_ = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IROTH);

  }else{
    assert(0);
  }

  //throw if file failed to open
  if(fd_ == -1){
    throw -1;
  }

  //allocate memory for buffer
  buf = (char *) malloc(sizeof(char)*bufsiz);
  size = 0;
  rd_ptr = buf;
  wt_ptr = buf;

  //read or read/write mode
  if(!cmpstr(rw_mode,"w")){
    this->size = read(fd_, buf, bufsiz);//fill the buffer
    if(this->size < 0){
      errorFlag = 1;
    }
  }
}


File::~File() {
  fflush();//commit all changes in buf
  free(buf);//release memory
  close(fd_);//close file descriptor
}


int File::ferror() {
  return errorFlag;
}


bool File::feof() {
  //it has reached the eof if written beyond last char
  if(this->size <= wt_ptr - buf)
        return true;
  return size == 0;
}


int File::setvbuf(char *buf, BufferMode mode, size_t size) {
  assert(0);			// Not implemented.
  return 0;
}


int File::fpurge() {
  if(cmpstr(rw_mode,"w"))//write only mode
  {
    wt_ptr = buf;//set write pointer back to buf(reset)
  }else{//read or read/write mode
    if(wt_ptr > rd_ptr){//write operation exists
      wt_ptr = rd_ptr;//reset write operatin back to where the last read took place
    }else{//last operatin must have been a read
      //simply reset buffer pointers for both rd and wt pointers
      size = 0;
      rd_ptr = buf;
      wt_ptr = buf;
    }
  }
  return 0;
}

int File::fflush() {
  //write only mode
  if(cmpstr(rw_mode,"w"))
  {
    int chars_written = wt_ptr - buf;//num of chars written in buffer
    ssize_t bytes_written = write(fd_,buf,chars_written);//write num of chars written in buf
    if(bytes_written < 0){
      errorFlag = 1;
      return -1;
    }
    wt_ptr = buf;//set write pointer back to buf
  }
  //read or read/write mode
  else{
    int chars_written = wt_ptr - rd_ptr;//num of chars written in buffer
    if(chars_written > 0)
    {
      int chars_read = rd_ptr - buf;//num of chars read before write begins
      lseek(fd_,-(size-chars_read), SEEK_CUR);//go back where the write begins
      ssize_t bytes_written = write(fd_,rd_ptr,chars_written);//write as far as wt_ptr advanced from rd_ptr in buf
      if(bytes_written < 0){
        errorFlag = 1;
        return -1;
      }
    }else
    {
      int chars_read = rd_ptr - buf;
      lseek(fd_,-(size-chars_read), SEEK_CUR);//go back to 1 char after the last read
    }
  

    //read ahead
    this->size = read(fd_, buf, bufsiz);//read it directly into client buf
    if(this->size < 0){
      errorFlag = 1;
    }
    rd_ptr = buf;
    wt_ptr = buf;
  }

  return 0;
}


size_t File::fread(void *ptr, size_t size, size_t nmemb) {

  //read or read/write mode
  if(!cmpstr(rw_mode,"w"))
  {
    ssize_t bytes_to_read = (size * nmemb);
    if(bytes_to_read >= bufsiz)//requesting bigger than buf size
    {
      //read directly
      //seek back to 1 char after the last read
      int chars_read = rd_ptr - buf;//num of chars read from buf
      lseek(fd_,-(this->size-chars_read), SEEK_CUR);//go back to 1 char after the last read

      //commit write operations if exists
      int chars_written = wt_ptr-rd_ptr;//num of chars written in buf
      if(chars_written > 0)
      {
        ssize_t bytes_written = write(fd_,rd_ptr,chars_written);//write as far as wt_ptr advanced from rd_ptr in buf
        if(bytes_written < 0){
          errorFlag = 1;
          return -1;
        }
      }

      //read it directly into client buf
      int bytes_read = read(fd_, ptr, bytes_to_read);
      if(bytes_read < 0){
        errorFlag = 1;
      }
      
      //fill the buffer
      this->size = read(fd_, buf, bufsiz);
      if(this->size < 0){
        errorFlag = 1;
      }
      rd_ptr = buf;
      wt_ptr = buf;

      return bytes_read;
    }
    else{
      ssize_t bytes_remain_inbuf = this->size-(wt_ptr-buf);//how many bytes remain in buf?
      if(bytes_remain_inbuf <= bytes_to_read//read request bigger than buf could cover
        || wt_ptr > rd_ptr)//buf has some writes
      {
        fflush();
      }

      //if size is less than requested bytes at this point, there is not enough bytes in the file
      //get smaller of size or bytes_to_read to finish the last read request
      bytes_to_read = this->size < bytes_to_read ? this->size : bytes_to_read;
      if(bytes_to_read == 0)
        return 0;

      //read requested number of bytes
      copy(ptr,rd_ptr,bytes_to_read);
      rd_ptr += bytes_to_read;
      wt_ptr = rd_ptr;

      return bytes_to_read;
    }
  }else{
    errorFlag = 1;//set error flag
  }
  return 0;
}


size_t File::fwrite(const void *ptr, size_t size, size_t nmemb) {
  //write or read/write mode
  if(!cmpstr(rw_mode,"r")) {
    ssize_t bytes_to_write = (size * nmemb);
    if(bytes_to_write >= bufsiz)//writing bigger than buf size
    {
      //write directly
      //seek back to 1 char after the last read
      int chars_read = rd_ptr - buf;//num of chars read from buf
      lseek(fd_,-(this->size-chars_read), SEEK_CUR);//go back to 1 char after the last read

      //commit write operations if exists
      int chars_written = wt_ptr-rd_ptr;//num of chars written in buf
      if(chars_written > 0)
      {
        ssize_t bytes_written = write(fd_,rd_ptr,chars_written);//write as far as wt_ptr advanced from rd_ptr in buf
        if(bytes_written < 0){
          errorFlag = 1;
          return -1;
        }
      }

      int bytes_written = write(fd_, ptr, bytes_to_write);//write it directly into disk
      if(bytes_written < 0){
        errorFlag = 1;
        return -1;
      }

      //if not write only, fill the buffer
      if(!cmpstr(rw_mode,"w")){
        this->size = read(fd_, buf, bufsiz);//read it directly into client buf
        if(this->size < 0){
          errorFlag = 1;
        }
        rd_ptr = buf;
        wt_ptr = buf;
      }

      return bytes_written;

    }else{//buf should have enough space for the write
      ssize_t bytes_remain_inbuf = bufsiz-(wt_ptr-buf);//how many bytes remain in buf?
      if(bytes_remain_inbuf < bytes_to_write)//flush it if request is bigger than what remains in buf
        fflush();

      //write requested number of bytes
      copy(wt_ptr, ptr, bytes_to_write);
      wt_ptr += bytes_to_write;

      return bytes_to_write;
    }
  }else{
    errorFlag = 1;
  }
  return 0; 			// placeholder
}


int File::fgetc() {
  if(size == 0){
    return -1;
  }
  
  char c[1];
  size_t chars_read = fread(c, 1, 1);
  if(chars_read < 0){
    errorFlag = 1;
  }

  return *c;
}


int File::fputc(int c) {
  //not read only
  if(!cmpstr(rw_mode,"r"))
  {
    char buf[1];
    buf[0] = c;
    size_t chars_written = fwrite(buf,1,1);
    if(chars_written < 0){
      errorFlag = 1;
    }
    // if(wt_ptr == buf + bufsiz)
    //   fflush();

    // *wt_ptr = c;
    // wt_ptr++;
  }else{
    errorFlag = 1;
    return -1;
  }
  
  return c;
}

char *File::fgets(char *s, int size) {
  //not write only
  if(!cmpstr(rw_mode,"w")){
    if(feof())
      return 0;

    fread(s, 1, size-1);
    *(s+size-1) = '\0';
  }else{
    errorFlag = 1;
    return NULL;//read not allowed
  }
  return s;
}

int File::fputs(const char *str) {
  //not read only
  if(!cmpstr(rw_mode,"r")){
    size_t s = lenstr(str);
    int rc = fwrite(str,1,s);
    return rc;
  }else{
    errorFlag = 1;
    return -1;
  }
  return 0;
}


int File::fseek(long offset, Whence whence) {
  fflush();

  off_t os = lseek(fd_,offset,whence);
  if(os < 0){
    errorFlag = 1;
    return -1;
  }

  //not wrtie only
  if(!cmpstr(rw_mode,"w")) {
    this->size = read(fd_, buf, bufsiz);//read it directly into client buf
    if(this->size < 0){
      errorFlag = 1;
    }
    rd_ptr = buf;
    wt_ptr = buf;
  }

  return 0;
}


static const int ITOA_BUFSIZE = 32; // log10(2**64 < 21) + sign character

static char *itoa(int i, char a[ITOA_BUFSIZE]) {
  char * p = a + (ITOA_BUFSIZE - 1);
  *p = '\0';			// Trailing NUL byte: end-of-string.
  if (i == 0) {
    *--p = '0';
    return p;
  }
  bool negative = (i < 0);
  if (negative) {
    i = -i;
  }
  for (; i > 0; i = i / 10) {
    *--p = '0' + (i % 10);
  }
  if (negative) {
    *--p = '-';
  }
  return p;
}


// Stripped-down version: only implements %d, %s, and %% format codes.
int File::fprintf(const char *format, ...) {
  int n = 0;			// Number of characters printed.
  va_list arg_list;
  va_start(arg_list, format);
  for (const char *p = format; *p != '\0'; p++) {
    if (*p != '%') {
      if (fputc(*p) < 0) {
	      return -1;
      }
      n++;
      continue;
    }
    
    switch(*++p) {
      case 's':
      {
        char *s = va_arg(arg_list, char *);
        for (;*s != '\0'; s++) {
          if (fputc(*s) < 0) {
            return -1;
          }
          n++;
        }
      }
      break;
      case 'd':
      {
        int i = va_arg(arg_list, int);
        char sbuf[ITOA_BUFSIZE];
        char *s = itoa(i, sbuf);
        for (;*s != '\0'; s++) {
          if (fputc(*s) < 0) {
            return -1;
          }
          n++;
        }
      }
      break;
      default:
        if (fputc(*p) < 0) {
          return -1;
        }
        n++;
    }
  }
  va_end(arg_list);
  return n;
}
