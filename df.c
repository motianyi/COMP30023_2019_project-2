/* A simple client program for server.c

   To compile: gcc client.c -o client

   To run: start the server, then the client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "sha256.h"


int main(int argc, char ** argv)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent * server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);


    /* Translate host name into peer's IP address ;
     * This is name translation service by the operating system
     */
    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    /* Building data structures for socket */

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    /* Create TCP socket -- active open
    * Preliminary steps: Setup: creation of active open socket
    */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(0);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(0);
    }

    /* Do processing
    */
    int g = 15;
    int p = 97;
    bzero(buffer, 256);

    //username
    char username[7] = "tianyim\n";

    n = write(sockfd, username, strlen(username));

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(0);
    }
    //first byte
    int b = 5;
    int gbmodp = pow(g, b)%p;

    char sgbmodp[256];
    itoa(gbmodp, sgbmodp, 10);

    //send gbmodp
    n = write(sockfd, sgbmodp,strlen(sgbmodp));

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(0);
    }


    //receive gamodp
    bzero(buffer, 256);

    n = read(sockfd, buffer, 255);

    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(0);
    }

    printf("%s\n", buffer);

    int gamodp = atoi(buffer);

    //calculate gbamodp

    int gbamodp = pow(g%p,a)%p;
    char sgbamodp[256];
    itoa(gbamodp, sgbamodp, 10);

     //send gbmodp
    n = write(sockfd, sgbamodp,strlen(sgbamodp));

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(0);
    }

    bzero(buffer, 256);


    //receive status report
    n = read(sockfd, buffer, 255);

    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(0);
    }

    printf("%s\n", buffer);



    // while(1){

    //     bzero(buffer, 256);

    //     fgets(buffer, 255, stdin);
        

    //     n = write(sockfd, buffer, strlen(buffer));

    //     if (n < 0)
    //     {
    //         perror("ERROR writing to socket");
    //         exit(0);
    //     }

    //     bzero(buffer, 256);

    //     n = read(sockfd, buffer, 255);

    //     if (n < 0)
    //     {
    //         perror("ERROR reading from socket");
    //         exit(0);
    //     }

    //     printf("%s\n", buffer);
    // }
    close(sockfd);

    return 0;
}