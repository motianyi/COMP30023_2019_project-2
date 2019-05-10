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
// int getb();
// int powermod(int g, int p, int b);
void guess(char* filename,int password_length);

void apply_hash(BYTE *hash, BYTE *data, int size);
void compare(char* password_filename, char* hashvalue_filename);




/*********************** MAIN **********************/
int main(int argc, char ** argv){
    // printf("%d\n",argc);
    if(argc == 1){
        char filename4[] = "pwd4sha256";
        char filename6[] = "pwd6sha256";

        // guess(filename4,4);
        guess(filename6,6);
    }else if (argc == 2){

    }else if (argc == 3){
        char* password_filename = argv[1];
        char* hashvalue_filename = argv[2];
        compare(password_filename, hashvalue_filename);
    }else{
        printf("error,wrong number of argument\n");
    }
    return 0;
}

void compare(char* password_filename, char* hashvalue_filename){
    //preprocess hash value file
    
    // Reading size of file
    FILE * file = fopen(hashvalue_filename, "rb");
    if (file == NULL) return;
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fclose(file);
    // Reading data to array of unsigned chars
    file = fopen(hashvalue_filename, "rb");
    unsigned char * in = (unsigned char *) malloc(size);
    int bytes_read = fread(in, sizeof(unsigned char), size, file);
    fclose(file);

    int number_hashes = (int)size/32;

    //convert to 2D array of byte
    BYTE hash_values[number_hashes][32];
    for (int i = 0; i < bytes_read; i++) {
        hash_values[i/32][i%32] = in[i];
    }

    //read pass word file one per line
}

void guess(char* filename,int password_length){

    
    
    // Reading size of file
    FILE * file = fopen(filename, "rb");
    if (file == NULL) return;
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fclose(file);
    // Reading data to array of unsigned chars
    file = fopen(filename, "rb");
    unsigned char * in = (unsigned char *) malloc(size);
    int bytes_read = fread(in, sizeof(unsigned char), size, file);
    fclose(file);

    // // get the size of the file
    // struct stat st;
    // stat(filename, &st);
    // size_t filesize = st.st_size;

    int number_hashes = (int)size/32;
    // printf("\n%d\n\n",number_hashes);

    //convert to 2D array of byte
    BYTE hash_values[number_hashes][32];
    for (int i = 0; i < bytes_read; i++) {
        hash_values[i/32][i%32] = in[i];
    }
    // //print again
    // for(int i = 0; i< number_hashes; i++){
    //     for(int j = 0; j < 32; j++){
    //         printf("%02x", hash_values[i][j]);
    //     }
    //     printf("\n");
    // }



    //brute force
    if(password_length == 4){
        int start = 32;
        int end = 126;
        for(int i = start; i < end; i++){
            for(int j = start; j < end; j++){
                for(int k = start; k < end; k++){
                    for(int l = start; l < end; l++){
                        BYTE guessed_word[4];
                        guessed_word[0] = (unsigned char)i;
                        guessed_word[1] = (unsigned char)j;
                        guessed_word[2] = (unsigned char)k;
                        guessed_word[3] = (unsigned char)l;
                        // printf("%c%c%c%c\n",guessed_word[0],guessed_word[1],guessed_word[2],guessed_word[3]);
                        BYTE hash[32];
                        apply_hash(hash, guessed_word, 4);
                        for(int m = 0; m < number_hashes; m++){
                            int result = memcmp(hash, hash_values[m],32); 
                            if(result == 0){
                                printf("%c%c%c%c %d\n",guessed_word[0],guessed_word[1],guessed_word[2],guessed_word[3], m+1);
                            }

                        }
                    }   
                }
            }
        }
    }else if(password_length == 6){
        printf("6 password ASCII 64 to 122\n");
        int start = 65;
        int end = 122;
         for(int i = start; i < end; i++){
            for(int j = start; j < end; j++){
                for(int k = start; k < end; k++){
                    for(int l = start; l < end; l++){
                        for(int x = start; x < end; x++){
                            for(int y = start; y < end; y++){
                                BYTE guessed_word[6];
                                guessed_word[0] = (unsigned char)i;
                                guessed_word[1] = (unsigned char)j;
                                guessed_word[2] = (unsigned char)k;
                                guessed_word[3] = (unsigned char)l;
                                guessed_word[4] = (unsigned char)x;
                                guessed_word[5] = (unsigned char)y;
                                // printf("%c%c%c%c\n",guessed_word[0],guessed_word[1],guessed_word[2],guessed_word[3]);
                                BYTE hash[32];
                                apply_hash(hash, guessed_word, 6);
                                for(int m = 0; m < number_hashes; m++){
                                    int result = memcmp(hash, hash_values[m],32); 
                                    if(result == 0){
                                        printf("%c%c%c%c%c%c %d\n",guessed_word[0],guessed_word[1],guessed_word[2],guessed_word[3],guessed_word[4],guessed_word[5],m+11);
                                    }

                                }
                            }
                        }
                    }   
                }
            }
        }
    }
}

void apply_hash(BYTE *hash, BYTE *data, int size){

    
    // printf("C\n");    
    
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, data, size);
    sha256_final(&ctx, hash);
    // printf("A\n");
    
}

