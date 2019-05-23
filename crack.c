/* COMP30023 Computer System Assignment2
guess password
Tianyi Mo
875556
May 2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>


#include "sha256.h"

/*********************** FUNCTION DECLARATIONS **********************/
// int getb();
// int powermod(int g, int p, int b);
void guess(char* filename,int password_length);

void apply_hash(BYTE *hash, BYTE *data, int size);
void compare(char* password_filename, char* hashvalue_filename);
void goodguess(int length);
int printResult(int lo, int hi, int length);




/*********************** MAIN **********************/
int main(int argc, char ** argv){
    if(argc == 1){
        // char filename4[] = "pwd4sha256";
        // char filename6[] = "pwd6sha256";

        // guess(filename4,4);
        // guess(filename6,6);
    }else if (argc == 2){

        //generate gusses
        goodguess(atoi(argv[1]));

    }else if (argc == 3){

        // compare hash and file
        char* password_filename = argv[1];
        char* hashvalue_filename = argv[2];
        compare(password_filename, hashvalue_filename);
        
    }else{
        printf("error,wrong number of argument\n");
    }
    return 0;
}

void compare(char* password_filename, char* hashvalue_filename){

    
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

    //read the password file
    FILE *fp;
    int MAXCHAR = 16;
    char word[MAXCHAR];
     
    fp = fopen(password_filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",password_filename);
    }
     
    //read the password file row by row
    while (fgets(word, MAXCHAR, fp) != NULL){
        
        strtok(word, "\n");
        BYTE unsigned_word[MAXCHAR];
        memcpy(unsigned_word, word, MAXCHAR);
        BYTE hash[32];

        //generate hash
        apply_hash(hash, unsigned_word, strlen(word));

        //compare hash
        for(int m = 0; m < number_hashes; m++){
            int result = memcmp(hash, hash_values[m],32); 
            if(result == 0){
                printf("%s %d\n",word,m+1);
            }
        }
    }
    fclose(fp);
    
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

   

    int number_hashes = (int)size/32;
    // printf("\n%d\n\n",number_hashes);

    //convert to 2D array of byte
    BYTE hash_values[number_hashes][32];
    for (int i = 0; i < bytes_read; i++) {
        hash_values[i/32][i%32] = in[i];
    }


    //brute force guess
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
        //generate all string from ascii value start to end
        int start = 48;
        int end = 89;
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
                                
                                BYTE hash[32];

                                //apply hash to generated word
                                apply_hash(hash, guessed_word, 6);

                                //compare with each hash value
                                for(int m = 0; m < number_hashes; m++){
                                    int result = memcmp(hash, hash_values[m],32); 
                                    if(result == 0){

                                        //print the result
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

    //do the hash steps in order
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, size);
    sha256_final(&ctx, hash);
}

void goodguess(int length){
    // int l = length;

    FILE *fp;
    int MAXCHAR = 16;
    char word[MAXCHAR];
     
    fp = fopen("freqwords.txt", "r");
    if (fp == NULL){
        printf("Could not open file %s","freqwords.txt");
    }

    // get length of the input word length
    int filelength= 0;
    while (fgets(word, MAXCHAR, fp) != NULL){
        filelength += 1;
    }
     
    char words[filelength][7];
    int i = 0;
    fp = fopen("freqwords.txt", "r");

    //store the file in 2d array
    while (fgets(word, MAXCHAR, fp) != NULL){
        strtok(word, "\n");
        strcpy(words[i], word);
        words[i][6] = '\0';
        i ++;
    }

    i = 0;

    //print the common passwords
    while(length>0 && i<filelength){
        printf("%s\n",words[i]);
        length--;
        i++;
    }

    //print common passwords with 1 capital letter
    if(length>0){
        for(int bit=0; bit<6; bit++){
            for(i = 0; i<filelength; i++){

                char newWord[7];
                strcpy(newWord,words[i]);
                if(newWord[bit]<='z' && 'a'<= newWord[bit]){
                    newWord[bit] = newWord[bit] - 32;
                }

                printf("%s\n",newWord);
                length--;
                if(length<=0){
                    break;
                }
            }if(length<=0){
                break;
            }
            
        }
    }
    
    //print common passwords with 2 capital letter
    if(length>0){
        
        for(int bit=0; bit<6; bit++){
            for(int bit2=0; bit2<6; bit2++){
                if(bit == bit2){
                    break;
                }
                for(i = 0; i<filelength; i++){
                    
                    char newWord[7];
                    strcpy(newWord,words[i]);

                    //change two characters to capital
                    if(newWord[bit]<='z' && 'a'<= newWord[bit]){
                        newWord[bit] = newWord[bit] - 32;
                    }
                    if(newWord[bit2]<='z' && 'a'<= newWord[bit2]){
                        newWord[bit2] = newWord[bit2] - 32;
                    }

                    printf("%s\n",newWord);
                    length--;
                    if(length<=0){
                        break;
                    }
                }if(length<=0){
                    break;
                }
            }if(length<=0){
                break;
            }
        }
    }

    

    // guss all small letters
    if(length>0){
        length = printResult(97, 122, length);
    }
    // guss all letters
    if(length>0){
        length = printResult(65, 122, length);
    }
    // guss all numbers and letters
    if(length>0){
        length = printResult(48, 122, length);
    }

    // guss all possible ascii
    if(length>0){
        length = printResult(32, 122, length);
    }
    
    if(length>0){
        printf("parameter too large");
    }    
}


int printResult(int lo, int hi, int length){
    //print guesses
    for(int a= lo; a < hi; a++){
        for(int b= lo; b < hi; b++){
            for(int c= lo; c < hi; c++){
                for(int d= lo; d < hi; d++){
                    for(int e= lo; e < hi; e++){
                        for(int f= lo; f < hi; f++){
                            char newWord[7];
                            newWord[0] = (char)a;
                            newWord[1] = (char)b;
                            newWord[2] = (char)c;
                            newWord[3] = (char)d;
                            newWord[4] = (char)e;
                            newWord[5] = (char)f;
                            newWord[6] = '\0';
                            printf("%s\n",newWord);
                            length--;
                            if(length<=0){
                                break;
                            }
                        }if(length<=0){
                            break;
                        }
                    }if(length<=0){
                        break;
                    }               
                }if(length<=0){
                    break;
                }
            }if(length<=0){
                break;
            }
        }if(length<=0){
            break;
        }
    }
    return length;
}