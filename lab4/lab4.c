#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5

sem_t mutex, full, empty;
int buffer[BUFFER_SIZE];
int in = 0, out = 0;

void *producer(void *arg)
{
    while (1)
    {
        // Produce item
        int item = rand() % 100;

        // Wait for an empty slot in the buffer
        sem_wait(&empty);
        sem_wait(&mutex);

        // Add item to the buffer
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;

        // Signal that buffer is no longer empty
        sem_post(&mutex);
        sem_post(&full);

        // Output producer information
        printf("Producer %lu produced: %d\n", pthread_self(), item);

        // Sleep for a random period
        usleep(rand() % 1000000);
    }
}

void *consumer(void *arg)
{
    while (1)
    {
        // Wait for a full slot in the buffer
        sem_wait(&full);
        sem_wait(&mutex);

        // Consume item from the buffer
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        // Signal that buffer is no longer full
        sem_post(&mutex);
        sem_post(&empty);

        // Output consumer information
        printf("Consumer %lu consumed: %d\n", pthread_self(), item);

        // Sleep for a random period
        usleep(rand() % 1000000);
    }
}

int main(int argc, char *argv[])
{
    // Parse command line arguments
    if (argc != 4)
    {
        printf("Usage: %s <sleep duration> <producer threads> <consumer threads>\n", argv[0]);
        return -1;
    }
    int sleep_duration = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);

    // Create producer threads
    pthread_t producer_threads[num_producers];
    for (int i = 0; i < num_producers; i++)
    {
        srand(time(NULL) + i);
        pthread_create(&producer_threads[i], NULL, producer, NULL);
    }

    // Create consumer threads
    pthread_t consumer_threads[num_consumers];
    for (int i = 0; i < num_consumers; i++)
    {
        srand(time(NULL) + i);
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);
    }

    // Sleep for the specified duration
    sleep(sleep_duration);

    // Terminate the program
    return 0;
}