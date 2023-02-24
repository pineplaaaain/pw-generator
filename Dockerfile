# FROM --platform=linux/x86_64 debian:bullseye-slim
FROM debian:bullseye-slim

ENV DEBIAN_FRONTEND noninteractive
ENV LANG ja_JP.UTF-8
ENV TZ Asia/Tokyo

RUN apt-get update -y \
    && apt-get install -y --no-install-recommends \
    git \
    build-essential \
    cmake \
    clang \
    g++ \
    libssl-dev \
    libjsoncpp-dev \
    openssl \
    zlib1g-dev \
    uuid-dev \
    vim \
    locales \
    tzdata \
    && locale-gen ja_JP.UTF-8 \
    && localedef -f UTF-8 -i ja_JP ja_JP \
    && apt clean -y \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# EXPOSE 80
