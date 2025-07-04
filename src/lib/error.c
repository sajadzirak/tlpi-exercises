#include <stdlib.h>
#include <stdio.h>

void error_handler(const char *error_msg)
{
    perror(error_msg);
    exit(EXIT_FAILURE);
}

void usage_error(const char *msg)
{
    fprintf(stderr, msg);
    exit(EXIT_FAILURE);
}