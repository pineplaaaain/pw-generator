# PW GENERATOR

セキュアなパスワードを生成するC++製コマンドラインツール

## 機能

- 8文字以上の任意の長さのパスワードが生成できます
- パスワードに用いる文字種の選択が可能です
  - 小文字 (a-z)
  - 大文字 (A-Z)
  - 数字 (0-9)
  - 記号 (!#$%^&*()_+-=)
- 選択した文字種それぞれから最低1文字が含まれることを保証します
- 暗号学的にセキュアな乱数（`std::random_device`）を直接利用しています
- パスワード強度（エントロピー）を表示します
- 1個生成時は自動でクリップボードにコピーします
- コマンドライン引数によるワンショット実行と対話モードの両方に対応

## 必要環境

- C++17対応コンパイラ (g++, clang++)
- make

## ビルド方法

```bash
make
```

## 使い方

### ワンショットモード（コマンドライン引数）

```bash
# 16文字、全文字種（自動クリップボードコピー）
./out/pwgen 16

# 20文字、小文字+数字のみ
./out/pwgen 20 -ln

# 5個まとめて生成
./out/pwgen 16 -c 5

# 5個生成してクリップボードにもコピー
./out/pwgen 16 -c 5 --copy

# ヘルプ表示
./out/pwgen --help
```

出力例：

```
Z+)IMf0H3$NO##S7
Strength: 99.7 bits (Very Strong)
(Copied to clipboard)
```

### オプション一覧

| オプション | 説明                         |
| ---------- | ---------------------------- |
| `-l`       | 小文字を含める (a-z)         |
| `-u`       | 大文字を含める (A-Z)         |
| `-n`       | 数字を含める (0-9)           |
| `-s`       | 記号を含める (!#$%^&*()_+-=) |
| `-c <N>`   | 生成数を指定 (1-100)         |
| `--copy`   | クリップボードにコピー       |
| `--help`   | ヘルプ表示                   |

文字種オプションを指定しない場合は全種が使用されます。

### 対話モード

引数なしで実行すると対話形式で動作します：

```bash
./out/pwgen
```

```
Enter password length (8 or more): 16
Use lowercase letters -- abcdefghijklmnopqrstuvwxyz (y/n)?: y
Use UPPERCASE letters -- ABCDEFGHIJKLMNOPQRSTUVWXYZ (y/n)?: y
Use numbers -- 0123456789 (y/n)?: y
Use symbols -- !#$%^&*()_+-= (y/n)?: y
How many passwords to generate? (default: 1): 1
aB3#xK9@mP2$qL7!
Strength: 99.7 bits (Very Strong)
(Copied to clipboard)
```

## Makeコマンド

```bash
make          # ビルド
make DEBUG=1  # デバッグビルド
make run      # ビルドして実行
make test     # テスト実行
make clean    # 生成ファイル削除
make rebuild  # クリーンビルド
```

## Dockerでのビルド・実行

### 開発用（ソースマウント）

```bash
# コンテナを起動
docker compose up -d

# コンテナ内でビルド
docker compose exec app make

# コンテナ内でビルド＆実行
docker compose exec app make run

# テスト実行
docker compose exec app make test

# ワンライナーでビルドだけして終了
docker compose run --rm app make
```

### 本番用（マルチステージビルド）

```bash
# 本番イメージをビルド
docker build -t pwgen .

# 実行
docker run --rm pwgen 16
docker run --rm pwgen 16 -c 5 -ln
```

## プロジェクト構成

```
.
├── src/
│   ├── main.cc              # エントリポイント（モード切り替え）
│   ├── ArgParser.h          # コマンドライン引数パーサー
│   ├── Config.h             # 設定構造体と定数
│   ├── Clipboard.h          # クリップボード操作（OS別）
│   ├── Entropy.h            # パスワード強度計算
│   ├── InteractiveMode.h    # 対話モード
│   ├── PasswordGenerator.h  # パスワード生成ロジック
│   ├── PasswordCharset.h    # 文字セット定義
│   └── StringUtils.h        # 文字列ユーティリティ
├── tests/
│   └── test_main.cc         # ユニットテスト
├── out/                     # ビルド出力先
├── Makefile                 # ビルド設定
├── Dockerfile               # 本番用（マルチステージ）
├── Dockerfile.dev           # 開発用
├── docker-compose.yml       # 開発環境
└── README.md
```

## ライセンス

MIT License
