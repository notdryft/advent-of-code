#!/usr/bin/env bash

if [[ $1 =~ [0-9]+ ]]; then
  binary="day$1"
else
  binary="${1}_test"
fi

docker build -f Dockerfile.valgrind -t valgrind:latest .
docker run -it --rm \
  -v "$(pwd)/../..:/app" \
  valgrind:latest \
  bash -c "make mrproper && make mem-$binary"
