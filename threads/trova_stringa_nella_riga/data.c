#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define abort(msg)   \
    do               \
    {                \
        printf(msg); \
        exit(1);     \
    } while (0)

int main(int argc, const char *argv[])
{

    int fd = open("a.txt", O_RDWR | O_CREAT | O_TRUNC, 0660);

    // char *riga = "mangia\0\ndorme\0\npiove\0\ncresce\0\nascolta\0\nlegge\0\n";
    char *riga = "mangia\ndorme\npiove\ncresce\nascolta\nlegge\n";
    write(fd, riga, 47);

    close(fd);
}
