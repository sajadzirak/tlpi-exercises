#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "error.h"

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc < 2)
        usage_error("Usage: tee [OPTION] [FILE]\n");

    int opt, append_mode = 0;
    char *opt_chars = "a";
    while ((opt = getopt(argc, argv, opt_chars)) != -1)
    {
        switch (opt)
        {
            case 'a':
                append_mode = 1;
                break;
            case '?':
                usage_error("Usage: tee [OPTION] [FILE]\n");
        }
    }

    if (optind >= argc)
        usage_error("Usage: tee [OPTION] [FILE]\n");

    char *dst_file = argv[optind];

    int open_flags = O_CREAT | O_WRONLY;
    if (!append_mode) open_flags |= O_TRUNC;
    else open_flags |= O_APPEND;

    int fd;
    if ((fd = open(dst_file, open_flags, 
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
        error_handler("opening file");

    ssize_t bytes_read, bytes_written = 0;
    char input_buf[BUF_SIZE];
    while ((bytes_read = read(STDIN_FILENO, input_buf, BUF_SIZE)) > 0)
    {      
        for(ssize_t total_written = 0; 
            total_written < bytes_read;
            total_written += bytes_written)
        {
            bytes_written = write(STDOUT_FILENO, input_buf + total_written, bytes_read - total_written);
            if (bytes_written == -1)
                error_handler("writing to stdout");
        }

        for(ssize_t total_written = 0;
            total_written < bytes_read;
            total_written += bytes_written)
        {
            bytes_written = write(fd, input_buf + total_written, bytes_read - total_written);
            if (bytes_written == -1)
                error_handler("writing to file");
        }
    }

    if (bytes_read == -1)
        error_handler("reading input");

    if (close(fd) == -1)
        error_handler("closing file");

    return 0;
}