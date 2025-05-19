#!/bin/bash

LOGFILE="benchmark_log.txt"
RESULTS="benchmark_results.txt"

> "$LOGFILE"       # ログファイル初期化
> "$RESULTS"       # 抽出ファイル初期化

RUNS=10
SIZES=(1000 10000 100000 1000000 10000000)

for N in "${SIZES[@]}"; do
    echo "=== データ数: $N を $RUNS 回テスト中 ==="
    for i in $(seq 1 $RUNS); do
        echo "=== [n=$N] テスト $i/$RUNS ===" >> "$LOGFILE"

        # 入力ファイル生成
        echo "$N" | ./input input.txt >> "$LOGFILE" 2>&1

        # qsort 実行（シングルスレッド）
        ./qsort input.txt output_qsort.txt >> "$LOGFILE" 2>&1

        # para_qsort 実行（並列クイックソート：非ハイブリッド）
        ./para_qsort input.txt output_para.txt >> "$LOGFILE" 2>&1

        # hybrid_qsort 実行（並列ハイブリッドクイックソート）
        ./hybrid_qsort input.txt output_hybrid.txt >> "$LOGFILE" 2>&1

        echo "" >> "$LOGFILE"
    done
done

# 「データ」または「時間」を含む行だけを抽出
grep -E "データ|時間" "$LOGFILE" > "$RESULTS"

echo "✅ 実行完了：benchmark_results.txt に結果を保存しました。"
