# コンパイラとフラグの設定
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -MMD -MP

# ビルドタイプ（make DEBUG=1 でデバッグビルド）
ifdef DEBUG
  CXXFLAGS += -g -O0 -DDEBUG
else
  CXXFLAGS += -O2
endif

# ディレクトリ設定
SRC_DIR = src
OUT_DIR = out

# ソースファイルの自動検出
SOURCES = $(wildcard $(SRC_DIR)/*.cc)
OBJECTS = $(patsubst $(SRC_DIR)/%.cc,$(OUT_DIR)/%.o,$(SOURCES))
DEPENDS = $(OBJECTS:.o=.d)

# 実行ファイル名
TARGET = $(OUT_DIR)/pwgen

# デフォルトターゲット
all: $(TARGET)

# リンク
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete: $(TARGET)"

# コンパイル（.cc → .o）
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# ヘッダ依存関係の読み込み
-include $(DEPENDS)

# ビルドして即実行
run: $(TARGET)
	@$(TARGET)

# 生成ファイルの削除
clean:
	rm -rf $(OUT_DIR)
	@echo "Cleaned output directory"

# ビルドし直し
rebuild: clean all

# テスト実行
test: $(TARGET)
	$(CXX) $(CXXFLAGS) -o $(OUT_DIR)/test_runner tests/test_main.cc
	@echo "Running tests..."
	@$(OUT_DIR)/test_runner

.PHONY: all run clean rebuild test
