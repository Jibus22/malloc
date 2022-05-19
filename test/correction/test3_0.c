#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "malloc.h"

#define M (1024 * 1024)

void print(char *s)
{
    write(1, s, strlen(s));
}

int main()
{ char *addr1; char *addr3;

    addr1 = (char*)malloc(16*M);
    strcpy(addr1, "Bonjour\n");
    print(addr1);
    addr3 = (char*)realloc(addr1, 128*M);
    addr3[127*M] = 42;
    print(addr3);
    return (0);
}
