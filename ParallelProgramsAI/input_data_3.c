#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "使い方: %s <出力ファイル名>\n", argv[0]);
        return 1;
    }

    int n;
    printf("生成する要素数を入力してください: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "正の整数を入力してください。\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "w");
    if (!fp) {
        perror("ファイルを開けませんでした");
        return 1;
    }

    srand48((long)time(NULL)); // 高品質な乱数初期化

    // 1行目に要素数
    fprintf(fp, "%d\n", n);

    // 2行目に n 個の float 値（0〜100.0）
    for (int i = 0; i < n; i++) {
        float val = (float)(drand48() * 100.0);
        fprintf(fp, "%.2f", val);
        if (i != n - 1) fprintf(fp, " ");
    }
    fprintf(fp, "\n");

    fclose(fp);
    printf("ファイル '%s' に %d 個のデータを出力しました。\n", argv[1], n);
    return 0;
}
