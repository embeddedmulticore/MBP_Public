import pandas as pd
import matplotlib.pyplot as plt

# CSV読み込み
df = pd.read_csv("benchmark_times.csv")

# データ
n_values = df["n"]
qsort = df["qsort"]
para_qsort = df["para_qsort"]
hybrid_qsort = df["hybrid_qsort"]

# グラフ描画
plt.figure(figsize=(10, 6))
plt.plot(n_values, qsort, marker='o', label='qsort (single-thread)')
plt.plot(n_values, para_qsort, marker='s', label='para_qsort')
plt.plot(n_values, hybrid_qsort, marker='^', label='hybrid_qsort')

plt.xlabel("Number of elements (n)")
plt.ylabel("Average time (seconds)")
plt.title("Sorting Performance Comparison (log scale Y-axis)")
plt.yscale('log')  # ← 縦軸を対数スケールに変更
plt.legend()
plt.grid(True, which="both", linestyle='--', linewidth=0.5)

plt.tight_layout()
plt.savefig("benchmark_plot.png")
plt.show()
