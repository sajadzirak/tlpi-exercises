#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
    ssize_t total_read = 0;

    for (int i = 0; i < iovcnt; i++)
    {
        ssize_t buf_total_read = 0;
        while (buf_total_read < iov[i].iov_len)
        {
            ssize_t bytes_read = read(fd, 
                                (char *)iov[i].iov_base + buf_total_read, 
                                iov[i].iov_len - buf_total_read);
            if (bytes_read == -1)
                return bytes_read;
            if (bytes_read == 0)
                return total_read + buf_total_read;
            buf_total_read += bytes_read;
        }

        total_read += buf_total_read;
    }

    return total_read;
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    ssize_t total_written = 0;

    for (int i = 0; i < iovcnt; i++)
    {
        ssize_t buf_total_written = 0;
        while (buf_total_written < iov[i].iov_len)
        {
            ssize_t bytes_written = write(fd, 
                                    (char *)iov[i].iov_base + buf_total_written,
                                    iov[i].iov_len - buf_total_written);
            if (bytes_written == -1)
                return bytes_written;
            buf_total_written += bytes_written;
        }

        total_written += buf_total_written;
    }

    return total_written;
}

int main(int argc, char *argv[])
{
    int iovcnt = 10, fd;
    struct iovec *iov = malloc(sizeof(struct iovec) * iovcnt);
    
    if ((fd = open("file", O_RDWR | O_CREAT, S_IRUSR | 
                    S_IWUSR | S_IRGRP | S_IROTH)) == -1)
        errExit("open");

    for (int i = 0; i < iovcnt; i++)
    {
        size_t buf_size = iovcnt + i * i;
        char *buf = (char *) malloc(buf_size);
        iov[i].iov_base = buf;
        iov[i].iov_len = buf_size;

        for (size_t j = 0; j < buf_size; j++)
            buf[j] = '!' + j; 
    }

    if (writev(fd, iov, iovcnt) == -1)
        errExit("writev");

    for (int i = 0; i < iovcnt; i++)
        free(iov[i].iov_base);

    for (int i = 0; i < iovcnt; i++)
    {
        size_t buf_size = iovcnt + i * i;
        char *buf = (char *) malloc(buf_size);
        iov[i].iov_base = buf;
        iov[i].iov_len = buf_size;
    }

    if (lseek(fd, 0, SEEK_SET) == -1)
        errExit("lseek");

    if (readv(fd, iov, iovcnt) == -1)
        errExit("readv");

    for (int i = 0; i < iovcnt; i++)
        free(iov[i].iov_base);

    return 0;
}