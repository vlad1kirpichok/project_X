/* oblique 2010
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "elf.h"
#include <sys/stat.h>
#include <memory.h>
#include "infector.h"
#include "common.h"


int main() {
    //    printf("Mister!:\n");
    printf("Hello world!:\n");
/*
    printf("Init params:\n");
    printf("1. Input file name: hello_world_source_app\n");
    printf("2. Inserted code file: mycode.c\n");
    printf("Write new elf executable file start.\n");
    printf("\t..increase e_shoff by PAGE_SIZE in the ELF header\n");
    printf("\t..patch the insertion code (parasite) to jump to the entry point (original)\n");
    printf("\t..locate the text segment program header\n");
    printf("\t..modify the entry point of the ELF header to point to the new code (p_vaddr + p_filesz)\n");
    printf("\t..increase p_filesz by account for the new code (parasite)\n");
    printf("\t..increase p_memsz to account for the new code (parasite)\n");
    printf("\t..for each phdr who's segment is after the insertion (text segment) increase p_offset by PAGE_SIZE\n");
    printf("\t..for the last shdr in the text segment increase sh_size by the parasite length\n");
    printf("\t..for each shdr who's section resides after the insertion increase sh_offset by PAGE_SIZE\n");
    printf("\t..physically insert the new code (parasite) and pad to PAGE_SIZE, into the file - text segment p_offset + p_filesz (original)\n");
    printf("Write new elf executable file success.\n");
    printf("Written output file name: hello_world_with_my_code_app\n");
    */
    return 0;
}

