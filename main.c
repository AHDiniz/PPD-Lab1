#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "threading.h"
#include "random.h"

#define MIN_COUNT 50
#define MAX_COUNT 50

void MergeSort(int *vec, int start, int end);

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

    return EXIT_SUCCESS;
}

void Merge(int *vec, int *aux, int start, int mid, int end)
{
    if (end <= start || end <= mid || mid <= start)
        return;
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
    if (end <= start)
        return;
    int mid = start + (start + end) / 2;
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
