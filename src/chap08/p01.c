#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>

/*
    since getpwnam() returns a pointer to a statically allocated struct passwd,
    the second call to getpwnam() will overwrite the previous one, and we see
    the same uid in the output.
*/

int main(int argc, char *argv[])
{
    printf("%ld %ld\n", (long) (getpwnam("avr")->pw_uid), 
    (long) (getpwnam("tsr")->pw_uid));

    return 0;
}