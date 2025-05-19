#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define THRESHOLD 1000    // 並列処理の閾値
#define INSERTION_SORT_LIMIT 32  // 挿入ソートに切り替えるサイズ

// 2つの要素を交換
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

// 挿入ソート（小さな範囲用）
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

// クイックソートのパーティション処理
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

// ハイブリッド並列クイックソート
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

int main() {
    int n = 10000;
    float *arr = malloc(n * sizeof(float));

    // ランダムデータ生成
    for (int i = 0; i < n; i++) {
        arr[i] = (float)rand() / RAND_MAX * 100.0;
    }

    printf("ソート前（先頭10個）:\n");
    for (int i = 0; i < 10; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    // 並列セクション開始
    #pragma omp parallel
    {
        #pragma omp single
        hybrid_quick_sort(arr, 0, n - 1);
    }

    printf("ソート後（先頭10個）:\n");
    for (int i = 0; i < 10; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
