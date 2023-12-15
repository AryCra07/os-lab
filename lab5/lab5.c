#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FRAME_SIZE 6       // 帧数
#define PAGE_SIZE 10       // 页大小
#define SEQUENCE_COUNT 100 // 指令序列长度

int frames[FRAME_SIZE];                  // 模拟内存帧
int frameUsage[FRAME_SIZE];              // 记录帧使用次数
int instructionSequence[SEQUENCE_COUNT]; // 存储指令序列
int pageSequence[SEQUENCE_COUNT];        // 存储页面序列

// 生成随机指令序列
void generateInstructionSequence()
{
    srand(time(NULL)); // seed
    for (int i = 0; i < SEQUENCE_COUNT; i++)
    {
        int random = rand() % 100;
        if (random < 70)
        {
            // 70% 顺序
            instructionSequence[i] = i;
        }
        else if (random < 80)
        {
            // 10% 前地址
            instructionSequence[i] = (i > 0) ? rand() % i : 0;
        }
        else
        {
            // 20% 后地址
            instructionSequence[i] = i + rand() % (SEQUENCE_COUNT - i);
        }
    }
}

// 生成页面序号序列
void generatePageSequence()
{
    for (int i = 0; i < SEQUENCE_COUNT; i++)
    {
        pageSequence[i] = instructionSequence[i] / PAGE_SIZE;
    }
}

// 检查是否命中，未命中返回-1
int findPageInMemory(int pageNumber)
{
    for (int i = 0; i < FRAME_SIZE; i++)
    {
        if (frames[i] == pageNumber)
        {
            return i;
        }
    }
    return -1;
}

// 最优页面置换
int optimalPageReplacement(int *frames, int currentIndex, int *pageSequence)
{
    int farthestIndex = -1, farthestDistance = -1;

    for (int i = 0; i < FRAME_SIZE; i++)
    {
        int nextPageIndex = currentIndex + 1;
        int j;
        for (j = nextPageIndex; j < SEQUENCE_COUNT; j++)
        {
            if (frames[i] == pageSequence[j])
            {
                if (j > farthestDistance)
                {
                    farthestDistance = j;
                    farthestIndex = i;
                }
                break;
            }
        }
        if (j == SEQUENCE_COUNT)
            return i;
    }
    return farthestIndex;
}

// LRU
int lruPageReplacement(int *frames, int currentIndex, int *pageSequence)
{
    int pageIndexToReplace = 0;

    int max_count = -1;

    for (int i = 0; i < FRAME_SIZE; i++)
    {
        if (max_count < frameUsage[i])
        {
            max_count = frameUsage[i];
            pageIndexToReplace = i;
        }
    }

    for (int i = 0; i < FRAME_SIZE; i++)
    {
        frameUsage[i]++;
    }

    return pageIndexToReplace;
}

// FIFO
int fifoPageReplacement(int *frames, int currentIndex, int *pageSequence)
{
    static int fifoIndex = 0;

    int pageIndexToReplace = fifoIndex;
    fifoIndex = (fifoIndex + 1) % FRAME_SIZE;

    return pageIndexToReplace;
}

// 主模拟程序
void simulatePageReplacement(int (*pageReplacementAlgorithm)(int *, int, int *))
{
    int pageFaults = 0;

    // 初始化
    memset(frames, -1, sizeof(frames));
    memset(frameUsage, 0, sizeof(frameUsage));

    for (int i = 0; i < SEQUENCE_COUNT; i++)
    {
        int pageNumber = pageSequence[i];

        // 检查页面是否已经在帧内
        int pageInMemoryIndex = findPageInMemory(pageNumber);

        if (pageInMemoryIndex == -1)
        {
            // 页错误
            pageFaults++;

            // 利用算法寻找替换帧
            int pageIndexToReplace = pageReplacementAlgorithm(frames, i, pageSequence);

            // 替换，并将未使用次数清零
            frames[pageIndexToReplace] = pageNumber;
            frameUsage[pageIndexToReplace] = 0;
        }
        else
        {
            // 将找到的页面，未使用次数清零
            frameUsage[pageInMemoryIndex] = 0;
        }
    }

    // 计算
    printf("Page Faults: %d\n", pageFaults);
    printf("Page Fault Rate: %.2f%%\n", (float)pageFaults / SEQUENCE_COUNT * 100);
}

int main()
{
    generateInstructionSequence();
    generatePageSequence();

    printf("Optimal Page Replacement Algorithm:\n");
    simulatePageReplacement(optimalPageReplacement);

    printf("\nLeast Recently Used Page Replacement Algorithm:\n");
    simulatePageReplacement(lruPageReplacement);

    printf("\nFirst-In-First-Out Page Replacement Algorithm:\n");
    simulatePageReplacement(fifoPageReplacement);

    return 0;
}
