#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "threading.h"
#include "random.h"

typedef struct mergeSortInput
{
    int threadId, numbersPerThread, numThreads, offset;
    int *vec;
} MergeSortInput;

typedef struct mergeInput
{
    int start, mid, end;
    int *vec;
} MergeInput;

void merge_threads(int numThreads, int numbersPerThread, int count, int *vec, int *vec_copy);
#define CHECK_TIME(x)                                                                                       \
    {                                                                                                       \
        struct timespec start, end;                                                                         \
        clock_gettime(CLOCK_REALTIME, &start);                                                              \
        x;                                                                                                  \
        clock_gettime(CLOCK_REALTIME, &end);                                                                \
        double f = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec); \
        printf("took [%f] ms\n", f / 1000000);                                                                \
    }

/* merge function */
void merge(int *vec, int left, int middle, int right)
{
    int left_length = middle - left + 1;
    int left_array[left_length];
    int i = 0; // index for left_array
    /* copy values to left array */
    for (int i = 0; i < left_length; i++)
    {
        left_array[i] = vec[left + i];
    }

    int j = 0; // index for right_array
    int right_length = right - middle;
    int right_array[right_length];
    /* copy values to right array */
    for (int j = 0; j < right_length; j++)
    {
        right_array[j] = vec[middle + 1 + j];
    }

    i = 0;
    j = 0;
    int k = 0; // Relative position in the merged array

    /** chose from right and left arrays and copy */
    while (i < left_length && j < right_length)
    {
        if (left_array[i] <= right_array[j])
        {
            vec[left + k] = left_array[i];
            i++;
        }
        else
        {
            vec[left + k] = right_array[j];
            j++;
        }
        k++;
    }

    /* copy the remaining values to the array */
    while (i < left_length)
    {
        vec[left + k] = left_array[i];
        k++;
        i++;
    }
    while (j < right_length)
    {
        vec[left + k] = right_array[j];
        k++;
        j++;
    }
}

/* perform merge sort */
void merge_sort(int *vec, int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;
        merge_sort(vec, left, middle);
        merge_sort(vec, middle + 1, right);
        merge(vec, left, middle, right);
    }
}

void thread_merge(void *arg)
{
    MergeInput *input = (MergeInput *)arg;
    merge(input->vec, input->start, input->mid, input->end);
    free(input);
}

void thread_merge_sort(void *arg)
{
    MergeSortInput *input = (MergeSortInput *)arg;

    int threadId = input->threadId;
    int numbersPerThread = input->numbersPerThread;
    int numThreads = input->numThreads;
    int offset = input->offset;
    int *vec = input->vec;

    int left = threadId * (numbersPerThread);
    int right = (threadId + 1) * (numbersPerThread)-1;
    if (threadId == numThreads - 1)
    {
        right += offset;
    }
    int middle = left + (right - left) / 2;
    if (left < right)
    {
        merge_sort(vec, left, right);
    }

    free(input);
}

/* test to ensure that the array is in sorted order */
void test_array_is_in_order(int vec[], int count)
{
    for (int i = 1; i < count; i++)
    {
        if (vec[i] < vec[i - 1])
        {
            printf("Error. Out of order sequence: %d found\n", vec[i]);
            return;
        }
    }
    printf("Array is in sorted order\n");
}

/* merge locally sorted sections */
void merge_sections_of_array(int arr[], int number, int aggregation, int numbersPerThread, int LENGTH)
{
    Thread *threads = (Thread *)malloc(sizeof(Thread) * number);
    for (int i = 0; i < number; i = i + 2)
    {
        int left = i * (numbersPerThread * aggregation);
        int middle = left + (numbersPerThread * aggregation) - 1;
        int right = ((i + 2) * numbersPerThread * aggregation) - 1;
        if (right >= LENGTH)
        {
            right = LENGTH - 1;
        }
        MergeInput *input = (MergeInput *)malloc(sizeof(MergeInput));
        input->start = left;
        input->mid = middle;
        input->end = right;
        input->vec = arr;
        ThreadCreate(&threads[i], thread_merge, input);
    }

    for (int i = 0; i < number; i = i + 2)
    {
        ThreadJoin(&threads[i]);
    }
    free(threads);
    if (number / 2 >= 1)
    {
        merge_sections_of_array(arr, number / 2, aggregation * 2, numbersPerThread, LENGTH);
    }
}

int main(void)
{
    InitRandom();

    // int countArray[5] = {100, 1000, 10000, 100000, 500000};
    int countArray[5] = {10000, 500000, 1000000, 1500000, 2000000};

    // Execute for different number of elements
    for (int c = 0; c < 5; c++)
    {
        int count = countArray[c];

        int *vec = malloc(sizeof(int) * count);
        for (int i = 0; i < count; ++i)
        {
            vec[i] = RandomRange(0, INT_MAX);
        }
        int *vec_copy = malloc(sizeof(int) * count);

        int threadsNum[5] = {1, 2, 4, 8, 16};

        // Execute for different number of threads
        for (int i = 0; i < 5; i++)
        {
            int numThreads = threadsNum[i];
            int numbersPerThread = count / numThreads;
            int offset = count % numThreads;

            printf("Sorting for [%d] numbers with [%d] threads ", count, numThreads);
            CHECK_TIME(merge_threads(numThreads, numbersPerThread, count, vec, vec_copy));
            test_array_is_in_order(vec_copy, count);
        }

        free(vec);
        free(vec_copy);
    }

    return EXIT_SUCCESS;
}

void merge_threads(int numThreads, int numbersPerThread, int count, int *vec, int *vec_copy)
{
    int offset = count % numThreads;
    Thread *threads = malloc(sizeof(Thread) * numThreads);

    // memcpy vec to vec_copy
    memcpy(vec_copy, vec, sizeof(int) * count);

    /* create threads */
    for (int i = 0; i < numThreads; i++)
    {
        MergeSortInput *input = malloc(sizeof(MergeSortInput));
        input->vec = vec_copy;
        input->threadId = i;
        input->numbersPerThread = numbersPerThread;
        input->numThreads = numThreads;
        input->offset = offset;

        ThreadCreate(&threads[i], thread_merge_sort, input);
    }
    for (long i = 0; i < numThreads; i++)
    {
        ThreadJoin(&threads[i]);
    }

    merge_sections_of_array(vec_copy, numThreads, 1, numbersPerThread, count);
    free(threads);
}