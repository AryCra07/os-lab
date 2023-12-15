#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

// shared memory size
#define SHM_SIZE 1024

// generate collatz_sequence and add to shared memory
void collatz_sequence(int start, int *shm_ptr)
{
    int index = 0;

    while (start != 1)
    {
        shm_ptr[index] = start;
        index++;

        if (start % 2 == 0)
        {
            start = start / 2;
        }
        else
        {
            start = 3 * start + 1;
        }
    }

    shm_ptr[index] = 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <start_number>\n", argv[0]);
        return 1;
    }

    int start = atoi(argv[1]);

    if (start <= 0)
    {
        printf("The number must be positive!\n");
        return 1;
    }

    // create shared memory
    int shm_fd = shm_open("/collatz_shm", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);

    // map shared memory to process address space
    int *shm_ptr = (int *)mmap(0, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // generate sequence and write to shm in child
        collatz_sequence(start, shm_ptr);
    }
    else
    {
        // wait for child
        wait(NULL);

        // print
        printf("------sequence from shared memory------\n");
        int index = 0;
        while (shm_ptr[index] != 0)
        {
            printf("%d ", shm_ptr[index]);
            index++;
        }
        printf("\n");
        printf("----------------- END -----------------\n");

        // delete
        munmap(shm_ptr, SHM_SIZE);
        close(shm_fd);
        shm_unlink("/collatz_shm");
    }

    return 0;
}
