#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 256

int main()
{
    int pipe1[2]; // pipe 1 from parent to child
    int pipe2[2]; // pipe 2 from child to parent

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
        // child
        close(pipe1[1]); // close pipe 1 - write
        close(pipe2[0]); // close pipe 2 - read

        char message[MAX_SIZE];
        read(pipe1[0], message, sizeof(message)); // read from pipe 1

        // flip the capitalization of letters
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

        write(pipe2[1], message, strlen(message) + 1); // write to pipe 2

        close(pipe1[0]);
        close(pipe2[1]);
    }
    else
    {
        // parent
        close(pipe1[0]); // close pipe 1 read
        close(pipe2[1]); // cloase pipe 2 write

        char message[256];
        printf("Enter a message in parent: ");
        fgets(message, sizeof(message), stdin); // get message from user

        write(pipe1[1], message, strlen(message) + 1); // wirte to pipe 1

        read(pipe2[0], message, sizeof(message)); // read processed message from pipe 2

        printf("Received message from child: %s", message);

        close(pipe1[1]);
        close(pipe2[0]);
    }

    return 0;
}
