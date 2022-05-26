/*Scrivere un programma C in cui dato un file A,
una stringa B e un intero N,
vengano creati N thread/processi che cerchino se
all’interno del file A esiste una linea uguale a B*/

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

FILE *fp;
int n_thread = 0;
int result = 0;
char *linea;
pthread_spinlock_t ptspin;

void acquire()
{

    pthread_spin_lock(&ptspin);
}

void release()
{

    pthread_spin_unlock(&ptspin);
}

void *foo(void *parameter)
{

    char lazystring[100];
    char *res;

    do
    {
        pthread_spin_lock(&ptspin);
        res = fgets(lazystring, 100, fp); 
                
        pthread_spin_unlock(&ptspin);
      
        if (strcmp(lazystring, (char *)parameter) == 0) {
            printf("risultato: %s", lazystring);
	    res = NULL;
            }
       
    } while (res != NULL);
}

void generate_threads(int n, char* string)
{

    pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);
    
    n_thread = n;
    pthread_t *array = malloc(n * sizeof(pthread_t));

    for (int i = 0; i < n; i++)
        pthread_create(array + i, NULL, foo, string);

    for (int i = 0; i < n; i++)
        pthread_join(array[i], NULL);

    pthread_spin_destroy(&ptspin);

    free(array);
}

void generate_file(char* name)
{

    fp = fopen(name, "r");

    if (fp < 0)
        abort("msg");
}

int main(int argc, const char *argv[])
{
    if (argc != 4)
        abort("usage: <name file> <n thread> <stringa>");

    generate_file(argv[1]);

    generate_threads(atoi(argv[2]), argv[3]);
}
