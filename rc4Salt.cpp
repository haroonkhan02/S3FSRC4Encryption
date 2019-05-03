#include <iostream>
#include <string>
#include <fstream>
#include <openssl/rc4.h>
//#include "rc4_skey.c"
//#include "rc4_enc.c"
#include <openssl/evp.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <vector>
#include <openssl/rand.h>

using namespace std;


int main(int argc, char *argv[]){

//User should type executuble + encryption/decryption + password + inputFile + outputFile

//Need to add more error checking. Does file input exist? Too many arguments given?


if(argc<5) //Check for sufficient arguments
{
printf("Error, too few arguments. Give encryption/decryption (e/d) + password + inputFile + outputFile");
return 0;
}


//Define Variables

int fd = open(argv[3],O_RDONLY); //Open input file

int fd2 = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0644); //Open output file with appropiate permissions

const int buffSize = 4000000;

unsigned char buffer[buffSize]; //Buffer that will hold file (4mb limit currently)

int fileSize = read(fd,buffer,buffSize); // Size of file

unsigned char outputBuffer[buffSize]; // Buffer that will hold encrypted/decrypted file

unsigned char salt[8]; //Char array storing salt hash

unsigned char *salty; //Char array pointer that will either point to salt generated (encryption) or salt hash read from file (decryption)

char *pass = argv[2]; //char pointer to password

static const char magic[] = "Salted__"; //Char array containing whether file has been salted or not

bool encrypting = false; //Flag if encryption operation


//Determine encryption or decryption
//If encryption selected
if(strcmp(argv[1],"e") == 0)
{
RAND_bytes(salt, sizeof(salt)); //Generate random bytes in salt array
write(fd2,magic,8); //Write the term Salted__ in first 8 bytes of output file
write(fd2,salt,8); //Write the salted hash in next 8 bytes of output file
encrypting = true; //Set flag true
}

//If decryption selected
else if (strcmp(argv[1],"d") == 0)
{
lseek(fd,8,SEEK_SET); //Move cursor to 8th position (where salt hash starts)
read(fd,salt,8); //Read 8 bytes from the 8th postion (contains the salt hash) and store in salt array
}

//If invalid option selected
else
{
printf("Error: You must enter either e for encryption or d for decryption %s",argv[1]);
return 0;
}


salty = salt; //Set salty pointer to salt array that contains the salt hash (salt hash generated if encryption selected or salt hash read from file if decryption selected)

unsigned char newPassword[16];//[EVP_MAX_KEY_LENGTH];
EVP_BytesToKey(EVP_rc4(),EVP_sha256(), salty, (const unsigned char *)pass, strlen(pass), 1, newPassword, NULL); //Create Openssl compatable password with salt stored in "newpPassword"

int newPassLength = 16; //Size of newPassword array



//Encrypt using RC4

if(!encrypting) //If decrypting, skip the salt part
{
lseek(fd,16,SEEK_SET); //Set cursor to after salted stuff
fileSize = read(fd,buffer,buffSize); //Update fileSize
}

RC4_KEY key;

RC4_set_key(&key, newPassLength, (const unsigned char*)newPassword); //Set RC4 key using newpassword

RC4(&key, fileSize, (const unsigned char*)buffer, outputBuffer);
lseek(fd2,0,SEEK_END); //Move cursor to end of output file
write(fd2,outputBuffer,fileSize); //Move contents there


return 0;
}

	        