// int main(int argc, char ** argv)
// {
//     int sockfd, portno, n;
//     struct sockaddr_in serv_addr;
//     struct hostent * server;

//     char buffer[256];

//     if (argc < 3)
//     {
//         fprintf(stderr, "usage %s hostname port\n", argv[0]);
//         exit(0);
//     }

//     portno = atoi(argv[2]);


//     /* Translate host name into peer's IP address ;
//      * This is name translation service by the operating system
//      */
//     server = gethostbyname(argv[1]);

//     if (server == NULL)
//     {
//         fprintf(stderr, "ERROR, no such host\n");
//         exit(0);
//     }

//     /* Building data structures for socket */

//     bzero((char *)&serv_addr, sizeof(serv_addr));

//     serv_addr.sin_family = AF_INET;

//     bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

//     serv_addr.sin_port = htons(portno);

//     /* Create TCP socket -- active open
//     * Preliminary steps: Setup: creation of active open socket
//     */

//     sockfd = socket(AF_INET, SOCK_STREAM, 0);

//     if (sockfd < 0)
//     {
//         perror("ERROR opening socket");
//         exit(0);
//     }

//     if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//     {
//         perror("ERROR connecting");
//         exit(0);
//     }

//     /* Do processing
//     */
//     int g = 15;
//     int p = 97;
//     bzero(buffer, 256);

//     //username
//     char username[] = "tianyim\n";
//     n = write(sockfd, username, strlen(username));
//     printf("username sent\n");

//     if (n < 0)
//     {
//         perror("ERROR writing to socket");
//         exit(0);
//     }
//     //first byte
//     int b = getb();
//     // int gbmodp = pow(g, b)%p;
//     int gbmodp = powermod(g,p,b);

//     printf("gbmodp =%d\n",gbmodp);

//     char sgbmodp[256];
//     // itoa(gbmodp, sgbmodp, 10);
//     sprintf(sgbmodp, "%d\n", gbmodp);

//     //send gbmodp
//     n = write(sockfd, sgbmodp,strlen(sgbmodp));

//     if (n < 0)
//     {
//         perror("ERROR writing to socket");
//         exit(0);
//     }


//     //receive gamodp
//     bzero(buffer, 256);

//     n = read(sockfd, buffer, 255);

//     if (n < 0)
//     {
//         perror("ERROR reading from socket");
//         exit(0);
//     }

//     printf("received is %s\n", buffer);

//     int gamodp = atoi(buffer);
    

//     //calculate gbamodp
//     int gbamodp = powermod(gamodp,p,b);
//     printf("gbamodp =%d\n",gbamodp);

    
//     // int gbamodp = pow(gamodp%p,b)%p;
//     char sgbamodp[256];
//     // itoa(gbamodp, sgbamodp, 10);
//     sprintf(sgbamodp, "%d\n", gbamodp);

 

//      //send gbmodp
//     n = write(sockfd, sgbamodp,strlen(sgbamodp));

//     if (n < 0)
//     {
//         perror("ERROR writing to socket");
//         exit(0);
//     }

//     bzero(buffer, 256);


//     //receive status report
//     n = read(sockfd, buffer, 255);

//     if (n < 0)
//     {
//         perror("ERROR reading from socket");
//         exit(0);
//     }

//     printf("%s\n", buffer);



//     close(sockfd);

//     return 0;
// }



// int getb(){

//     // char html[2049];
//     // get the size of the file
//     // struct stat st;

//     char filename[] = "dh.c";


//     // get the size of the file
//     struct stat st;
//     stat(filename, &st);
    
//     //store size
//     size_t size = st.st_size;

//     char * buffer = 0;
//     long length;
//     FILE * f = fopen (filename, "rb");

//     if (f){
//         fseek (f, 0, SEEK_END);
//         length = ftell (f);
//         fseek (f, 0, SEEK_SET);
//         buffer = malloc (length);
//         if (buffer){
//             size_t s = fread (buffer, 1, length, f);
//             if(s<0){
//                 printf("error\n");
//             }
//         }
//         fclose (f);
//     }
//     // printf("%s",buffer);
//     printf("D\n");




//     unsigned char* data0 = (unsigned char*)buffer;
//     printf("C\n");
//     BYTE data[size];
//     memcpy(data, data0, size);
    
//     printf("B\n");
//     BYTE hash[32];
//     SHA256_CTX ctx;
//     // sha256_transform(ctx, data);
//     sha256_init(&ctx);
//     sha256_update(&ctx, data, size);
//     sha256_final(&ctx, hash);
//     printf("A");
    
//     printf("hashedChars: ");
//     for (int i = 0; i < 32; i++) {
//         printf("%x", hash[i]);
//     }
//     printf("\n");

//     unsigned char first = hash[0];
// 	 printf("%x\n", hash[0]);
//     int b = 0;
// 	b = (unsigned int)first;
    
//     printf("b = %d\n", b);
//     return b;
// }




// int powermod(int g, int p, int b)
// {
// 	// int g = 15;
// 	// int p = 97;
// 	// int b = 256;
// 	// double r;
// 	// r = fmod(pow(g,b),p);

// 	int i;
// 	int product = 1;
// 	for(i=0; i<b; i++){
// 		product *= g;
// 		if(product > p){
// 			product = product%p;
// 		}
// 	}

// 	printf("%d\n",product);
//     return product;
    
// }