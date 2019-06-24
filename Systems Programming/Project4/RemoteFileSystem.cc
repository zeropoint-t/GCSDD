#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#include "RemoteFileSystem.h"
#include "Request.h"
#include "Response.h"

// Connect to remote system.  Throw error if connection cannot be
// made.
RemoteFileSystem::RemoteFileSystem(const char *host, short port, unsigned long auth_token, struct timeval *timeout)
:host(host), port(port), auth_token(auth_token), timeout(timeout)
{
    timeout->tv_sec = 7;

    struct addrinfo hints;
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_addrlen = 0;
    hints.ai_addr = NULL;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;

    struct addrinfo *addresses;
    
    char port_buf[5];
    sprintf(port_buf,"%d", port);
    int result = getaddrinfo(this->host, port_buf, &hints, &addresses);
    if (result != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        exit(EXIT_FAILURE);
    } 

    int n;
    struct addrinfo *p;
    for (n = 0, p = addresses; p != NULL; n++, p = p->ai_next) {
        if (p->ai_addr->sa_family != AF_INET) {
            printf("%d: family %d  is not AF_INET(%d)", n, p->ai_addr->sa_family, AF_INET);
        } else {
            struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;
            long ip = addr->sin_addr.s_addr;
            printf("%d: %ld.%ld.%ld.%ld\n",
                n,
                (ip & 0xff), 
                ((ip >> 8) & 0xff), 
                ((ip >> 16) & 0xff), 
                ((ip >> 24) & 0xff)
            );
        }
    }

    printf("\n");

    server_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sd < 0) {
        perror("creating socket");
        exit(EXIT_FAILURE);
    }

    server_sockaddr = *addresses->ai_addr;
}

// Disconnect
RemoteFileSystem::~RemoteFileSystem()
{

}

