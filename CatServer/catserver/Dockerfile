FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    cmake g++ make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY cs/ .

RUN cmake -S . -B build && cmake --build build

RUN mkdir -p /app/stats

WORKDIR /app/build
CMD ["./catserver", "32132", "45341", "0.0.0.0"]
