#include <setjmp.h>

jmp_buf env;

void f1()
{
    setjmp(env);
    return;
}

int main(int argc, char *argv[])
{
    f1();

    longjmp(env, 1);

    return 0;
}

/* 
    In the result, our program will encounter segmentation fault:
    [1]    43625 segmentation fault (core dumped)  ./p02 
*/