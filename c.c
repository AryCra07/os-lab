#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 256

int main()
{
    int pipe1[2]; // 管道1，从父进程到子进程
    int pipe2[2]; // 管道2，从子进程到父进程

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("Pipe creation failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        // 子进程
        close(pipe1[1]); // 关闭管道1写入端
        close(pipe2[0]); // 关闭管道2读取端

        char message[MAX_SIZE];
        read(pipe1[0], message, sizeof(message)); // 从管道1中读取消息

        // 将消息中的字母大小写进行翻转
        for (int i = 0; message[i] != '\0'; i++)
        {
            if (islower(message[i]))
            {
                message[i] = toupper(message[i]);
            }
            else if (isupper(message[i]))
            {
                message[i] = tolower(message[i]);
            }
        }

        write(pipe2[1], message, strlen(message) + 1); // 将翻转后的消息写入管道2

        close(pipe1[0]);
        close(pipe2[1]);
    }
    else
    {
        // 父进程
        close(pipe1[0]); // 关闭管道1读取端
        close(pipe2[1]); // 关闭管道2写入端

        char message[256];
        printf("Enter a message in parent: ");
        fgets(message, sizeof(message), stdin); // 从用户输入中获取消息

        write(pipe1[1], message, strlen(message) + 1); // 将消息写入管道1

        read(pipe2[0], message, sizeof(message)); // 从管道2中读取翻转后的消息

        printf("Received message from child: %s", message);

        close(pipe1[1]);
        close(pipe2[0]);
    }

    return 0;
}
