#!/usr/bin/env bash

day=$1

make mrproper && \
  make "day$day" && \
  "./bin/day$day"