// Return new open file object.  Client is responsible for
// deleting.
RemoteFileSystem::File *RemoteFileSystem::open(const char *pathname, const char *mode)
{
    struct Request req;
    req.seqnum = htonl(getRandom());
    req.auth_token = htonl(getRandom());
    req.reqType = htons(0);//open
    strcpy(req.action.open.pathname, pathname);
    strcpy(req.action.open.mode, mode);

    /* send request */
    int result = 
      sendto(server_sd, 
        (void *)&req, 
        sizeof(struct Request), 
        0, 
        (struct sockaddr *) &server_sockaddr, 
        sizeof(struct sockaddr_in)
      );

    if (result < 0) {
        perror("sending to socket");
        exit(EXIT_FAILURE);
    }

    char res_buf[sizeof(struct Response)];
    socklen_t len = sizeof(struct sockaddr_in);


    if (setsockopt(server_sd, SOL_SOCKET, SO_RCVTIMEO ,timeout,sizeof(*timeout)) < 0) {
        perror("Error");
    }

    /* wait for server to respond */
    ssize_t rc =   
      recvfrom(server_sd, 
        res_buf, 
        sizeof(struct Response), 
        0, 
        (struct sockaddr *) &server_sockaddr, 
        &len
    );
    
    //check for socket error
    if (rc < 0) {
        std::cout << "1 more retry" << std::endl;
        /* 1 more retry*/
        rc =   
          recvfrom(server_sd, 
            res_buf, 
            sizeof(struct Response), 
            0, 
            (struct sockaddr *) &server_sockaddr, 
            &len
        );
      
        if (rc < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    struct Response *res = (Response *)(&res_buf);
    puts("-------------------------response from server------------------------------");
    printf("Seqnum: %lu\n",(unsigned long)ntohl(res->seqnum));
    printf("Req type: %d\n",ntohs(res->reqType));
    printf("Success: %d\n",(int16_t)ntohs(res->action.open.success));
    puts("-------------------------------------------------------");

    File * file = new File(this, pathname, mode);
    return file;
}

int RemoteFileSystem::chmod(const char *pathname, mode_t mode){

    struct Request req;
    req.seqnum = htonl(getRandom());
    req.auth_token = htonl(getRandom());
    req.reqType = htons(4);//chmod
    strcpy(req.action.chmod.pathname, pathname);
    req.action.chmod.mode = htons(mode);

    /* send request */
    int result = 
      sendto(server_sd, 
        (void *)&req, 
        sizeof(struct Request), 
        0, 
        (struct sockaddr *) &server_sockaddr, 
        sizeof(struct sockaddr_in)
      );

    if (result < 0) {
        perror("sending to socket");
        exit(EXIT_FAILURE);
    }

    char res_buf[sizeof(struct Response)];
    socklen_t len = sizeof(struct sockaddr_in);

    if (setsockopt(server_sd, SOL_SOCKET, SO_RCVTIMEO ,timeout,sizeof(*timeout)) < 0) {
        perror("Error");
    }

    /* wait for server to respond */
    ssize_t rc =   
      recvfrom(server_sd, 
        res_buf, 
        sizeof(struct Response), 
        0, 
        (struct sockaddr *) &server_sockaddr, 
        &len
    );
    
    //check for socket error
    if (rc < 0) {
        std::cout << "1 more retry" << std::endl;
        /* 1 more retry*/
        rc =   
          recvfrom(server_sd, 
            res_buf, 
            sizeof(struct Response), 
            0, 
            (struct sockaddr *) &server_sockaddr, 
            &len
        );
      
        if (rc < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    struct Response *res = (Response *)(&res_buf);
    puts("-------------------------response from server------------------------------");
    printf("Seqnum: %lu\n",(unsigned long)ntohl(res->seqnum));
    printf("Req type: %d\n",ntohs(res->reqType));
    printf("Success: %d\n",(int16_t)ntohs(res->action.chmod.success));
    puts("-------------------------------------------------------");

    return 0;
}

int RemoteFileSystem::unlink(const char *pathname){
    struct Request req;
    req.seqnum = htonl(getRandom());
    req.auth_token = htonl(getRandom());
    req.reqType = htons(5);//unlink
    strcpy(req.action.unlink.pathname, pathname);

    /* send request */
    int result = 
      sendto(server_sd, 
        (void *)&req, 
        sizeof(struct Request), 
        0, 
        (struct sockaddr *) &server_sockaddr, 
        sizeof(struct sockaddr_in)
      );

    if (result < 0) {
        perror("sending to socket");
        exit(EXIT_FAILURE);
    }

    char res_buf[sizeof(struct Response)];
    socklen_t len = sizeof(struct sockaddr_in);

    if (setsockopt(server_sd, SOL_SOCKET, SO_RCVTIMEO ,timeout,sizeof(*timeout)) < 0) {
        perror("Error");
    }

    /* wait for server to respond */
    ssize_t rc =   
      recvfrom(server_sd, 
        res_buf, 
        sizeof(struct Response), 
        0, 
        (struct sockaddr *) &server_sockaddr, 
        &len
    );
    
    //check for socket error
    if (rc < 0) {
        std::cout << "1 more retry" << std::endl;
        /* 1 more retry*/
        rc =   
          recvfrom(server_sd, 
            res_buf, 
            sizeof(struct Response), 
            0, 
            (struct sockaddr *) &server_sockaddr, 
            &len
        );
      
        if (rc < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    struct Response *res = (Response *)(&res_buf);
    puts("-------------------------response from server------------------------------");
    printf("Seqnum: %lu\n",(unsigned long)ntohl(res->seqnum));
    printf("Req type: %d\n",ntohs(res->reqType));
    printf("Success: %d\n",(int16_t)ntohs(res->action.unlink.success));
    puts("-------------------------------------------------------");

    return 0;
}

int RemoteFileSystem::rename(const char *oldpath, const char *newpath){
    struct Request req;
    req.seqnum = htonl(getRandom());
    req.auth_token = htonl(getRandom());
    req.reqType = htons(6);//rename
    strcpy(req.action.rename.oldpath, oldpath);
    strcpy(req.action.rename.newpath, newpath);

    /* send request */
    int result = 
      sendto(server_sd, 
        (void *)&req, 
        sizeof(struct Request), 
        0, 
        (struct sockaddr *) &server_sockaddr, 
        sizeof(struct sockaddr_in)
      );

    if (result < 0) {
        perror("sending to socket");
        exit(EXIT_FAILURE);
    }

    char res_buf[sizeof(struct Response)];
    socklen_t len = sizeof(struct sockaddr_in);

    if (setsockopt(server_sd, SOL_SOCKET, SO_RCVTIMEO ,timeout,sizeof(*timeout)) < 0) {
        perror("Error");
    }

    /* wait for server to respond */
    ssize_t rc =   
      recvfrom(server_sd, 
        res_buf, 
        sizeof(struct Response), 
        0, 
        (struct sockaddr *) &server_sockaddr, 
        &len
    );
    
    //check for socket error
    if (rc < 0) {
        std::cout << "1 more retry" << std::endl;
        /* 1 more retry*/
        rc =   
          recvfrom(server_sd, 
            res_buf, 
            sizeof(struct Response), 
            0, 
            (struct sockaddr *) &server_sockaddr, 
            &len
        );
      
        if (rc < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    struct Response *res = (Response *)(&res_buf);
    puts("-------------------------response from server------------------------------");
    printf("Seqnum: %lu\n",(unsigned long)ntohl(res->seqnum));
    printf("Req type: %d\n",ntohs(res->reqType));
    printf("Success: %d\n",(int16_t)ntohs(res->action.rename.success));
    puts("-------------------------------------------------------");

    return 0;
}


RemoteFileSystem::File::File(RemoteFileSystem* filesystem, const char *pathname, const char *mode)
{
    this->pathname = pathname;
    this->mode = mode;
    this->rfs = filesystem;
}

RemoteFileSystem::File::~File(){

}

ssize_t RemoteFileSystem::File::read(void *buf, size_t count)
{
    cout << "I am reading!!" << endl;

    struct Request req;
    req.seqnum = htonl(rfs->getRandom());
    req.auth_token = htonl(rfs->getRandom());
    req.reqType = htons(1);//read
    req.action.read.size = htons(count);

    /* send request */
    int result = 
      sendto(rfs->server_sd, 
        (void *)&req, 
        sizeof(struct Request), 
        0, 
        (struct sockaddr *) &rfs->server_sockaddr, 
        sizeof(struct sockaddr_in)
      );

    if (result < 0) {
        perror("sending to socket");
        exit(EXIT_FAILURE);
    }

    char res_buf[sizeof(struct Response)];
    socklen_t len = sizeof(struct sockaddr_in);

    /* wait for server to respond */
    ssize_t rc =   
      recvfrom(rfs->server_sd, 
        res_buf, 
        sizeof(struct Response), 
        0, 
        (struct sockaddr *) &rfs->server_sockaddr, 
        &len
    );
    
    //check for socket error
    if (rc < 0) {
        std::cout << "1 more retry" << std::endl;
        /* 1 more retry*/
        rc =   
          recvfrom(rfs->server_sd, 
            res_buf, 
            sizeof(struct Response), 
            0, 
            (struct sockaddr *) &rfs->server_sockaddr, 
            &len
        );
      
        if (rc < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    struct Response *res = (struct Response *)(&res_buf);
    puts("----------------response from server-------------------");
    printf("Seqnum: %lu\n",(unsigned long)ntohl(res->seqnum));
    printf("Req type: %d\n",ntohs(res->reqType));
    printf("Size read: %d\n",ntohs(res->action.read.size));
    printf("Read: %s\n",res->action.read.buf);
    printf("Error: %d\n",ntohs(res->action.read.error));
    printf("EOF: %d\n",ntohs(res->action.read.eof));
    puts("-------------------------------------------------------");

    if(buf != NULL)
      memcpy(buf, res->action.read.buf, (size_t)ntohs(res->action.read.size));

    return 0;
}

ssize_t RemoteFileSystem::File::write(void *buf, size_t count)
{
    cout << "I am writing!!" << endl;

    struct Request req;
    req.auth_token = htonl(rfs->getRandom());
    req.seqnum = htonl(rfs->getRandom());
    req.reqType = htons(2);//write
    req.action.write.size = htons(1);
    req.action.write.nitems = htons(count);
    memcpy(req.action.write.buf, buf, count);

    /* send request */
    int result = 
      sendto(rfs->server_sd, 
        (void *)&req, 
        sizeof(struct Request), 
        0, 
        (struct sockaddr *) &rfs->server_sockaddr,
        sizeof(struct sockaddr_in)
      );

    if (result < 0) {
        perror("sending to socket");
        exit(EXIT_FAILURE);
    }

    char res_buf[sizeof(struct Response)];
    socklen_t len = sizeof(struct sockaddr_in);

    /* wait for server to respond */
    ssize_t rc =   
      recvfrom(rfs->server_sd, 
        res_buf, 
        sizeof(struct Response), 
        0, 
        (struct sockaddr *) &rfs->server_sockaddr,
        &len
    );
    
    //check for socket error
    if (rc < 0) {
        std::cout << "1 more retry" << std::endl;
        /* 1 more retry*/
        rc =   
          recvfrom(rfs->server_sd, 
            res_buf, 
            sizeof(struct Response), 
            0, 
            (struct sockaddr *) &rfs->server_sockaddr, 
            &len
        );
      
        if (rc < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    struct Response *res = (struct Response *)(&res_buf);
    puts("----------------response from server-------------------");
    printf("Seqnum: %lu\n",(unsigned long)ntohl(res->seqnum));
    printf("Req type: %d\n",ntohs(res->reqType));
    printf("Size written: %d\n",ntohs(res->action.write.size));
    printf("Error: %d\n",ntohs(res->action.write.error));
    printf("EOF: %d\n",ntohs(res->action.write.eof));
    puts("-------------------------------------------------------");

    return 0;
}

off_t RemoteFileSystem::File::lseek(off_t offset, int whence)
{
     cout << "I am lseeking!!" << endl;

    struct Request req;
    req.seqnum = htonl(rfs->getRandom());
    req.auth_token = htonl(rfs->getRandom());
    req.reqType = htons(3);//lseek
    req.action.lseek.offset = htons(offset);
    req.action.lseek.whence = htons(whence);

    /* send request */
    int result = 
      sendto(rfs->server_sd,
        (void *)&req,
        sizeof(struct Request),
        0,
        (struct sockaddr *) &rfs->server_sockaddr, 
        sizeof(struct sockaddr_in)
      );

    if (result < 0) {
        perror("sending to socket");
        exit(EXIT_FAILURE);
    }

    char res_buf[sizeof(struct Response)];
    socklen_t len = sizeof(struct sockaddr_in);

    /* wait for server to respond */
    ssize_t rc =   
      recvfrom(rfs->server_sd, 
        res_buf, 
        sizeof(struct Response), 
        0, 
        (struct sockaddr *) &rfs->server_sockaddr, 
        &len
    );
    
    //check for socket error
    if (rc < 0) {
        std::cout << "1 more retry" << std::endl;
        /* 1 more retry*/
        rc =   
          recvfrom(rfs->server_sd, 
            res_buf, 
            sizeof(struct Response), 
            0, 
            (struct sockaddr *) &rfs->server_sockaddr, 
            &len
        );
      
        if (rc < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    struct Response *res = reinterpret_cast<struct Response *>(&res_buf);
    puts("----------------response from server-------------------");
    printf("Seqnum: %lu\n",(unsigned long)ntohl(res->seqnum));
    printf("Req type: %d\n",ntohs(res->reqType));
    printf("offset: %d\n",(int16_t)ntohs(res->action.lseek.success));
    printf("Error: %d\n",ntohs(res->action.lseek.error));
    printf("EOF: %d\n",ntohs(res->action.lseek.eof));
    puts("-------------------------------------------------------");

    return 0;
}

uint32_t RemoteFileSystem::File::close(){

    cout << "Requesting to close" << endl;
    
    /* create a request */
    struct Request req;
    req.seqnum = htonl(rfs->getRandom());
    req.auth_token = htonl(rfs->getRandom());
    req.reqType = htons(42);//read

    /* send request */
    int result = 
      sendto(rfs->server_sd, 
        (void *)&req, 
        sizeof(struct Request), 
        0, 
        (struct sockaddr *) &rfs->server_sockaddr, 
        sizeof(struct sockaddr_in)
      );

    if (result < 0) {
        perror("sending to socket");
        exit(EXIT_FAILURE);
    }

    char res_buf[sizeof(struct Response)];
    socklen_t len = sizeof(struct sockaddr_in);

    /* wait for server to respond */
    ssize_t rc =   
      recvfrom(rfs->server_sd, 
        res_buf, 
        sizeof(struct Response), 
        0, 
        (struct sockaddr *) &rfs->server_sockaddr, 
        &len
    );
    
    //check for socket error
    if (rc < 0) {
        std::cout << "1 more retry" << std::endl;
        /* 1 more retry*/
        rc =   
          recvfrom(rfs->server_sd, 
            res_buf, 
            sizeof(struct Response), 
            0, 
            (struct sockaddr *) &rfs->server_sockaddr, 
            &len
        );
      
        if (rc < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    struct Response *res = (struct Response *)(&res_buf);
    puts("----------------response from server-------------------");
    printf("Seqnum: %lu\n",(unsigned long)ntohl(res->seqnum));
    printf("Req type: %d\n",ntohs(res->reqType));
    printf("Success: %d\n",(int16_t)ntohs(res->action.close.success));
    puts("-------------------------------------------------------");

    return 0;
}