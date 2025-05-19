#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define THRESHOLD 1000  // 並列化の閾値（小さい配列には並列を使わない）

// 配列の要素を交換する関数
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

// パーティション分割
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

// 並列クイックソート
void parallel_quick_sort(float arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // 配列サイズが大きいときのみ並列タスクを使う
        #pragma omp task shared(arr) if (high - low > THRESHOLD)
        parallel_quick_sort(arr, low, pi - 1);

        #pragma omp task shared(arr) if (high - low > THRESHOLD)
        parallel_quick_sort(arr, pi + 1, high);
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

    // 並列クイックソート実行
    #pragma omp parallel
    {
        #pragma omp single
        parallel_quick_sort(arr, 0, n - 1);
    }

    printf("ソート後（先頭10個）:\n");
    for (int i = 0; i < 10; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
