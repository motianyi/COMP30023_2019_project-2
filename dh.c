/* A simple dh.c program 
Tianyi Mo*/
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
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/sendfile.h>
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

    //send username
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

    //calculate (g^b)mod p
    int gbmodp = powermod(g,p,b);

    // printf("gbmodp =%d\n",gbmodp);

    //convert to char
    char sgbmodp[256];
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
    //convert char int
    int gamodp = atoi(buffer);
    

    //calculate gbamodp
    int gbamodp = powermod(gamodp,p,b);
    printf("gbamodp =%d\n",gbamodp);

    //convert int to char array
    char sgbamodp[256];
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

    //apply hash on the content of this file(dh.c)

    char filename[] = "dh.c";


    // get the size of the file
    struct stat st;
    stat(filename, &st);
    
    //store size
    size_t size = st.st_size;

    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    //load the file
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

    //allocate buffer to it
    unsigned char* data0 = (unsigned char*)buffer;
    
    BYTE data[size];
    memcpy(data, data0, size);
    
    BYTE hash[32];
    SHA256_CTX ctx;
    
    //apply sha256 to the contents
    sha256_init(&ctx);
    sha256_update(&ctx, data, size);
    sha256_final(&ctx, hash);
    
    
    
    unsigned char first = hash[0];
    int b = 0;
	b = (unsigned int)first;
    
    return b;
}




int powermod(int g, int p, int b)
{
	//do power first and then do mod on p

	int i;
	int product = 1;
	for(i=0; i<b; i++){
		product *= g;
		if(product > p){
			product = product%p;
		}
	}

	// printf("%d\n",product);
    return product;
    
}