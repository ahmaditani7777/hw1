#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_VALUE 10000  

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void bubbleSort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
    }
}
void parallelBucketSort(int *arr, int n, int num_buckets, int num_threads) {
  
    int **buckets = (int **)malloc(num_buckets * sizeof(int *));
    int *bucket_sizes = (int *)malloc(num_buckets * sizeof(int));
    
    for (int i = 0; i < num_buckets; i++) {
        buckets[i] = (int *)malloc(n * sizeof(int));
        bucket_sizes[i] = 0;
    }
    int max_value = MAX_VALUE;
    int bucket_range = (max_value + 1) / num_buckets;
    
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < n; i++) {
        int bucket_index = arr[i] / bucket_range;
        #pragma omp critical 
        {
            buckets[bucket_index][bucket_sizes[bucket_index]++] = arr[i];
        }
    }
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_buckets; i++) {
        if (bucket_sizes[i] > 0) {
            bubbleSort(buckets[i], bucket_sizes[i]);
        }
        }
    int index = 0;
    for (int i = 0; i < num_buckets; i++) {
        for (int j = 0; j < bucket_sizes[i]; j++) {
            arr[index++] = buckets[i][j];
        }
    }

    for (int i = 0; i < num_buckets; i++) {
        free(buckets[i]);
    }
    free(buckets);
    free(bucket_sizes);
}

int main() {
    int n = 10000;  
    int num_buckets = 10;  
    int num_threads = 4;  

    int *arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % MAX_VALUE;  
    }

    printf("Unsorted array (first 20 elements):\n");
    for (int i = 0; i < 20; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    parallelBucketSort(arr, n, num_buckets, num_threads);
    printf("Sorted array (first 20 elements):\n");
    for (int i = 0; i < 20; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
}
