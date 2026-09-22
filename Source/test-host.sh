#!/bin/sh
set -eu
cd "$(dirname "$0")"
SDL_CONFIG=${SDL_CONFIG:-sdl-config}
CC=${CC:-cc}
OUT=${TEST_BINARY:-/tmp/cloudwish-tests}
# sdl-config intentionally supplies compiler/linker argument words.
$CC -std=gnu99 -O2 -Wall -Wextra -Werror -DMIYOO_ROTATE_180 tests.c model.c render.c audio.c adventure.c $($SDL_CONFIG --cflags --libs) -lm -o "$OUT"
cd ../Roms/PORTS/Games/Cloudwish
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy "$OUT" "$@"
