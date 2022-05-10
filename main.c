#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "threading.h"
#include "random.h"

#define MIN_COUNT 10
#define MAX_COUNT 25

typedef struct mergeSortInput
{
    int start, end;
    int *vec;
} MergeSortInput;

typedef struct mergeInput
{
    int start, mid, end;
    int *vec;
} MergeInput;

void ParallelMergeSort(void *in);
void ParallelMerge(void *in);
void MergeSort(int *vec, int start, int end);
void Merge(int *vec, int start, int mid, int end);
int IsSorted(int *vec, int count);

int main(void)
{
    InitRandom();

    time_t timeStart = clock() / CLOCKS_PER_SEC;

    int count = 200000; // RandomRange(MIN_COUNT, MAX_COUNT);
    int *vec = malloc(sizeof(int) * count);
    int threadsNum = 2; // 8; // RandomRange(2, 6);

    Thread *threadIDs = malloc(sizeof(Thread) * threadsNum);
    MergeSortInput *mergeSortInputs = malloc(sizeof(MergeSortInput) * threadsNum);
    MergeInput *mergeInputs = malloc(sizeof(MergeInput) * threadsNum);

    int vecPositionsSize = (threadsNum * 2);
    int *vecPositions = malloc(sizeof(int) * vecPositionsSize);
    vecPositions[0] = 0;

    vecPositions[vecPositionsSize - 1] = count - 1;

    int sizeOfPartition = count / threadsNum;

    for (int i = 1; i < vecPositionsSize - 1; i++)
    {
        vecPositions[i] = i % 2 == 0 ? vecPositions[i - 1] + 1 : vecPositions[i - 1] + (sizeOfPartition - 1);
    }

    for (int i = 0; i < count; ++i)
    {
        vec[i] = RandomRange(0, 50);
    }

    for (int i = 0; i < threadsNum; i++)
    {
        int start = vecPositions[i * 2];
        int end = vecPositions[i * 2 + 1];

        mergeSortInputs[i].vec = vec;
        mergeSortInputs[i].start = start;
        mergeSortInputs[i].end = end;
        ThreadCreate(&(threadIDs[i]), ParallelMergeSort, &(mergeSortInputs[i]));
    }

    for (int i = 0; i < threadsNum; ++i)
        ThreadJoin(&(threadIDs[i]));
    
    for (int i = 0; i < threadsNum; ++i)
        threadIDs[i] = 0;

    int ct = threadsNum;

    while (ct > 1)
    {
        int numGroups = ct / 2 + ct % 2;
        int groupSize = count / numGroups;

        int t = 0;
        for (int i = 0; i < numGroups; i++)
        {
            int start = 0, mid = 0, end = 0;            

            start = (i * groupSize);
            mid = (i * groupSize) + (groupSize / 2) - 1;
            end = (i + 1) * groupSize - 1;

            // int start = vecPositions[(i * 2)];
            // int mid = vecPositions[(i + 1) * 2];
            // int end = vecPositions[(i + 1) * 2 + 1];

            // print pointers
            mergeInputs[i].vec = vec;
            mergeInputs[i].start = start;
            mergeInputs[i].mid = mid;
            mergeInputs[i].end = end;
            // ParallelMerge(&(mergeInputs[i]));
            ThreadCreate(&(threadIDs[t]), ParallelMerge, &(mergeInputs[i]));
            ++t;
            ct--;
        }

        for (int k = 0; k < t; ++k)
            ThreadJoin(&(threadIDs[k]));
    }

    time_t timeEnd = clock() / CLOCKS_PER_SEC;

    printf("Time to run = %ld\n", (timeEnd - timeStart));

    printf("Sorted = %s\n", IsSorted(vec, count) ? "YES" : "NO");

    free(vecPositions);
    free(vec);
    free(threadIDs);
    free(mergeSortInputs);
    free(mergeInputs);

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

void ParallelMerge(void *in)
{
    MergeInput *input = (MergeInput *)in;
    Merge(input->vec, input->start, input->mid, input->end);
}

int IsSorted(int *vec, int count)
{
    for (int i = 1; i < count; ++i)
    {
        if (vec[i] < vec[i - 1])
            return 0;
    }
    return 1;
}
