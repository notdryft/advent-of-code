#!/usr/bin/env bash

make mrproper && \
  make all CMDLINE_FLAGS=-DDEBUG && \
  make all

case "$(uname -s)" in
  Darwin)
    DYLD_LIBRARY_PATH=$(pwd)/../libaoc/lib:$DYLD_LIBRARY_PATH
    export DYLD_LIBRARY_PATH
    ;;
esac

for day in {1..2}; do
  "./bin/day$day"
done
