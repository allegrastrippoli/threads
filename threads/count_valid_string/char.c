/* int count_valid_string (char *filename, char *charset, int n).
Tale funzione deve lanciare un numero di thread/processi pari a n
per leggere dal file filename una sequenza di stringhe e, per ciascuna di esse,
contare quante di queste contengono tutti i caratteri contenuti nella stringa charset.
infine, la funzione stampa su standard output e ritorna il numero totale di stringhe identificato.
Il file è tale per cui, ciascuna stringa è lunga 50 caratteri incluso il terminatore di linea.
 */

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

int fd = -1;

int find(char c, char *buffer)
{

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        printf("c = %c,  buffer[%d] = %c \n", c, i, buffer[i]);
        if (c == buffer[i])
        {
            printf("sono qui \n");
            return 1;
        }
    }

    return 0;
}

int count_valid_string(char *filename, char *charset)
{

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        abort("msg");

    char *buffer = malloc(4);

    int size = 1;
    int res = 0;

    while (size)
    {
        int count = 1;

        size = read(fd, buffer, 4);
        if (size < 0)
            abort("msg");

        for (int i = 0; i < charset[i] != '\0'; i++)
        {

            if (find(charset[i], buffer) == 0)
            {
                count = 0;
                break;
            }
            printf("eccomi: %d \n", count);
        }

        if (count == 1)
            res++;
    }

    return res;
}

int main()
{

    int res = count_valid_string("b.txt", "335");

    printf("sono state trovate %d occorrenze di charset \n", res);
}
