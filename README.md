# PW GENERATOR

セキュアなパスワードを生成するC++製コマンドラインツール

## 機能

- 8文字以上の任意の長さのパスワードが生成できます
- パスワードに用いる文字種の選択が可能です
  - 小文字 (a-z)
  - 大文字 (A-Z)
  - 数字 (0-9)
  - 記号 (!@#$%^&*()_+-=)
- 暗号学的にセキュアな乱数を利用しています

## 必要環境

- C++17対応コンパイラ (g++, clang++)
- make

## ビルド方法

```bash
make
```

## 使い方

```bash
./out/pwgen
```

対話形式でパスワードを生成します：

```
Enter password length (8 or more): 16
Use lowercase letters -- abcdefghijklmnopqrstuvwxyz (y/n)?: y
Use UPPERCASE letters -- ABCDEFGHIJKLMNOPQRSTUVWXYZ (y/n)?: y
Use numbers -- 0123456789 (y/n)?: y
Use symbols -- !@#$%^&*()_+-= (y/n)?: y
Generated password: aB3#xK9@mP2$qL7!
```

## Makeコマンド

```bash
make          # ビルド
make DEBUG=1  # デバッグビルド
make run      # ビルドして実行
make clean    # 生成ファイル削除
make rebuild  # クリーンビルド
```

## Dockerでのビルド・実行

```bash
# コンテナを起動
docker compose up -d

# コンテナ内でビルド
docker compose exec app make

# コンテナ内でビルド＆実行
docker compose exec app make run

# ワンライナーでビルドだけして終了
docker compose run --rm app make
```

## プロジェクト構成

```
.
├── src/
│   ├── main.cc              # メインプログラム
│   ├── PasswordGenerator.h  # パスワード生成ロジック
│   ├── PasswordCharset.h    # 文字セット定義
│   └── StringUtils.h        # 文字列ユーティリティ
├── out/                     # ビルド出力先
├── Makefile                 # ビルド設定
├── LICENSE                  # ライセンス
└── README.md
```

## ライセンス

MIT License