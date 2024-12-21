#!/usr/bin/env bash

make mrproper && \
  make -j"$(nproc)" all CMDLINE_FLAGS=-DTRACE && \
  make -j"$(nproc)" all CMDLINE_FLAGS=-DDEBUG && \
  make -j"$(nproc)" all CMDLINE_FLAGS=-DTRACE && \
  make -j"$(nproc)" all

case "$(uname -s)" in
  Darwin)
    DYLD_LIBRARY_PATH=$(pwd)/../libaoc/lib:$DYLD_LIBRARY_PATH
    export DYLD_LIBRARY_PATH
    ;;
esac

for day in {1..21}; do
  "./bin/day$day"
done
