#!/usr/bin/env bash

day=$1

inputs="../../inputs/2023/day$day"

mkdir -p "$inputs"

touch "$inputs/part1_test"
touch "$inputs/part2_test"

curl \
  -H "Cookie: $(cat .cookie)" \
  -o "$inputs/data" \
  https://adventofcode.com/2023/day/$day/input
