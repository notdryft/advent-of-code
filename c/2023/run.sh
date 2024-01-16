#!/usr/bin/env bash

args="$(getopt -o dit --long debug,info,trace -- "$@")"
eval set -- "$args"

while true; do
  case "$1" in
    -d | --debug) debug=true; shift ;;
    -i | --info) info=true;   shift ;;
    -t | --trace) trace=true; shift ;;
    * )                       shift  ; break ;;
  esac
done

if [[ $1 =~ [0-9]+ ]]; then
  binary="day$1"
else
  binary="${1}_test"
fi

if [ "$trace" = "true" ]; then
  flags=-DTRACE=true
elif [ "$debug" = "true" ]; then
  flags=-DDEBUG=true
elif [ "$info" = "true" ]; then
  flags=-DINFO=true
fi

make_flags=CMDLINE_FLAGS=$flags

make mrproper && \
  make "$binary" "$make_flags" && \
  "./bin/$binary"
