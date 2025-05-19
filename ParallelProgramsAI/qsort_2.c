#include <stdio.h>
#include <stdlib.h>

// 要素を交換
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

// パーティション処理（クイックソート用）
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
    if (argc != 3) {
        fprintf(stderr, "使い方: %s <入力ファイル> <出力ファイル>\n", argv[0]);
        return 1;
    }

    // 入力ファイル読み込み
    FILE *fin = fopen(argv[1], "r");
    if (!fin) {
        perror("入力ファイルを開けませんでした");
        return 1;
    }

    int n;
    fscanf(fin, "%d", &n);
    if (n <= 0) {
        fprintf(stderr, "不正な配列サイズです\n");
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
            fprintf(stderr, "データ読み込みに失敗しました\n");
            free(arr);
            fclose(fin);
            return 1;
        }
    }
    fclose(fin);

    // ソート実行
    quick_sort(arr, 0, n - 1);

    // 出力ファイル書き出し
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

    printf("ソート結果を '%s' に出力しました。\n", argv[2]);
    return 0;
}
