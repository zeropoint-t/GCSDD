//
// RemoteFileSystem.h
//
// Client-side remote (network) filesystem
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#if !defined(RemoteFileSystem_H)
#define RemoteFileSystem_H


#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/random.h>
#include <random>
#include <chrono>

using namespace std;

class RemoteFileSystem {
  public:

  enum Whence {
    SEEK_SET_,
    SEEK_CUR_,
    SEEK_END_
  };

  // File represents an open file object
  class File {
  public:
    // Destructor closes open file.
    ~File();

    ssize_t read(void *buf, size_t count);
    ssize_t write(void *buf, size_t count);
    off_t lseek(off_t offset, int whence);
    uint32_t close();

  private:
    // Only RemoteFileSystem can open a file.
    friend class RemoteFileSystem;
    File(RemoteFileSystem* filesystem, const char *pathname, const char *mode);

    RemoteFileSystem* rfs;
    const char *pathname;
    const char *mode;

    // Disallow copy & assignment
    File(File const &) = delete;
    void operator=(File const &) = delete;
  };

  // Connect to remote system.  Throw error if connection cannot be
  // made.
  RemoteFileSystem(const char *host,
		   short port,
		   unsigned long auth_token,
		   struct timeval *timeout);

  // Disconnect
  ~RemoteFileSystem();

  // Return new open file object.  Client is responsible for
  // deleting.
  File *open(const char *pathname, const char *mode);

  int chmod(const char *pathname, mode_t mode);
  int unlink(const char *pathname);
  int rename(const char *oldpath, const char *newpath);

 private:
    const char *host;
    short port;
    unsigned long auth_token; 
    struct timeval *timeout;
    struct sockaddr server_sockaddr;
    int server_sd;

  // File class may use private methods of the RemoteFileSystem to
  // implement their operations. Alternatively, you can create a
  // separate Connection class that is local to your implementation.
  friend class File;

  uint32_t getRandom(){
      // size_t bufsz = 256;
      // char *buf = (char *)malloc(bufsz);
      // uint32_t ret;
      // ret = getrandom(buf, bufsz, GRND_RANDOM);
      //return ret;

      unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
      default_random_engine customer_arrival_time (seed);
      poisson_distribution<int> distribution(200);
      unsigned int arrival_time = distribution(customer_arrival_time);
      return arrival_time * 1000;
  }

  // Disallow copy & assignment
  RemoteFileSystem(RemoteFileSystem const &) = delete;
  void operator=(RemoteFileSystem const &) = delete;
};

#endif