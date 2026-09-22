CLOUDWISH KINGDOM — MIYOO DEMO 0.1
Target: Miyoo Mini Plus, Onion OS 4.3.1
Status: compiled ARM hardware-test build. NOT tested on a physical Miyoo.

INSTALL
1. Shut down the Miyoo and put its SD card in your computer.
2. Copy the Roms folder from this package to the root of the SD card,
   merging it with the existing Roms folder. Do not replace/delete that folder.
   The result must include:
   /Roms/PORTS/Games/Cloudwish/cloudwish
   /Roms/PORTS/Games/Cloudwish/run.sh
   /Roms/PORTS/Shortcuts/Adventure/Cloudwish.notfound
3. Safely eject the card and start the Miyoo.
4. Enable Ports Collection in Apps > Package Manager > Verified if necessary.
5. Open Games > Ports and run ~Import ports. Then look under Adventure
   for Cloudwish. Menu labels may vary with your theme.
6. If the entry does not appear, refresh the game list and reopen Ports.

PLAY
D-pad    Walk; select numbered spell points
A        Talk / use magic / connect selected spell point
B        Undo last spell point; back out when no points are connected
Y        Hint; in a spell, select the next needed point
L1/R1    Change creature (L2/R2 also supported)
START    Pause / resume
Pause    D-pad to choose; A to confirm. Choose Save and quit to exit.

Follow the golden mission marker. Move close, choose the creature named
in the quest, and press A. Connect points 1–4 with A; use D-pad and B as
needed. There are no timers, falling penalties, purchases or online features.

MISSION ROUTE
Milo reveals the hidden stars. Luna makes the rainbow bridge. Cross its
center to find Pip, press A, and walk back to the HOME marker with Pip.
Then Ember lights the lantern, Poppy welcomes Pip with flowers, and Milo
brightens the sky. Visit Pip again after finishing for a happy greeting.

WHAT IS INCLUDED
Four original Cloudwish creature sprites, a simplified Rainbow Meadow,
seven mission steps, four-point magic interactions and local progress saves.
This is a small native adaptation, not the full browser game or a ROM file.
Magic interactions share the same point-connection mechanic in this demo.
The shop, outfits, other islands and audio are not included yet.

SAVES
Autosaves after each completed mission and when using Save and quit.
Save file: Roms/PORTS/Games/Cloudwish/saves/slot1.txt
Use Save and quit before turning off the device. Mid-puzzle progress is
not saved. Browser saves and Onion emulator save states are separate.
To start over, exit and rename slot1.txt on your computer to keep a backup.
A save-error message means the card may be full or unwritable.

FIRST DEVICE TEST (about 5 minutes)
- Does the game open with a readable, correctly oriented 640x480 screen?
- Do D-pad, A, B, Y and both shoulder-button pairs match the controls above?
- Can Milo reveal stars and Luna open the bridge?
- Can you reach Pip, escort Pip home, and finish all three follow-up tasks?
- Does Save and quit return to Onion, and does relaunch restore progress?
Frame pacing, hardware button behavior, launcher integration and power/menu
interactions still require this physical-device check. Use START to exit;
GameSwitcher/suspend behavior is not claimed or implemented by this demo.

TROUBLESHOOTING
If it returns to Onion immediately, send the contents of:
Roms/PORTS/Games/Cloudwish/cloudwish.log
If that file is absent, confirm Ports is installed and the exact paths above.
Also tell us whether you saw a black screen or an error. No OS upgrade is
required by these instructions. Don't change firmware to troubleshoot this
untested first build.

BUILD / VALIDATION
Source and rebuild instructions are in Source/. The package contains a
compiled 32-bit ARM EABI5 Linux executable, dynamically linked to SDL 1.2,
libm and libc supplied by the device/Onion. No third-party shared libraries
are bundled. No overclock setting is enabled; the game caps at about 30 fps.
Host tests passed for save roundtrip, malformed saves, bridge restrictions,
all mission steps, creature gating, wrong spell point, undo and save/quit.
Screen layouts were inspected through host SDL's dummy video driver.
These checks do not prove that the binary runs on the physical handheld.

References:
https://onionui.github.io/docs/ports
https://github.com/OnionUI/Onion
https://github.com/OnionUI/Ports-Collection
https://github.com/shauninman/union-miyoomini-toolchain
