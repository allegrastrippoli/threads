#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
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

#define CHUNK 10

int fd = -1;
int global_count = 0;
// pthread_spinlock_t ptspin;

/*void acquire() {

    pthread_spin_lock(&ptspin);
}

void release() {

    pthread_spin_unlock(&ptspin);
}*/

void remove_you(char c, char *string)
{

    if (strlen(string) == 1)
        string[0] = '\0';

    else
    {

        for (int i = 0; string[i] != '\0'; i++)
        {
            if (string[i] == c)
            {
                for (int j = i; string[j] != '\0'; j++)
                {
                    printf("string[%d] = %c, string[%d] = %c \n", j, string[j], j + 1, string[j + 1]);
                    string[j] = string[j + 1];
                    printf("result: %s \n", string);
                }
                break;
            }
        }
    }
}

void *foo(void *par)
{

    char *buffer = malloc(CHUNK);
    char *keeper = (char *)par;
    char *string = malloc(strlen(keeper));

    int size = 1;
    int count = 0;

    while (size)
    {
        for (int i = 0; keeper[i] != '\0'; i++)
            string[i] = keeper[i];

        size = read(fd, buffer, CHUNK);
        if (size < 0)
            abort("mistake \n");

        for (int i = 0; buffer[i] != '\0'; i++)
        {

            for (int j = 0; string[j] != '\0'; j++)
            {
                // printf("buffer[%d] = %c, string[%d]=%c\n", i, buffer[i], j, string[j]);
                if (buffer[i] == string[j])
                    remove_you(string[j], string);
            }
        }

        if (string[0] == '\0')
        {
            count++;
            printf("count: %d \n", count);
        }
    }

    // acquire();
    global_count += count;
    // release();

    free(buffer);
}

void open_file(char *namefile, char *charset, int n)
{

    // pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);

    fd = open(namefile, O_RDONLY);

    pthread_t *array = malloc(n * sizeof(pthread_t));

    for (int i = 0; i < n; i++)
        pthread_create(array + i, NULL, foo, (void *)charset);

    for (int i = 0; i < n; i++)
        pthread_join(array[i], NULL);

    // pthread_spin_destroy(&ptspin);

    close(fd);
}

int main(int argc, char *argv[])
{

    char charset[] = "123";
    open_file("b.txt", charset, 1);
    printf("%d \n", global_count);
}
