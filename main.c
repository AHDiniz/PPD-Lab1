#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "threading.h"
#include "random.h"

#define MIN_COUNT 1000
#define MAX_COUNT 999999

void ParallelMergeSort(int *vec, int count, int threads);
void MergeSort(int *vec, int start, int end);
void Merge(int *vec, int start, int mid, int end);

int count = 0;

void TestFunction(void *i)
{
    int t = *((int *)i);
    count += t;
}

int main(void)
{
    InitRandom();

    int count = RandomRange(MIN_COUNT, MAX_COUNT);
    int *vec = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
    {
        vec[i] = RandomRange(INT_MIN, INT_MAX);
    }
    
    MergeSort(vec, 0, count - 1);

    free(vec);

    Thread threads[4];

    for (int i = 0; i < 4; ++i)
    {
        ThreadCreate(&(threads[i]), TestFunction, NULL);
    }

    printf("Count = %d\n", count);

    return EXIT_SUCCESS;
}

void ParallelMergeSort(int *vec, int count, int threads)
{

}

void MergeSort(int *vec, int start, int end)
{
    if (end <= start) return;
    int mid = (end + start) / 2;
    MergeSort(vec, start, mid);
    MergeSort(vec, mid + 1, end);
    Merge(vec, start, mid, end);
}

void OutMerge(int *vec, int *a, int aCount, int *b, int bCount)
{
    for (int i = 0, j = 0, k = 0; k < aCount + bCount; ++k)
    {
        if (i == aCount)
        {
            vec[k] = b[j++];
            continue;
        }
        if (j == bCount)
        {
            vec[k] = a[i++];
            continue;
        }
        vec[k] = a[i] < b[j] ? a[i++] : b[j++];
    }
}

void Merge(int *vec, int start, int mid, int end)
{
    int *aux = malloc(sizeof(int) * (end - start));
    memcpy(aux, vec + start, sizeof(int) * (end - start));
    OutMerge(aux, vec + start, mid - start, vec + mid + 1, end - mid + 1);
    memcpy(vec + start, aux, sizeof(int) * (end - start));
    free(aux);
}
