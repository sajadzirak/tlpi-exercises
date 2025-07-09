#include <unistd.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;
    if ((fd = open("f1", O_CREAT | O_RDWR, S_IRUSR | 
            S_IWUSR | S_IRGRP | S_IROTH)) == -1)
        errExit("open");

    int dupfd;
    if ((dupfd = dup(fd)) == -1)
        errExit("dup");

    off_t off, dupoff;
    if ((off = lseek(fd, 100, SEEK_CUR)) == -1)
        errExit("lseek");

    if ((dupoff = lseek(dupfd, 0, SEEK_CUR)) == -1)
        errExit("lseek");

    printf("same offset: %lld == %lld\n", off, dupoff);

    int flags, dupflags;
    if ((flags = fcntl(fd, F_GETFL)) < 0)
        errExit("fcntl");

    if (fcntl(fd, F_SETFL, flags | O_APPEND) < 0)
        errExit("fcntl");

    if ((dupflags = fcntl(dupfd, F_GETFL)) < 0)
        errExit("fcntl");
    
    printf("same open flags: %d == %d\n", flags | O_APPEND, dupflags);

    if (close(fd) == -1)
        errExit("close");

    if (close(dupfd) == -1)
        errExit("close");

    return 0;
}