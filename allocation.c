#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>

#define ALIGNMENT_BLOCK_SIZE 512

#define A (24 * 1048576)
void *B = (void *)0x4DB4BADD;
#define E (152 * 1048576)
#define G 99
#define I 48
#define D 37
#define files_number I
#define true 1
#define false 0
char *ptr;
sem_t semaphore;

void *fill_area(void *data)
{
    int i = *(int *)data;

    int part_size = A / D;
    char *part_start = ptr + part_size * i;
    char *part_end = (i == D) ? (ptr + A) : (ptr + part_size * (i + 1));
    char buffer[G];
    int urandom;
    int j = G;
    for (char *address = part_start; address < part_end; address++)
    {
        if (j == G)
        {
            urandom = open("/dev/urandom", O_RDONLY);
            read(urandom, buffer, G);
            close(urandom);
            j = 0;
        }
        *address = buffer[j++];
    }
}

int main()
{
    printf("Preallocation phase");
    getchar();
    ptr = malloc(A * sizeof(int));
    printf("Postallocation phase");
    getchar();
    pthread_t fill_threads[D];
    int fill_thread_numbers[D];
    for (int i = 0; i < D; i++)
    {
        fill_thread_numbers[i] = i;
        pthread_create(&fill_threads[i], NULL, fill_area, (void *)&fill_thread_numbers[i]);
    }

    for (int i = 0; i < D; i++) {
        pthread_join(fill_threads[i], NULL);
    }

    printf("Predeallocation phase");
    getchar();

    // for (char* start = ptr; start < ptr + A; start++) {
    //     free(start);
    // }

    realloc(ptr, A);

    printf("Postdeallocation phase");
    getchar();

    return 0;
}