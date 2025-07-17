#include <stdio.h>
#include <omp.h>

int main(void) {
    int n = 1000;
    long long sum = 0;  // 合計を格納する変数（大きめの型にしておく）
    
    // 並列forで加算。reductionでスレッドごとの部分和を合計
    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= n; i++) {
        sum += i;
    }

    printf("1から%dまでの和 = %lld\n", n, sum);
    return 0;
}
