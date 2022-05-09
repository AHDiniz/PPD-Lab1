#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "threading.h"
#include "random.h"

#define MIN_COUNT 500
#define MAX_COUNT 1000

#ifndef _WIN32
#define INT_MIN -2147483647 - 1
#define INT_MAX  2147483647
#endif

void MergeSort(int *vec, int start, int end);

int main(void)
{
    printf("1\n");

    InitRandom();

    int count = RandomRange(MIN_COUNT, MAX_COUNT);
    int *vec = malloc(sizeof(int) * count);

    printf("2\n");

    for (int i = 0; i < count; ++i)
    {
        vec[i] = RandomRange(INT_MIN / 100, INT_MAX / 100);
    }

    printf("3\n");
    
    MergeSort(vec, 0, count);

    free(vec);

    printf("4\n");

    return EXIT_SUCCESS;
}

void Merge(int *vec, int *aux, int start, int mid, int end)
{
    int i, j;
    for (i = mid + 1; i > start; --i)
        aux[i - 1] = vec[i - 1];
    for (j = mid; j < end; ++j)
        aux[end + mid - j] = vec[j + 1];
    for (int k = start; k <= end; ++k)
        vec[k] = aux[j] < aux[i] ? aux[j--] : aux[i++];
}

void MergeSortAux(int *vec, int *aux, int start, int end)
{
    if (end < start)
        return;
    int mid = (start + end) / 2;
    MergeSortAux(vec, aux, start, mid);
    MergeSortAux(vec, aux, mid + 1, end);
    Merge(vec, aux, start, mid, end);
}

void MergeSort(int *vec, int start, int end)
{
    int *aux = malloc(sizeof(int) * (end - start));
    MergeSortAux(vec, aux, start, end);
    free(aux);
}
