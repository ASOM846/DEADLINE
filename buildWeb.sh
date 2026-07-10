#!/usr/bin/env bash
set -e

source "$HOME/emsdk/emsdk_env.sh"

emcc -o index.html \
  $(find src -type f -name "*.cpp") \
  -Isrc \
  -I"$HOME/libs/raylib/src" \
  "$HOME/libs/raylib/src/libraylib.web.a" \
  -O3 \
  -flto \
  -Wall \
  -DPLATFORM_WEB \
  -sUSE_GLFW=3 \
  -sALLOW_MEMORY_GROWTH=1 \
  -sASSERTIONS=0 \
  -sINITIAL_MEMORY=67108864 \
  -sSTACK_SIZE=5242880 \
  --closure 1 \
  --shell-file minshell.html \
  --preload-file assets

zip -r web_build.zip index.html index.js index.wasm index.data assets

rm index*
