#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "threading.h"
#include "random.h"

#define MIN_COUNT 10
#define MAX_COUNT 25

void MergeSort(int *vec, int start, int end);

int main(void)
{
    InitRandom();

    int count = RandomRange(MIN_COUNT, MAX_COUNT);
    int *vec = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
    {
        // vec[i] = RandomRange(INT_MIN / 100, INT_MAX / 100);
        vec[i] = RandomRange(0, 50);
    }

    MergeSort(vec, 0, count - 1);

    for (int i = 0; i < count; ++i)
        printf("%d: %d\n", i, vec[i]);

    free(vec);

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
