#include "RemoteFileSystem.h"

#include <sys/random.h>
#include <stdlib.h>

// Driver code 
int main() { 
    
    const char * host = "192.168.84.247";//"127.0.0.1";
    const short port = 10000;
    struct timeval timeout;
    timeout.tv_sec = 7;
    timeout.tv_usec = 0;
    unsigned long auth_token = 12345;

    RemoteFileSystem rfs(host, port, auth_token, &timeout);

    //open
    const char *pathname = "test.txt";
    RemoteFileSystem::File * file = rfs.open(pathname, "r+");
    if(file == NULL){
        printf("File connection failed to open\n");
        exit(EXIT_FAILURE);
    }

    //read
    unsigned char buf[6];
    ssize_t rc = file->read(buf, sizeof(buf)-1);
    if(rc < 0){
        printf("Read failed\n");
        exit(EXIT_FAILURE);
    }
    buf[sizeof(buf)-1] = '\0';
    printf("Client read: %s\n",(char *)buf);

    //seek
    rc = file->lseek(3, RemoteFileSystem::SEEK_CUR_);
    if(rc < 0){
        printf("Read failed\n");
        exit(EXIT_FAILURE);
    }

    //write
    unsigned char wrt[] = "---";
    rc = file->write(wrt, sizeof(wrt));
    if(rc < 0){
        printf("Write failed\n");
        exit(EXIT_FAILURE);
    }

    //rename
    const char *newpathname = "newfile.txt";
    rc = rfs.rename(pathname, newpathname);

    if(rc < 0){
        printf("Write failed\n");
        exit(EXIT_FAILURE);
    }

    // //close
    rc = file->close();
    if(rc < 0){
        printf("Close failed\n");
        exit(EXIT_FAILURE);
    }

    //unlink
    rc = rfs.unlink(newpathname);
    if(rc < 0){
        printf("unlink failed\n");
        exit(EXIT_FAILURE);
    }

    //chmod
}