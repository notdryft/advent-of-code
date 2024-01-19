#!/usr/bin/env bash

if [[ $1 =~ [0-9]+ ]]; then
  binary="day$1"
else
  binary="${1}_test"
fi

case $(uname -s) in
  Darwin)
    docker build --platform linux/amd64 -f Dockerfile.valgrind -t valgrind:latest .
    docker run -it --rm \
      --platform linux/amd64 \
      --volume "$(pwd)/../..:/app" \
      valgrind:latest \
      bash -c "make mrproper && make mem-$binary"
    ;;
  Linux)
    make mrproper && make "mem-$binary"
    ;;
esac
