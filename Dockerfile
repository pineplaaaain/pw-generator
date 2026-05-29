# ビルドステージ
FROM debian:bullseye-slim AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y \
    && apt-get install -y --no-install-recommends \
    build-essential \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY src/ src/
COPY Makefile .

RUN make

# 実行ステージ
FROM debian:bullseye-slim

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=ja_JP.UTF-8
ENV TZ=Asia/Tokyo

RUN apt-get update -y \
    && apt-get install -y --no-install-recommends \
    locales \
    tzdata \
    && locale-gen ja_JP.UTF-8 \
    && localedef -f UTF-8 -i ja_JP ja_JP \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/out/pwgen /usr/local/bin/pwgen

ENTRYPOINT ["pwgen"]
