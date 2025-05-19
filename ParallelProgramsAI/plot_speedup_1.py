import pandas as pd
import matplotlib.pyplot as plt

# CSV読み込み
df = pd.read_csv("benchmark_times.csv")

# 各データ取得
n_values = df["n"]
qsort_time = df["qsort"]
para_time = df["para_qsort"]
hybrid_time = df["hybrid_qsort"]

# 性能向上倍率（speedup）を計算
speedup_para = qsort_time / para_time
speedup_hybrid = qsort_time / hybrid_time

# グラフ描画
plt.figure(figsize=(10, 6))
plt.plot(n_values, speedup_para, marker='o', label='para_qsort speedup')
plt.plot(n_values, speedup_hybrid, marker='s', label='hybrid_qsort speedup')

plt.xlabel("Number of elements (n)")
plt.ylabel("Speedup (×)")
plt.title("Speedup over qsort (higher is better)")
# plt.xscale("log")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()

plt.tight_layout()
plt.savefig("speedup_plot.png")
plt.show()
