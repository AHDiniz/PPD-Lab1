#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "threading.h"
#include "random.h"

#define MIN_COUNT 10
#define MAX_COUNT 25

typedef struct mergeSortInput
{
    int start, end;
    int *vec;
} MergeSortInput;

void ParallelMergeSort(void *in);
void MergeSort(int *vec, int start, int end);
void Merge(int *vec, int start, int mid, int end);

int main(void)
{
    printf("Number of cores: %d\n", GetCoreCount());

    InitRandom();

    int count = 24; // RandomRange(MIN_COUNT, MAX_COUNT);
    int *vec = malloc(sizeof(int) * count);
    int threadsNum = GetCoreCount(); // 8; // RandomRange(2, 6);

    Thread *threadIDs = malloc(sizeof(Thread) * threadsNum);

    int vecPositionsSize = (threadsNum * 2);
    int *vecPositions = malloc(sizeof(int) * vecPositionsSize);
    vecPositions[0] = 0;

    vecPositions[vecPositionsSize - 1] = count - 1;

    int sizeOfPartition = count / threadsNum;

    for (int i = 1; i < vecPositionsSize - 1; i++)
    {
        vecPositions[i] = i % 2 == 0 ? vecPositions[i - 1] + 1 : vecPositions[i - 1] + (sizeOfPartition - 1);
    }

    // print vecPositions with name and index
    for (int i = 0; i < vecPositionsSize; i++)
    {
        printf("vecPositions[%d] = %d\n", i, vecPositions[i]);
    }

    for (int i = 0; i < count; ++i)
    {
        vec[i] = RandomRange(0, 50);
    }

    for (int i = 0; i < threadsNum; i++)
    {
        int start = vecPositions[i * 2];
        int end = vecPositions[i * 2 + 1];

        printf("Thread %d: start = %d, end = %d\n", i, start, end);
        // Call MergeSort on each thread
        // MergeSort(vec, start, end);
        MergeSortInput in;
        in.vec = vec;
        in.start = start;
        in.end = end;
        ThreadCreate(&(threadIDs[i]), ParallelMergeSort, &in);
    }

    for (int i = 0; i < threadsNum; ++i)
        ThreadJoin(&(threadIDs[i]));

    // TODO: Startar as threads recursivamente
    // merge all threads
    for (int i = 0; i < threadsNum; i += 2)
    {
        int start = vecPositions[i * 2];
        int mid = vecPositions[(i + 1) * 2];
        int end = vecPositions[(i + 1) * 2 + 1];

        // print pointers
        printf("Merging: start = %d, mid = %d, end = %d\n", start, mid, end);

        Merge(vec, start, start + (end - start) / 2, end);
    }

    int start = 0;
    int mid = (count - 1) / 2;
    int end = count - 1;

    printf("Merging: start = %d, mid = %d, end = %d\n", start, mid, end);
    Merge(vec, start, mid, end);

    // print vec with name and index
    for (int i = 0; i < count; i++)
    {
        printf("vec[%d] = %d\n", i, vec[i]);
    }

    free(vecPositions);
    free(vec);
    free(threadIDs);

    return EXIT_SUCCESS;
}

void Merge(int *vec, int start, int mid, int end)
{
    int start2 = mid + 1;
    // if the first half is smaller than the second half, return
    if (vec[mid] <= vec[start2])
        return;

    // while start <= mid and start2 <= end, loop
    while (start <= mid && start2 <= end)
    {
        // if element at start is smaller than element at start2, increment start
        if (vec[start] <= vec[start2])
        {
            start++;
        }
        else
        {
            int value = vec[start2];
            int index = start2;

            // shift all elements from start2 to end to the left
            while (index != start)
            {
                vec[index] = vec[index - 1];
                index--;
            }

            vec[start] = value;

            // update all pointers
            start++;
            mid++;
            start2++;
        }
    }
}

void MergeSort(int *vec, int start, int end)
{
    if (end <= start)
        return;
    int mid = start + (end - start) / 2;
    MergeSort(vec, start, mid);
    MergeSort(vec, mid + 1, end);
    Merge(vec, start, mid, end);
}

void ParallelMergeSort(void *in)
{
    MergeSortInput *input = (MergeSortInput *)in;
    MergeSort(input->vec, input->start, input->end);
}
