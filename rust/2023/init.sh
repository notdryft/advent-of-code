#!/usr/bin/env bash

day=$1

inputs="../../inputs/2023/day$day"

mkdir -p "$inputs"

touch "$inputs/sample"

curl \
  -H "Cookie: $(cat .cookie)" \
  -o "$inputs/data" \
  https://adventofcode.com/2023/day/$day/input

day=$day envsubst < src/bin/day.tmpl > "src/bin/day$day.rs"
