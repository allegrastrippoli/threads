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

int fd = -1;
int count_globale = 0;

int main(int argc, const char *argv[])
{

    fd = open("b.txt", O_RDONLY);

    int chunk = 10;
    char *buffer = malloc(chunk);

    int size = 1;
    int count_locale = 0;

    while (size != 0)
    {
        int check = 1;

        size = read(fd, buffer, chunk);
        if (size < 0)
            abort("msg");

        printf("ultimo carattere: %c \n", buffer[strlen(buffer) - 1]);
        buffer[strlen(buffer) - 1] = '\0';
        int length = strlen(buffer);
        printf("expected: 9 got: %d \n", length);

        for (int i = 0; i < length / 2; i++)
        {
            printf("buffer[%d] = %c, buffer[%d] = %c \n", i, buffer[i], length - 1 - i, buffer[length - 1 - i]);
            if (buffer[i] != buffer[length - 1 - i])
            {
                check = 0;
                break;
            }
        }

        if (check == 1)
        {
            printf("ci sono \n");
            count_locale++;
        }
    }

    count_globale += count_locale;

    printf("numero totale: %d \n", count_globale);

    close(fd);
    free(buffer);
}
