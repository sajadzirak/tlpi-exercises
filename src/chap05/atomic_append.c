#include <fcntl.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 3 && argc != 4)
        usageErr("%s filename num-bytes [x]\n", argv[0]);
    
    int oflags = O_WRONLY | O_CREAT | O_APPEND;
    if (argc == 4 && strcmp(argv[3], "x") == 0)
        oflags = O_WRONLY | O_CREAT;

    int fd;
    if ((fd = open(argv[1], oflags, 
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
        errExit("open");

    int numbytes;
    if ((numbytes = atoi(argv[2])) == 0)
        usageErr("%s filename num-bytes [x]\n", argv[0]);

    for (size_t i = 0; i < numbytes; i++)
    {
        if (lseek(fd, 0, SEEK_END) == -1)
            errExit("lseek");
        
        if (write(fd, "\0", 1) != 1)
            errExit("write");
    }

    if (close(fd) == -1)
        errExit("close");

    return 0;
}