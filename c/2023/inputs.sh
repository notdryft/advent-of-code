#!/usr/bin/env bash

day=$1

curl \
  -H "Cookie: $(cat .cookie)" \
  -o "inputs/day$day/input" \
  https://adventofcode.com/2023/day/$day/input
