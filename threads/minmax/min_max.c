#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define abort(msg)   \
    do               \
    {                \
        printf(msg); \
        exit(1);     \
    } while (0)

#define CHUNK 512

short min_s = 0x7fff, max_s = 0xffff;
short *shared_array;
int n_threads = 0;
int fd = -1;

void find_minmax()
{

    short min = shared_array[0];
    short max = shared_array[0];

    for (int i = 0; i < 2 * n_threads; i++)
    {
        if (shared_array[i] > max)
            max = shared_array[i];

        if (shared_array[i] < min)
            min = shared_array[i];
    }

    printf("risultato min %d \n max %d \n ", min, max);
}

void *foo(void *i)
{

    int size = 1;

    short *buffer = malloc(CHUNK);

    short max_finale_thread = max_s;
    short min_finale_thread = min_s;

    while (size)
    {

        size = read(fd, buffer, CHUNK);
        if (size < 0)
            abort("msg \n");

        short max = buffer[0];
        short min = buffer[0];

        for (int i = 0; i < size / sizeof(short); i++)
        {
            if (buffer[i] > max)
                max = buffer[i];

            if (buffer[i] < min)
                min = buffer[i];
        }

        if (max_finale_thread < max)
            max_finale_thread = max;
        if (min_finale_thread > min)
            min_finale_thread = min;
    }

    shared_array[(int)i] = max_finale_thread;
    shared_array[(int)i + n_threads] = min_finale_thread;
}

void generate_threads(int n)
{

    pthread_t *array = malloc(n * sizeof(pthread_t));

    for (int i = 0; i < n; i++)
        pthread_create(array + i, NULL, foo, (void *)i);

    for (int i = 0; i < n; i++)
        pthread_join(array[i], NULL);
}

void generate_file(char *filename, int n_byte)
{

    int *buffer = malloc(n_byte);

    int ff = open("/dev/urandom", O_RDONLY);

    read(ff, buffer, n_byte);

    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0660);

    write(fd, buffer, n_byte);

    lseek(fd, 0, SEEK_SET);
}

int main(int argc, char const *argv[])
{
    if (argc != 4)
        printf("usage: <namefile> <n threads> <n byte>");

    n_threads = atoi(argv[2]);
    shared_array = malloc(2 * n_threads * sizeof(int));
    generate_file(argv[1], atoi(argv[3]));
    generate_threads(n_threads);
    find_minmax();
}