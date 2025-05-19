#include <stdio.h>
#include <omp.h>

int main() {
    int sum = 0;
    int i;

    // OpenMPで並列forループ＋reductionでsumを安全に加算
    #pragma omp parallel for reduction(+:sum)
    for (i = 1; i <= 1000; i++) {
        sum += i;
    }

    printf("1から1000までの合計は: %d\n", sum);
    return 0;
}
