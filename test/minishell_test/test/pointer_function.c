#include <unistd.h>
#include "../libft/includes/libft.h"

int sum1(int x, int y)
{
    return(x + y);
}
int sub1(int x, int y)
{
    return(x - y);
}
int mult(int x, int y)
{
    return(x * y);
}
int div1(int x, int y)
{
    if (y != 0) return (x / y); else  return 0;
}

int     get_ope(char *fun_name)
{
    if (!strcmp(fun_name, "sum1"))
        return 0;
    if (!strcmp(fun_name, "sub1"))
        return 1;
    if (!strcmp(fun_name, "mult"))
        return 2;
    if (!strcmp(fun_name, "div1"))
        return 3;
    return -1;
}

int (*ope[])(int, int) = {sum1, sub1, mult, div1};

int main()
{
    int x = 50;
    int y = 10;
    int ret;
    char function[5];

    read(0, function, 5);
    function[4] = 0;
    if ((ret = get_ope(function)) >= 0) {
        printf("the number is: %d\n", ope[ret](x, y));
    } else {
        printf("bad command: %s\n", function);
    }


    return 0;
}
