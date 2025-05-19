#include <stdio.h>
#include <stdlib.h>

// 要素を交換する関数
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
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

// クイックソート（再帰）
void quick_sort(float arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
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
    if (n <= 0) {
        fprintf(stderr, "不正な配列サイズです。\n");
        fclose(fp);
        return 1;
    }

    float *arr = malloc(n * sizeof(float));
    if (!arr) {
        fprintf(stderr, "メモリ確保に失敗しました\n");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(fp, "%f", &arr[i]) != 1) {
            fprintf(stderr, "データ読み込みに失敗しました\n");
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

    quick_sort(arr, 0, n - 1);

    printf("ソート後（先頭10個）:\n");
    for (int i = 0; i < n && i < 10; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    // 必要に応じて結果をファイルに出力することも可能
    // 例：出力ファイルにソート結果を書き出す処理など

    free(arr);
    return 0;
}
