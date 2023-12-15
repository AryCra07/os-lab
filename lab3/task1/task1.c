#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_THREADS 3
#define MAX_SIZE 1000

int num_cnt = 0;
int numbers[MAX_SIZE];
int max = INT_MIN;
int min = INT_MAX;
int avg = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex

void *calc_avg(void *num_array)
{
    int sum = 0;
    int *numbers = (int *)num_array;
    for (int i = 0; i < num_cnt; ++i)
        sum += numbers[i];
    pthread_mutex_lock(&mutex);
    avg = sum / num_cnt;
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

void *calc_max(void *num_array)
{
    int *numbers = (int *)num_array;
    max = INT_MIN;
    for (int i = 0; i < num_cnt; ++i)
        if (max < numbers[i])
        {
            pthread_mutex_lock(&mutex);
            max = numbers[i];
            pthread_mutex_unlock(&mutex);
        }
    pthread_exit(0);
}

void *calc_min(void *num_array)
{
    int *numbers = (int *)num_array;
    min = INT_MAX;
    for (int i = 0; i < num_cnt; ++i)
        if (min > numbers[i])
        {
            pthread_mutex_lock(&mutex);
            min = numbers[i];
            pthread_mutex_unlock(&mutex);
        }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    num_cnt = argc - 1;
    for (int i = 0; i < num_cnt; ++i)
        numbers[i] = atoi(argv[i + 1]);

    if (argc < 2)
    {
        fprintf(stderr, "Usage: task1 too few arguments!\n");
        return -1;
    }

    pthread_t worker[NUM_THREADS];

    // create 3 threads
    pthread_create(&worker[0], NULL, calc_avg, numbers);
    pthread_create(&worker[1], NULL, calc_max, numbers);
    pthread_create(&worker[2], NULL, calc_min, numbers);

    // wait for all threads done
    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(worker[i], NULL);

    // print
    printf("The average value is %d\n", avg);
    printf("The maximum value is %d\n", min);
    printf("The minimum value is %d\n", max);
    return 0;
}
