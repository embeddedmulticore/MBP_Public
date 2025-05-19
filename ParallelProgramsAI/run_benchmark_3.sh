#!/bin/bash

LOGFILE="benchmark_log.txt"
CSVFILE="benchmark_times.csv"

> "$LOGFILE"
echo "n,qsort,para_qsort,hybrid_qsort" > "$CSVFILE"

RUNS=10
SIZES=(1000 10000 100000 1000000 10000000)

# 実行関数から時間を抽出
extract_time() {
    grep "$1" | grep -oE "[0-9]+\.[0-9]+"
}

for N in "${SIZES[@]}"; do
    echo "=== データ数: $N を $RUNS 回テスト中 ==="

    qsort_times=()
    para_times=()
    hybrid_times=()

    for i in $(seq 1 $RUNS); do
        echo "=== [n=$N] テスト $i/$RUNS ===" >> "$LOGFILE"

        # 入力ファイル作成
        echo "$N" | ./input input.txt >> "$LOGFILE" 2>&1

        # qsort 実行
        qlog=$(./qsort input.txt output_qsort.txt 2>&1)
        echo "$qlog" >> "$LOGFILE"
        qtime=$(echo "$qlog" | extract_time "シングルスレッドクイックソート")
        qsort_times+=("$qtime")

        # para_qsort 実行
        plog=$(./para_qsort input.txt output_para.txt 2>&1)
        echo "$plog" >> "$LOGFILE"
        ptime=$(echo "$plog" | extract_time "並列クイックソート時間")
        para_times+=("$ptime")

        # hybrid_qsort 実行
        hlog=$(./hybrid_qsort input.txt output_hybrid.txt 2>&1)
        echo "$hlog" >> "$LOGFILE"
        htime=$(echo "$hlog" | extract_time "ハイブリッド並列クイックソート")
        hybrid_times+=("$htime")

        echo "" >> "$LOGFILE"
    done

    # 平均計算
    avg() {
        local sum=0
        for t in "${@}"; do
            sum=$(echo "$sum + $t" | bc -l)
        done
        echo "scale=6; $sum / ${#@}" | bc -l
    }

    qavg=$(avg "${qsort_times[@]}")
    pavg=$(avg "${para_times[@]}")
    havg=$(avg "${hybrid_times[@]}")

    echo "$N,$qavg,$pavg,$havg" >> "$CSVFILE"
done

echo "✅ 実行完了：平均時間を benchmark_times.csv に保存しました。"
