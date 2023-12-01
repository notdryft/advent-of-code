#!/usr/bin/env bash

day=$1

mkdir -p "../../inputs/2023/day$day"

curl \
  -H "Cookie: $(cat .cookie)" \
  -o "../../inputs/2023/day$day/input" \
  https://adventofcode.com/2023/day/$day/input
