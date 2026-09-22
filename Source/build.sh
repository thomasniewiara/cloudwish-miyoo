#!/bin/sh
set -eu
cd "$(dirname "$0")"
TC=${MIYOO_TOOLCHAIN:-/opt/miyoomini-toolchain}
SYS="$TC/usr/arm-linux-gnueabihf/sysroot"
"$TC/usr/bin/arm-linux-gnueabihf-gcc" --sysroot="$SYS" -DMIYOO_ROTATE_180 -std=gnu99 -O2 -Wall -Wextra -Werror -I"$SYS/usr/include/SDL" main.c model.c render.c audio.c -L"$SYS/usr/lib" -Wl,-rpath-link,"$SYS/usr/lib" -lSDL -lm -o ../Roms/PORTS/Games/Cloudwish/cloudwish
"$TC/usr/bin/arm-linux-gnueabihf-strip" ../Roms/PORTS/Games/Cloudwish/cloudwish
