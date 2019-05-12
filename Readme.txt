RC4 Standalone + S3FS Integrated RC4 Encryption


This package contains two separate components. The first is a standalone program that can encrypt and decrypt files using RC4.
The second is a modified version of S3FS-Fuse which integrates this encryption functionality to automatically encrypt files 
uploaded from your local bucket and decrypt files downloaded from S3. Both the standalone program and modified 
S3FS-Fuse use sha256 and salting to hash the password before encryption. This means they are also both backwards-compatible 
with OpenSSL. 


In order to run the standalone program, first download the file rc4Salt.cpp and compile using the 
command “g++ rc4Salt.cpp -lssl -lcrypto”. Next, select the executable file and pass the following parameters operation 
(e for encryption or d for decryption), password, inputfile, outputfile. An example would be “./a.out e hotdog input.txt output.txt”. 
In order to decrypt it, “./a.out d hotdog output.txt outputdec.txt”.


In order to install the modified S3FS-Fuse, you must first complete some prerequisite instructions. 
1. Create an Amazon S3 account if you don’t already have one from https://aws.amazon.com/s3/ and setup a bucket with your 
desired settings.
2. Install the required dependencies using the command “sudo apt-get install automake autotools-dev g++ git libcurl4-gnutls-dev 
libfuse-dev libssl-dev lib-xml2-dev make pkg-config” 
3. Download the full S3FS-Fuse installation from https://github.com/s3fs-fuse/s3fs-fuse.
4. Replace the fdcache.cpp and fdcache.h files in the src directory and the Makefile on the root of the official download 
with the provided versions.
5. Navigate into your S3FS folder and type the commands “./autogen.sh ./configure make sudo make install”
6. Create a password file using your bucket’s Access Key ID and Secret Access Key with the command 
“echo ACCESS_KEY_ID:SECRET_ACCESS_KEY > ${HOME}/.passwd-s3fs
chmod 600 ${HOME}/.passwd-s3fs”
7. Mount your local bucket with the command “s3fs mybucket /path/to/mountpoint -o passwd_file=${HOME}/.passwd-s3fs”


At this point, you should have a fully functioning S3FS installation that can automatically encrypt files being uploaded 
from local bucket and decrypt files being downloaded. To test, either drag and drop a file into your mounted bucket folder 
or run the command “cp sample.txt /path/to/mountpoint”. Attempt to open the file inside the bucket. It should be encrypted 
with the first 8 characters reading “Salted__”. To test the decryption function, you move the file from the bucket to a different directory, and then move it back. It should now be decrypted. You can confirm that it fully decrypted by running the “diff” 
command between the original copy of the file with the encrypted then decrypted copy of the file.
