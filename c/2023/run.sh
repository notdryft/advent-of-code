#!/usr/bin/env bash

args="$(getopt -o d --long debug -- "$@")"
eval set -- "$args"

while true; do
  case "$1" in
    -d | --debug) debug=true; shift ;;
    * )                       shift  ; break ;;
  esac
done

if [[ $1 =~ [0-9]+ ]]; then
  binary="day$1"
else
  binary="${1}_test"
fi

if [ "$debug" = "true" ]; then
  make_flags=CMDLINE_FLAGS=-DDEBUG=true
else
  make_flags=CMDLINE_FLAGS=
fi

make mrproper && \
  make "$binary" "$make_flags" && \
  "./bin/$binary"
