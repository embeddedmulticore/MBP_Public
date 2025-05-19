#!/bin/bash

# カラーテキスト用
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "要素数 n を入力してください:"
read n

# 入力ファイル名と出力ファイル名
INPUT_FILE="input.txt"
OUTPUT1="output_qsort.txt"
OUTPUT2="output_para.txt"

# 入力ファイル生成
echo ">>> 入力ファイルを生成中..."
./input <<< "$n"
if [ $? -ne 0 ]; then
  echo -e "${RED}input プログラムの実行に失敗しました${NC}"
  exit 1
fi

# シングルスレッドソート実行
echo ">>> qsort を実行中..."
./qsort "$INPUT_FILE" "$OUTPUT1"
if [ $? -ne 0 ]; then
  echo -e "${RED}qsort の実行に失敗しました${NC}"
  exit 1
fi

# 並列ソート実行
echo ">>> para_qsort を実行中..."
./para_qsort "$INPUT_FILE" "$OUTPUT2"
if [ $? -ne 0 ]; then
  echo -e "${RED}para_qsort の実行に失敗しました${NC}"
  exit 1
fi

# 出力結果の比較
echo ">>> 結果を比較中..."
if diff -q "$OUTPUT1" "$OUTPUT2" > /dev/null; then
  echo -e "${GREEN}OK：ソート結果は一致しています${NC}"
else
  echo -e "${RED}NG：ソート結果が一致しません${NC}"
  echo "詳細な差分："
  diff "$OUTPUT1" "$OUTPUT2"
fi
