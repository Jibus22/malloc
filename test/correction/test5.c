#include <stdlib.h>

#include "malloc.h"

int main()
{
    char *addr1;

    write(1, "start\n", 6);
    addr1 = malloc(1024);
    addr1 = malloc(1024 * 32);
    addr1 = malloc(1024 * 1024);
    addr1 = malloc(1024 * 1024 * 16);
    addr1 = malloc(1024 * 1024 * 128);
    show_alloc_mem(); 
    return (0); 
}
