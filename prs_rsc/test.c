#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd1, fd2;

    fd1 = open("file.txt", O_RDONLY);
    fd2 = dup(fd1);

    printf("fd1 = %d\n", fd1);
    printf("fd2 = %d\n", fd2);

    close(fd1);
    close(fd2);

    return 0;
}
