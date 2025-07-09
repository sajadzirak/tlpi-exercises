#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int fcntl_dup(int oldfd)
{
    if (fcntl(oldfd, F_GETFL) < 0)
    {
        errno = EBADF;
        return -1;
    }
    
    int newfd = fcntl(oldfd, F_DUPFD, 0);
    return newfd;
}

int fcntl_dup2(int oldfd, int newfd)
{
    if (fcntl(oldfd, F_GETFL) < 0)
    {
        errno = EBADF;
        return -1;
    }
    
    if (oldfd == newfd)
        return newfd;

    close(newfd);  // ignore close errors like what dup2 does

    int fd;
    if ((fd = fcntl(oldfd, F_DUPFD, newfd)) != newfd)
    {
        close(fd);
        return -1;
    }

    return fd;
}