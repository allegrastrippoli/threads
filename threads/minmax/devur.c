#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define abort(msg)   \
    do               \
    {                \
        printf(msg); \
        exit(1);     \
    } while (0)

#define CHUNK 512

int ff = -1;
short *glob;
int n_threads = 0;
pthread_spinlock_t ptspin;
/* ciascun thread/processo può allocare memoria nell’heap per al più 512 byte ;
è ammesso allocare di variabili globali (data) e locali (stack) per al più 128 byte.
*/

void acquire()
{

    pthread_spin_lock(&ptspin);
}

void release()
{

    pthread_spin_unlock(&ptspin);
}

short find_max_min()
{

    int max = glob[0];
    int min = glob[0];

    for (int i = 0; i < 2 * n_threads; i++)
    {

        if (max < glob[i])
            max = glob[i];

        if (min > glob[i])
            min = glob[i];
    }

    printf("il max e' %d, il min e' %d\n", max, min);

    free(glob);
}

void *foo(void *param)
{

    short *buffer = malloc(CHUNK);

    int size = 0;
    short maxcorrente = 0;
    short mincorrente = 0;
    do
    {
        acquire();
        size = read(ff, buffer, CHUNK);
        release();

        short max = buffer[0];
        short min = buffer[0];
        for (int i = 0; i < size; i++)
        {

            if (max < buffer[i])
            {
                max = buffer[i];
            }

            if (min > buffer[i])
            {
                min = buffer[i];
            }
        }

        /* alla prima iterazione del do, maxcorrente viene assegnato a max
         in questo modo nell'iterazione del do successiva posso tenere traccia
         del risultato dell'iterazione precedente */
        if (maxcorrente == 0)
            maxcorrente = max;

        if (maxcorrente < max)
            maxcorrente = max;

        if (mincorrente == 0)
            mincorrente = min;

        if (mincorrente > min)
            mincorrente = min;

    } while (size > 0);

    glob[(int)param] = mincorrente;
    glob[(int)param + n_threads] = maxcorrente;
}

void generate_threads(int n)
{

    pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);

    pthread_t *array = malloc(n * sizeof(pthread_t));

    for (int i = 0; i < n; i++)
        pthread_create(array + i, NULL, foo, (void *)i);

    for (int i = 0; i < n; i++)
        pthread_join(array[i], NULL);

    pthread_spin_destroy(&ptspin);

    free(array);
}

void generate_file(const char *namefile, int n)
{

    int fd = open("/dev/random", O_RDONLY);
    if (fd < 0)
        abort("errore 1 \n");

    int *buffer = malloc(n);
    int size = read(fd, buffer, n);
    if (size < 0)
        abort("errore 2 \n");

    close(fd);

    ff = open(namefile, O_RDWR | O_CREAT | O_TRUNC, 0660);
    if (ff < 0)
        abort("errore 3\n");

    int res = 0;
    do
    {
        res = write(ff, buffer, n);
        if (res < 0)
            abort("errore 4\n");
    } while (res != size);

    lseek(ff, 0, SEEK_SET);

    free(buffer);
}

int main(int argc, const char *argv[])
{

    if (argc != 4)
        abort("errore 0: usage dvr <nome_file> <num_byte> <num_thread> \n");

    n_threads = atoi(argv[3]);

    glob = malloc(2 * n_threads * sizeof(short));

    generate_file(argv[1], atoi(argv[2]));

    generate_threads(n_threads);

    find_max_min();

    close(ff);
}
