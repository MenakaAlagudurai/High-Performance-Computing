#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 10

int a[SIZE][SIZE], b[SIZE][SIZE], sum[SIZE][SIZE], prod[SIZE][SIZE];
double time_taken1, time_taken2;
struct timespec start, end;

/* Thread function for matrix addition */
void* add_matrix(void* arg)
{
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            sum[i][j] = a[i][j] + b[i][j];
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    time_taken1 = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / 1e9;

    return NULL;
}

/* Thread function for matrix multiplication */
void* multiply_matrix(void* arg)
{
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            prod[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
            {
                prod[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    time_taken2 = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / 1e9;

    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    /* Initialize matrices with random values */
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            a[i][j] = rand() % 10;
            b[i][j] = rand() % 10;
        }
    }

    /* Create threads */
    pthread_create(&thread1, NULL, add_matrix, NULL);
    pthread_create(&thread2, NULL, multiply_matrix, NULL);

    /* Wait for threads to finish */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    /* Print Addition Result */
    printf("\nResult of Addition:\n");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            printf("%3d ", sum[i][j]);
        printf("\n");
    }

    /* Print Multiplication Result */
    printf("\nResult of Multiplication:\n");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            printf("%4d ", prod[i][j]);
        printf("\n");
    }

    /* Print Execution Time */
    printf("\n--- Execution Time ---\n");
    printf("Execution Time for Addition: %f seconds\n", time_taken1);
    printf("Execution Time for Multiplication: %f seconds\n", time_taken2);

    return 0;
}
