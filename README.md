# steganography
Steganography program in C created by Thomas Rey. This program allows the user to hide a string of characters into an image, this works by changing individual rgb values of
pixels into a new image. It also allows for the decryption of messages by comparing the new image with the old one. NOTE: this only works for uncompressed BMP files, https://online-converting.com/image/convert2bmp/.
This website is useful as it allows for the convertion of JPG files to uncompressed BMP format.

# How to use
## Compiling
Compile the files with the Makefile by typing 'make' in the terminal, otherwise use 'gcc -Wall -pedantic -o steg steg.c image.c'. Of course this will require your system to have
gcc installed. 
## Running the program
Once compiled run the program from the terminal by using ./steg, execution arguments are required. To hide a message into a file run ./steg encrypt IMAGENAME.bmp, this will create
a new image file called imageEncrypted.bmp. To decrypt a message from two images run ./steg decrypt IMAGENAME.bmp ENCRYPTEDIMAGENAME.bmp, the order does not matter.
