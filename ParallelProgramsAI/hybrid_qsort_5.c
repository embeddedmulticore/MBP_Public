#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define PARALLEL_THRESHOLD 1000
#define INSERTION_THRESHOLD 32

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
    if (high - low <= INSERTION_THRESHOLD) {
        insertion_sort(arr, low, high);
        return;
    }

    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp task shared(arr) if (high - low > PARALLEL_THRESHOLD)
        hybrid_quick_sort(arr, low, pi - 1);

        #pragma omp task shared(arr) if (high - low > PARALLEL_THRESHOLD)
        hybrid_quick_sort(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "使い方: %s <入力ファイル> <出力ファイル>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");
    if (!fin) {
        perror("入力ファイルを開けませんでした");
        return 1;
    }

    int n;
    if (fscanf(fin, "%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "配列サイズの読み込みに失敗しました\n");
        fclose(fin);
        return 1;
    }

    float *arr = malloc(n * sizeof(float));
    if (!arr) {
        fprintf(stderr, "メモリ確保に失敗しました\n");
        fclose(fin);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(fin, "%f", &arr[i]) != 1) {
            fprintf(stderr, "データの読み込みに失敗しました（%d番目）\n", i);
            free(arr);
            fclose(fin);
            return 1;
        }
    }
    fclose(fin);

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        hybrid_quick_sort(arr, 0, n - 1);
    }

    double end = omp_get_wtime();
    printf("ハイブリッド並列クイックソート時間: %.6f 秒\n", end - start);

    FILE *fout = fopen(argv[2], "w");
    if (!fout) {
        perror("出力ファイルを開けませんでした");
        free(arr);
        return 1;
    }

    fprintf(fout, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(fout, "%.2f", arr[i]);
        if (i != n - 1) fprintf(fout, " ");
    }
    fprintf(fout, "\n");

    fclose(fout);
    free(arr);

    printf("para_qsort（ハイブリッド）：'%s' → '%s' に出力しました。\n", argv[1], argv[2]);
    return 0;
}
