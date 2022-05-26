/* Write a C function with the following interface:

int above_threshold(char *filename, int n, int threshold).

This function must launch a number of threads / processes equal
to n to read a sequence of ints from the binary file filename and
count how many of these have a value greater than threshold.
Finally, the function prints to standard output and returns the
total number of int greater than threshold value contained in the
file.

*/
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FILENAME "dataset.bin"
#define NUM_THREADS 4
#define NUM_THRESHOLDS 3

#define abort(msg)   \
    do               \
    {                \
        printf(msg); \
        exit(1);     \
    } while (0)

int thresholds[NUM_THRESHOLDS] = {0x0, 0x40000000, 0xc0000000};
int fd = -1;
int total = 0;
pthread_spinlock_t ptspin;

void acquire()
{
    pthread_spin_lock(&ptspin);
}

void release()
{
    pthread_spin_unlock(&ptspin);
}

void *foo(void *par)
{
    int chunk = 1024;
    int *buffer = malloc(chunk);

    int size = 1;
    int count = 0;

    while (size > 0)
    {
        acquire();
        size = read(fd, buffer, chunk);
        release();

        if (size < 0)
            abort(" E1 \n");

        for (int i = 0; i < (size / sizeof(int)); i++)
        {
            if ((int)par < buffer[i])
                count++;
        }
    }
    acquire();
    total += count;
    release();

    free(buffer);
}

int above_threshold(char *filename, int n, int threshold)
{

    total = 0;
    pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);

    fd = open(FILENAME, O_RDONLY);

    pthread_t *array = malloc(n * sizeof(pthread_t));

    for (int i = 0; i < n; i++)
        pthread_create(array + i, NULL, foo, (void *)threshold);

    for (int i = 0; i < n; i++)
        pthread_join(array[i], NULL);

    pthread_spin_destroy(&ptspin);

    close(fd);
    free(array);

    return total;
}

int main()
{
    int res;
    int i;
    for (i = 0; i < NUM_THRESHOLDS; i++)
    {
        res = above_threshold(FILENAME, NUM_THREADS, thresholds[i]);
        printf("The correct answer for %d is %d\n", thresholds[i], res);
    }
}
