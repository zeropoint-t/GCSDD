
#include <sys/socket.h>

#define MAX_MESSAGE_SIZE 1024

//response struct to represent response object
struct Response {

    unsigned long seqnum;
    uint16_t reqType;//represents open, read, write or lseek

    union {
      struct {
        int16_t success;
      } open;

      struct {
        size_t size;
        unsigned char buf[MAX_MESSAGE_SIZE];
        uint16_t error;
        uint16_t eof;
      } read;

      struct {
        size_t size;
        uint16_t error;
        uint16_t eof;
      } write;

      struct {
        int32_t success;
        uint16_t error;
        uint16_t eof;
      } lseek;

      struct {
        int16_t success;
      } close;

      struct {
        int16_t success;
      } chmod;

      struct {
        int16_t success;
      } unlink;

      struct {
        int16_t success;
      } rename;

    } action;
};