#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

void safe_write(int fd, const char *buf, size_t n)
{
    for (ssize_t total_written = 0, bytes_written; 
            total_written < n;
            total_written += bytes_written)
    {
        if ((bytes_written = write(fd, buf + total_written, n - total_written)) == -1)
            errExit("write");
    }
}

#define BUF_SIZE 2048

int main(int argc, char *argv[])
{
    if (argc != 3)
        usageErr("%s [SRC] [DST]\n", argv[0]);

    int src_fd, dst_fd;
    if ((src_fd = open(argv[1], O_RDONLY)) == -1)
        errExit("open");
    if ((dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
        errExit("open");

    char buf[BUF_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buf, BUF_SIZE)) > 0)
    {
        size_t i, j;
        for (i = 0, j = 0; i < bytes_read; i++)
        {
            if (buf[i] == '\0')
            {
                if (i > j)
                    safe_write(dst_fd, buf + j, i - j);
                for (j = i + 1; j < bytes_read && buf[j] == '\0'; j++);
                if (lseek(dst_fd, j - i, SEEK_CUR) == -1)
                    errExit("lseek");
                i = j - 1;
            }
        }

        if (j < bytes_read)
            safe_write(dst_fd, buf + j, bytes_read - j);
    }

    if (bytes_read == -1)
        errExit("read");

    if (close(src_fd) == -1)
        errExit("close");
    if (close(dst_fd) == -1)
        errExit("close");

    return 0;
}