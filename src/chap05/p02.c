#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;
    if ((fd = open("append_test.txt", O_APPEND | O_WRONLY)) == -1)
        errExit("open");
    
    if (lseek(fd, 0, SEEK_SET) == -1)
        errExit("lseek");

    if (write(fd, "let's write some data.", 22) < 22)
        errExit("write");

    if (close(fd) == -1)
        errExit("close");

    return 0;
}