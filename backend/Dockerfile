FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    uuid-dev \
    libssl-dev \
    zlib1g-dev \
    libpq-dev \
    libjsoncpp-dev \
    pkg-config \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp_bcrypt
RUN git clone https://github.com/rg3/libbcrypt.git && \
    cd libbcrypt && \
    make && \
    cp bcrypt.h /usr/local/include/ && \
    cp bcrypt.a /usr/local/lib/libbcrypt.a && \
    cd / && rm -rf /tmp_bcrypt

WORKDIR /tmp_build
RUN git clone https://github.com/drogonframework/drogon.git && \
    cd drogon && \
    git submodule update --init && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc) && make install && \
    cd / && rm -rf /tmp_build

WORKDIR /tmp_build_jwt
RUN git clone https://github.com/Thalhammer/jwt-cpp.git && \
    cd jwt-cpp && \
    mkdir build && cd build && \
    cmake .. && \
    make install && \
    cd / && rm -rf /tmp_build_jwt

WORKDIR /build
COPY . .

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . -j$(nproc)

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    ca-certificates \
    libcurl4 \
    libpq5 \
    libjsoncpp25 \
    libssl3 \
    uuid-runtime \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /usr/local/lib/libdrogon* /usr/lib/
COPY --from=builder /usr/local/lib/libtrantor* /usr/lib/
COPY --from=builder /build/build/ScientificEventsPlatform .
COPY --from=builder /build/config.json ./config.json
COPY --from=builder /build/db/init.sql ./db/init.sql
COPY --from=builder /build/services ./services

EXPOSE 8080

CMD ["./ScientificEventsPlatform"]