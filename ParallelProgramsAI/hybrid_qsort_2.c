#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define THRESHOLD 1000
#define INSERTION_SORT_LIMIT 32

void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void insertion_sort(float arr[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        float key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int partition(float arr[], int low, int high) {
    float pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void hybrid_quick_sort(float arr[], int low, int high) {
    if (high - low <= INSERTION_SORT_LIMIT) {
        insertion_sort(arr, low, high);
        return;
    }

    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp task shared(arr) if (high - low > THRESHOLD)
        hybrid_quick_sort(arr, low, pi - 1);

        #pragma omp task shared(arr) if (high - low > THRESHOLD)
        hybrid_quick_sort(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "使い方: %s input.txt\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("ファイルを開けませんでした");
        return 1;
    }

    int n;
    fscanf(fp, "%d", &n);

    float *arr = malloc(n * sizeof(float));
    if (!arr) {
        fprintf(stderr, "メモリ確保に失敗しました\n");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(fp, "%f", &arr[i]) != 1) {
            fprintf(stderr, "データの読み込みに失敗しました\n");
            free(arr);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    printf("ソート前（先頭10個）:\n");
    for (int i = 0; i < n && i < 10; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    #pragma omp parallel
    {
        #pragma omp single
        hybrid_quick_sort(arr, 0, n - 1);
    }

    printf("ソート後（先頭10個）:\n");
    for (int i = 0; i < n && i < 10; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
