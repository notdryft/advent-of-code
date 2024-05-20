#!/usr/bin/env bash

day=$1

inputs="../../inputs/2023/day$day"

mkdir -p "$inputs"

touch "$inputs/sample"

curl \
  -H "Cookie: $(cat .cookie)" \
  -o "$inputs/data" \
  https://adventofcode.com/2023/day/$day/input

day=$day envsubst < src/day.tmpl > "src/day$day.c"

gsed -i"" "s/days = [0-9]\+/days = $day/" Makefile
gsed -i"" "s/{1\.\.[0-9]\+}/{1..$day}/" check.sh
echo "day $day:" | tee -a IDEAS.md
