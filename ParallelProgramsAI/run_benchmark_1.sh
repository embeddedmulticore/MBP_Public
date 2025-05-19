#!/bin/bash

# 記録用ログファイル
LOGFILE="benchmark_log.txt"
> "$LOGFILE"  # 初期化

# 実行回数とテストサイズ
RUNS=10
SIZES=(1000 10000 100000 1000000 10000000)

for N in "${SIZES[@]}"; do
    echo "=== データ数: $N を $RUNS 回テスト中 ==="
    for i in $(seq 1 $RUNS); do
        echo "=== テスト $i/$RUNS（n=$N）===" >> "$LOGFILE"
        
        # 入力ファイル生成
        echo "$N" | ./input input.txt >> "$LOGFILE" 2>&1

        # qsort 実行
        ./qsort input.txt output_qsort.txt >> "$LOGFILE" 2>&1

        # para_qsort 実行
        ./para_qsort input.txt output_para.txt >> "$LOGFILE" 2>&1

        echo "" >> "$LOGFILE"
    done
done

# 「データ」または「時間」を含む行のみ抽出
grep -E "データ|時間" "$LOGFILE" > benchmark_results.txt

echo "実行完了：benchmark_results.txt に結果を保存しました。"
