

/* Sample UDP server */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "Request.h"
#include "Response.h"

#include <iostream>

int main(){

    FILE * f = NULL;
    short port = atoi("10000");

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    int rc = 
        bind(sockfd, 
            (struct sockaddr *) &server_address, 
            sizeof(server_address)
        );
        
    if (rc < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    for (;;) {                
        /*
            receive request from client
        */
        char req_buf[sizeof(struct Request)];
        struct sockaddr_in client_address;
        socklen_t len = sizeof(client_address);

        int n = 
            recvfrom(sockfd, 
                req_buf, 
                sizeof(struct Request), 
                0, 
                (struct sockaddr *) &client_address, 
                &len
            );
        if (n < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        
        struct Request *req = (struct Request *)(req_buf);

        switch(ntohs(req->reqType)) 
        {
            case 0: {//open
                puts("-------------------------------------------------------");
                printf("Received from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                printf("Seq num: %lu\n",(unsigned long)ntohl(req->seqnum));
                printf("Auth token: %lu\n",(unsigned long)ntohl(req->auth_token));
                printf("Path name: %s\n",req->action.open.pathname);
                printf("Req type: %d\n",ntohs(req->reqType));
                printf("Mode: %s\n",req->action.open.mode);
                puts("-------------------------------------------------------");
                
                /* open a file */
                f = fopen(req->action.open.pathname, req->action.open.mode);
                    
                /* send response back to client */
                struct Response res;
                res.seqnum = req->seqnum;
                res.reqType = req->reqType;
                res.action.open.success = htons(f == NULL ? 1 : 0);         
                
                // sleep(10);

                sendto(sockfd, 
                    (void *)&res, 
                    sizeof(struct Response), 
                    0, 
                    (struct sockaddr *)&client_address,
                    sizeof(client_address)
                );

                break;
            }
            case 1: {//read
                puts("-------------------------------------------------------");
                printf("Received from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                printf("Seq num: %lu\n",(unsigned long)ntohl(req->seqnum));
                printf("Auth token: %lu\n",(unsigned long)ntohl(req->auth_token));
                printf("Req type: %d\n",ntohs(req->reqType));
                printf("Size to read: %d\n",ntohs(req->action.read.size));
                puts("-------------------------------------------------------");
                
                size_t count = ntohs(req->action.read.size);

                /* read a file */
                char *buf = (char *)malloc(count);

                /* Read and display data */
                size_t size_read = fread(buf, 1, count, f);

                /* send response back to client */
                struct Response res;
                res.seqnum = req->seqnum;
                res.reqType = req->reqType;
                res.action.read.size = htons(size_read);
                res.action.read.error = htons(ferror(f));
                res.action.read.eof = htons(feof(f));   
                memcpy(res.action.read.buf, buf, size_read);

                sendto(sockfd, 
                    (void *)&res, 
                    sizeof(struct Response), 
                    0, 
                    (struct sockaddr *)&client_address,
                    sizeof(client_address)
                );

                free(buf);

                break;
            }
            case 2: {//write
                puts("-------------------------------------------------------");
                printf("Received from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                printf("Seq num: %lu\n",(unsigned long)ntohl(req->seqnum));
                printf("Auth token: %lu\n",(unsigned long)ntohl(req->auth_token));
                printf("Req type: %d\n",ntohs(req->reqType));
                printf("Size to write: %d\n",ntohs(req->action.write.size));
                printf("nitems: %d\n",ntohs(req->action.write.nitems));
                puts("-------------------------------------------------------");
                
                /* Read and display data */
                size_t size_written = fwrite(req->action.write.buf, ntohs(req->action.write.size), ntohs(req->action.write.nitems)-1, f);

                fflush(f);

                /* send response back to client */
                struct Response res;
                res.seqnum = req->seqnum;
                res.reqType = req->reqType,
                res.action.write.size = htons(size_written);
                res.action.write.error = htons(ferror(f));
                res.action.write.eof = htons(feof(f));   
                
                sendto(sockfd, 
                    (void *)&res, 
                    sizeof(struct Response), 
                    0, 
                    (struct sockaddr *)&client_address,
                    sizeof(client_address)
                );

                break;
            }
            case 3: {//lseek
                puts("-------------------------------------------------------");
                printf("Received from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                printf("Seq num: %lu\n",(unsigned long)ntohl(req->seqnum));
                printf("Auth token: %lu\n",(unsigned long)ntohl(req->auth_token));
                printf("Req type: %d\n",ntohs(req->reqType));
                printf("Offset: %d\n",ntohs(req->action.lseek.offset));
                printf("Whence: %d\n",ntohs(req->action.lseek.whence));
                puts("-------------------------------------------------------");

                /* Read and display data */
                int32_t success = fseek(f,ntohs(req->action.lseek.offset),ntohs(req->action.lseek.whence));

                /* send response back to client */
                struct Response res;
                res.seqnum = req->seqnum;
                res.reqType = req->reqType;
                res.action.lseek.success = htons(success);
                res.action.lseek.error = htons(ferror(f));
                res.action.lseek.eof = htons(feof(f));       

                sendto(sockfd, 
                    (void *)&res, 
                    sizeof(struct Response), 
                    0, 
                    (struct sockaddr *)&client_address,
                    sizeof(client_address)
                );

                break;
            }
            case 4: {//chmod
                puts("-------------------------------------------------------");
                printf("Received from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                printf("Seq num: %lu\n",(unsigned long)ntohl(req->seqnum));
                printf("Auth token: %lu\n",(unsigned long)ntohl(req->auth_token));
                printf("Req type: %d\n",ntohs(req->reqType));
                printf("Pathname: %s\n",req->action.chmod.pathname);
                printf("mode: %d\n",(mode_t)ntohs(req->action.chmod.mode));
                puts("-------------------------------------------------------");

                /* chmode */
                mode_t mode = ntohs(req->action.chmod.mode);
                // char s[4]; 
                // sprintf(s,"%d", mode);
                int success = chmod(req->action.chmod.pathname, mode);//strtol(s, 0, 8));

                /* send response back to client */
                struct Response res;
                res.seqnum = req->seqnum;
                res.reqType = req->reqType;
                res.action.chmod.success = htons(success);

                sendto(sockfd, 
                    (void *)&res, 
                    sizeof(struct Response), 
                    0, 
                    (struct sockaddr *)&client_address,
                    sizeof(client_address)
                );

                break;
            }
            case 5: {//unlink
                puts("-------------------------------------------------------");
                printf("Received from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                printf("Seq num: %lu\n",(unsigned long)ntohl(req->seqnum));
                printf("Auth token: %lu\n",(unsigned long)ntohl(req->auth_token));
                printf("Req type: %d\n",ntohs(req->reqType));
                printf("Pathname: %s\n",req->action.unlink.pathname);
                puts("-------------------------------------------------------");

                /* chmode */
                int success = unlink(req->action.unlink.pathname);

                /* send response back to client */
                struct Response res;
                res.seqnum = req->seqnum;
                res.reqType = req->reqType;
                res.action.unlink.success = htons(success);

                sendto(sockfd, 
                    (void *)&res, 
                    sizeof(struct Response), 
                    0, 
                    (struct sockaddr *)&client_address,
                    sizeof(client_address)
                );

                break;
            }
            case 6: {//rename
                puts("-------------------------------------------------------");
                printf("Received from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                printf("Seq num: %lu\n",(unsigned long)ntohl(req->seqnum));
                printf("Auth token: %lu\n",(unsigned long)ntohl(req->auth_token));
                printf("Req type: %d\n",ntohs(req->reqType));
                printf("Old Path: %s\n",req->action.rename.oldpath);
                printf("New Path: %s\n",req->action.rename.newpath);
                puts("-------------------------------------------------------");

                /* chmode */
                int16_t success = rename(req->action.rename.oldpath, req->action.rename.newpath);

                /* send response back to client */
                struct Response res;
                res.seqnum = req->seqnum;
                res.reqType = req->reqType;
                res.action.rename.success = htons(success);

                sendto(sockfd, 
                    (void *)&res, 
                    sizeof(struct Response), 
                    0, 
                    (struct sockaddr *)&client_address,
                    sizeof(client_address)
                );

                break;
            }
            case 42: {//close
                puts("-------------------------------------------------------");
                printf("Received from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                printf("Seq num: %lu\n",(unsigned long)ntohl(req->seqnum));
                printf("Auth token: %lu\n",(unsigned long)ntohl(req->auth_token));
                printf("Req type: %d\n",ntohs(req->reqType));
                puts("-------------------------------------------------------");

                /* close a file */
                int rc = fclose(f);                

                /* send response back to client */
                struct Response res;
                res.seqnum = req->seqnum;
                res.reqType = req->reqType;
                res.action.close.success = htons(rc);         

                sendto(sockfd, 
                    (void *)&res, 
                    sizeof(struct Response), 
                    0, 
                    (struct sockaddr *)&client_address,
                    sizeof(client_address)
                );

                break;
            }
            default: {//push the message to queue

            }
        }//end switch
    }
}