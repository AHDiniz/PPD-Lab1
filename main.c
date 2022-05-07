#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "threading.h"

void ParallelMergeSort(int *vec, int count, int threads);
void MergeSort(int *vec, int start, int end);
void Merge(int *vec, int start, int mid, int end);

int main(void)
{
    

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
