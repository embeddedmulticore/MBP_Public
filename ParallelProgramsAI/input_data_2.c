#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n;
    char filename[256];

    printf("生成する要素数を入力してください（例: 10000）: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("正の整数を入力してください。\n");
        return 1;
    }

    printf("出力ファイル名を入力してください（例: input.txt）: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("ファイルを開けませんでした");
        return 1;
    }

    // drand48 の初期化（seedはtimeベース）
    srand48((long)time(NULL));

    // 1行目に配列サイズ
    fprintf(fp, "%d\n", n);

    // 2行目に乱数 float 値（0〜100.0）
    for (int i = 0; i < n; i++) {
        float val = (float)(drand48() * 100.0);
        fprintf(fp, "%.2f", val);
        if (i != n - 1) fprintf(fp, " ");
    }
    fprintf(fp, "\n");

    fclose(fp);
    printf("ファイル '%s' に %d 個の乱数データを出力しました。\n", filename, n);

    return 0;
}
