#!/usr/bin/env bash

if [[ $1 =~ [0-9]+ ]]; then
  binary="day$1"
else
  binary="${1}_test"
fi

make mrproper && \
  make "$binary" && \
  "./bin/$binary"
