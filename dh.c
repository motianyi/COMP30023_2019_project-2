/* A simple client program  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>


#include "sha256.h"

/*********************** FUNCTION DECLARATIONS **********************/
int getb();
int powermod(int g, int p, int b);



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
    char username[] = "tianyim\n";
    n = write(sockfd, username, strlen(username));
    printf("username sent\n");

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(0);
    }
    //first byte
    int b = getb();
    // int gbmodp = pow(g, b)%p;
    int gbmodp = powermod(g,p,b);

    printf("gbmodp =%d\n",gbmodp);

    char sgbmodp[256];
    // itoa(gbmodp, sgbmodp, 10);
    sprintf(sgbmodp, "%d\n", gbmodp);

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

    printf("received is %s\n", buffer);

    int gamodp = atoi(buffer);
    

    //calculate gbamodp
    int gbamodp = powermod(gamodp,p,b);
    printf("gbamodp =%d\n",gbamodp);

    
    // int gbamodp = pow(gamodp%p,b)%p;
    char sgbamodp[256];
    // itoa(gbamodp, sgbamodp, 10);
    sprintf(sgbamodp, "%d\n", gbamodp);

 

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



    close(sockfd);

    return 0;
}



int getb(){

    // char html[2049];
    // get the size of the file
    // struct stat st;

    char filename[] = "dh.c";


    // get the size of the file
    struct stat st;
    stat(filename, &st);
    
    //store size
    size_t size = st.st_size;

    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f){
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer){
            size_t s = fread (buffer, 1, length, f);
            if(s<0){
                printf("error\n");
            }
        }
        fclose (f);
    }
    // printf("%s",buffer);
    printf("D\n");




    unsigned char* data0 = (unsigned char*)buffer;
    printf("C\n");
    BYTE data[size];
    memcpy(data, data0, size);
    
    printf("B\n");
    BYTE hash[32];
    SHA256_CTX ctx;
    // sha256_transform(ctx, data);
    sha256_init(&ctx);
    sha256_update(&ctx, data, size);
    sha256_final(&ctx, hash);
    printf("A");
    
    printf("hashedChars: ");
    for (int i = 0; i < 32; i++) {
        printf("%x", hash[i]);
    }
    printf("\n");

    unsigned char first = hash[0];
	 printf("%x\n", hash[0]);
    int b = 0;
	b = (unsigned int)first;
    
    printf("b = %d\n", b);
    return b;
}




int powermod(int g, int p, int b)
{
	// int g = 15;
	// int p = 97;
	// int b = 256;
	// double r;
	// r = fmod(pow(g,b),p);

	int i;
	int product = 1;
	for(i=0; i<b; i++){
		product *= g;
		if(product > p){
			product = product%p;
		}
	}

	printf("%d\n",product);
    return product;
    
}