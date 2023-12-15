#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// generate Collatz sequence
void collatz_sequence(int pid, int start)
{
    if (pid == 0)
    {
        printf("Child:");
    }
    while (start != 1)
    {
        printf(" %d", start);

        if (start % 2 == 0)
        {
            start = start / 2;
        }
        else
        {
            start = 3 * start + 1;
        }
    }

    printf(" 1\n");
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

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // generate sequence in child process
        collatz_sequence(pid, start);
    }
    else
    {
        // wait for child
        wait(NULL);
        printf("\nParent: Done.\n");
    }

    return 0;
}
