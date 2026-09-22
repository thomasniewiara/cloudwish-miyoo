#!/bin/sh
cd /mnt/SDCARD/Roms/PORTS/Games/Cloudwish || exit 1
export LD_LIBRARY_PATH="/mnt/SDCARD/.tmp_update/lib/parasyte:/mnt/SDCARD/.tmp_update/lib:/customer/lib:/lib:/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
exec ./cloudwish > cloudwish.log 2>&1
