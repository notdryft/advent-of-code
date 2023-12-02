#!/usr/bin/env bash

name=$1

make mrproper && \
  make "${name}_test" && \
  "./bin/${name}_test"
