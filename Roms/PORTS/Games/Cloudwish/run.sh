#!/bin/sh
cd /mnt/SDCARD/Roms/PORTS/Games/Cloudwish || exit 1
# Match Onion 4.3.1's library order; do not prefer the alternate Ports SDL.
export LD_LIBRARY_PATH="/lib:/config/lib:/mnt/SDCARD/miyoo/lib:/mnt/SDCARD/.tmp_update/lib:/mnt/SDCARD/.tmp_update/lib/parasyte"
exec ./cloudwish > cloudwish.log 2>&1
