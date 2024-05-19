#!/usr/bin/env bash

day=$1

inputs="../../inputs/2015/day$day"

mkdir -p "$inputs"

touch "$inputs/sample"

curl \
  -H "Cookie: $(cat .cookie)" \
  -o "$inputs/data" \
  https://adventofcode.com/2015/day/$day/input

day=$day envsubst < src/day.tmpl > "src/day$day.c"

gsed -i"" "s/days = [0-9]\+/days = $day/" Makefile
