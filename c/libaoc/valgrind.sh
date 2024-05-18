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

binary="${1}_test"

if [ "$trace" = "true" ]; then
  flags=-DTRACE
elif [ "$debug" = "true" ]; then
  flags=-DDEBUG
elif [ "$info" = "true" ]; then
  flags=-DINFO
fi

make_flags=CMDLINE_FLAGS=$flags

case $(uname -s) in
  Darwin)
    docker build --platform linux/amd64 -f Dockerfile.valgrind -t valgrind:latest .
    docker run -it --rm \
      --platform linux/amd64 \
      --volume "$(pwd)/../..:/app" \
      valgrind:latest \
      bash -c "make mrproper && make mem-$binary $make_flags"
    ;;
  Linux)
    make mrproper && make "mem-$binary" "$make_flags"
    ;;
esac
