#!/usr/bin/env bash

make mrproper && \
  make all CMDLINE_FLAGS=-DDEBUG && \
  make all

for day in {1..25}; do
  "./bin/day$day"
done
