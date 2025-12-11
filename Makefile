# コンパイラとフラグの設定
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# ディレクトリ設定
SRC_DIR = src
OUT_DIR = out

# ソースファイルと実行ファイル名
SOURCES = $(SRC_DIR)/main.cc
TARGET = $(OUT_DIR)/pwgen

# デフォルトターゲット（makeと打つだけで実行される）
all: $(TARGET)

# 実行ファイルのビルドルール
$(TARGET): $(SOURCES)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)
	@echo "Build complete: $(TARGET)"

# ビルドして即実行
run: $(TARGET)
	@$(TARGET)

# 生成ファイルの削除
clean:
	rm -rf $(OUT_DIR)
	@echo "Cleaned output directory"

# ビルドし直し（クリーン→ビルド）
rebuild: clean all

# ターゲットがファイル名と衝突しないように宣言
.PHONY: all run clean rebuild
