FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    g++ cmake git libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY CMakeLists.txt .
COPY backend/ backend/

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j$(nproc)

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/chess_server /usr/local/bin/chess_server

EXPOSE 18080

CMD ["chess_server"]
