#!/bin/sh
cd /mnt/SDCARD/Roms/PORTS/Games/Cloudwish || exit 1
# Use the same SDL search order as Onion 4.3.1.
export LD_LIBRARY_PATH="/lib:/config/lib:/mnt/SDCARD/miyoo/lib:/mnt/SDCARD/.tmp_update/lib:/mnt/SDCARD/.tmp_update/lib/parasyte"
{
    # Older imported shortcuts had audio disabled. Use Onion's own helper
    # here so replacing this game folder also enables the native audio path.
    if pgrep -x audioserver >/dev/null 2>&1 || pgrep -x audioserver.mod >/dev/null 2>&1; then
        miyoodir=/mnt/SDCARD/miyoo
        if [ -f /mnt/SDCARD/.tmp_update/script/stop_audioserver.sh ]; then
            . /mnt/SDCARD/.tmp_update/script/stop_audioserver.sh
        fi
    fi
    exec ./cloudwish
} > cloudwish.log 2>&1
