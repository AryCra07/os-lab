#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// 函数用于生成Collatz猜想的数列
void collatz_sequence(int start)
{
    while (start != 1)
    {
        printf("Child: %d\n", start);

        if (start % 2 == 0)
        {
            start = start / 2;
        }
        else
        {
            start = 3 * start + 1;
        }
    }

    printf("Child: 1\n");
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
        printf("The number must be positive!");
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
        // 子进程中生成数列
        collatz_sequence(start);
    }
    else
    {
        // 父进程等待子进程结束
        wait(NULL);
        printf("\nParent: Done.\n");
    }

    return 0;
}
