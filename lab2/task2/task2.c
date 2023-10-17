/*
1 4         8 7 6
2 5         5 4 3
3 6
C[i][j] = A[i][...] * B[...][j]
*/
#include <stdio.h>
#include <pthread.h>

#define MAXN 10

int A[MAXN][MAXN];
int B[MAXN][MAXN];
int C[MAXN][MAXN];
int A_ROW, A_COL, B_ROW, B_COL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct Node
{
    int row;
    int col;
};

void *Calc(void *node)
{
    int res = 0;
    int r = ((struct Node *)node)->row;
    int c = ((struct Node *)node)->col;

    for (int i = 0; i < A_COL; ++i)
    {
        res += A[r][i] * B[i][c];
    }
    pthread_mutex_lock(&mutex);
    C[r][c] = res;
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
    pthread_t worker[MAXN][MAXN];
    struct Node mat_node[MAXN][MAXN];

    printf("Enter the number of rows and columns in the matrix A:\n");
    scanf("%d %d", &A_ROW, &A_COL);
    printf("Enter values of A:\n");
    for (int i = 0; i < A_ROW; i++)
    {
        for (int j = 0; j < A_COL; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Enter the number of rows and columns in the matrix B:\n");
    scanf("%d %d", &B_ROW, &B_COL);
    printf("Enter values of B:\n");
    for (int i = 0; i < B_ROW; i++)
    {
        for (int j = 0; j < B_COL; j++)
        {
            scanf("%d", &B[i][j]);
        }
    }

    if (A_COL != B_ROW || A_ROW > MAXN || A_COL > MAXN || B_ROW > MAXN || B_COL > MAXN)
    {
        fprintf(stderr, "Usage: task2 invalid input!\n");
        return -1;
    }

    for (int i = 0; i < A_ROW; ++i)
    {
        for (int j = 0; j < B_COL; ++j)
        {
            mat_node[i][j].row = i;
            mat_node[i][j].col = j;

            int ret = pthread_create(&worker[i][j], NULL, Calc, (void *)&mat_node[i][j]);
            if (ret)
            {
                fprintf(stderr, "Usage: task2 create thread fail!\n");
                return -1;
            }
        }
    }

    printf("Here is the result:\n");
    for (int i = 0; i < A_ROW; ++i)
    {
        for (int j = 0; j < B_COL; ++j)
        {
            pthread_join(worker[i][j], NULL);
        }
    }

    for (int i = 0; i < A_ROW; ++i)
    {
        for (int j = 0; j < B_COL; ++j)
        {
            printf("%-5d", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}