
#include <sys/socket.h>

#define MAX_MESSAGE_SIZE 1024

//request struct to represent a request object
struct Request {

    unsigned long auth_token;
    unsigned long seqnum;
    uint16_t reqType;//represents read, write or lseek

    union {
      struct {
        char pathname[255];
        char mode[2];
      } open;

      struct {
        size_t size;
      } read;

      struct {
        size_t size;
        size_t nitems;
        unsigned char buf[MAX_MESSAGE_SIZE];
      } write;

      struct {
        off_t offset;
        int whence;
      } lseek; 

      struct {
        char pathname[255];
        mode_t mode;
      } chmod; 

      struct {
        char pathname[255];
      } unlink; 

      struct {
        char oldpath[255];
        char newpath[255];
      } rename; 

    } action;
};