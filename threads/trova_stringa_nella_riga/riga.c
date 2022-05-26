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

/* quando leggo da file:
esempio: mangia\n\0dorme\n\0piove\n\0cresce\n\0ascolta\n\0legge\n\0
 quando l'utente inserisce il dato si
esempio: mangia\n */

FILE *fd;
// pthread_spinlock_t ptspin;

void *foo(void *par)
{

	int length = 512;
	char *buffer = malloc(length);

	char *res = NULL;

	do
	{
		// pthread_spin_lock(&ptspin);
		res = fgets(buffer, length, fd);
		// pthread_spin_unlock(&ptspin);

		if (strcmp(buffer, (char *)par) == 0)
		{
			printf("stringa: %s, \n", buffer);
			res = NULL;
		}

	} while (res != NULL);

	free(buffer);
}

void file_check(char *file_name, int n_threads)
{

	// pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);

	fd = fopen(file_name, "r");

	pthread_t *array = malloc(n_threads * sizeof(pthread_t));

	int fine = 1;

	do
	{

		printf("inserire stringa da cercare: \n");
		char string[100];
		fgets(string, 100, stdin);
		string[strlen(string) - 1] = '\0';

		fseek(fd, 0, SEEK_SET);

		printf("stringa trovata: \n");
		for (int i = 0; i < n_threads; i++)
			pthread_create(array + i, NULL, foo, (void *)string);

		for (int i = 0; i < n_threads; i++)
			pthread_join(array[i], NULL);
		printf("nessun'altra \n");

		printf("scrivere 1 per continuare, 0 per arrestarsi: \n");
		scanf("%d%*c", &fine);

	} while (fine == 1);

	// pthread_spin_destroy(&ptspin);

	fclose(fd);
	free(array);
}

int main(int argc, const char *argv[])
{

	if (argc != 3)
		abort("usage: <file name> <n threads>");

	file_check(argv[1], atoi(argv[2]));
}
