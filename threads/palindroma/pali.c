/*
Si scriva una funzione di codice C.
Tale funzione deve lanciare un numero di thread/processi pari 
a n per leggere dal file filename una sequenza di stringhe e 
contare quante di queste siano palindrome. Infine, la funzione 
stampa su standard output e ritorna il numero totale di stringhe identificato.*/

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

        /*rimuovo \0 */
        int length = size - 1;
        for (int i = 0; i < length / 2; i++)
        {
            if (buffer[i] != buffer[length - 1 - i])
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
