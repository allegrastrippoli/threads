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

        if (buffer[strlen(buffer) - 1] != EOF)
        {
            //printf("length di 123454321: expected 10 got %d \n", strlen(buffer));
            buffer[strlen(buffer) - 1] = '\0';
            //printf("length di 123454321: expected 9 got %d \n", strlen(buffer));
        }
        int length = strlen(buffer) - 1;
        //printf("length di 123454321: expected 8 got %d \n", length);
        for (int i = 0; buffer[i] != '\0'; i++)
            printf("%c ", buffer[i]);
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            // printf("buffer[%d] = %c, buffer[%d] = %c \n", i, buffer[i], length - i, buffer[length - i]);
            if (buffer[i] != buffer[length - i])
            {
                check = 0;
                break;
            }
        }

        if (check == 1)
            count_locale++;
    }

    count_globale += count_locale;

    printf("numero totale: %d \n", count_globale);

    close(fd);
    free(buffer);
}
