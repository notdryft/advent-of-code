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

binary="day$1"

if [ "$trace" = "true" ]; then
  flags=-DTRACE
elif [ "$debug" = "true" ]; then
  flags=-DDEBUG
elif [ "$info" = "true" ]; then
  flags=-DINFO
fi

make_flags=CMDLINE_FLAGS=$flags

case "$(uname -s)" in
  Darwin)
    DYLD_LIBRARY_PATH=$(pwd)/../libaoc/lib:$DYLD_LIBRARY_PATH
    export DYLD_LIBRARY_PATH
    ;;
esac

make mrproper && \
  make "$binary" "$make_flags" && \
  "./bin/$binary"
