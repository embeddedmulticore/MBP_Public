#include <stdio.h>
#include <omp.h>

#define N 10  // 配列サイズ

void parallel_odd_even_sort(float arr[], int n) {
    int i, phase;
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            // 偶数フェーズ：インデックスが偶数のペアを比較
            #pragma omp parallel for private(i) shared(arr)
            for (i = 0; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    float temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                }
            }
        } else {
            // 奇数フェーズ：インデックスが奇数のペアを比較
            #pragma omp parallel for private(i) shared(arr)
            for (i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    float temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                }
            }
        }
    }
}

int main() {
    float arr[N] = {9.2, 3.1, 5.5, 1.0, 4.6, 2.2, 8.8, 7.7, 6.6, 0.5};

    printf("ソート前:\n");
    for (int i = 0; i < N; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    parallel_odd_even_sort(arr, N);

    printf("ソート後:\n");
    for (int i = 0; i < N; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    return 0;
}
