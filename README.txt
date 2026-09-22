CLOUDWISH KINGDOM — MIYOO ADVENTURE 0.3
For Miyoo Mini Plus / Onion OS 4.3.1

WHAT'S NEW
- Five adventure islands plus Cloud Home. Rainbow Meadow is joined by
  Fairy Garden, Crystal Peaks, Candy Clouds and Moon Meadow.
- 23 mission steps total: the original 7 and 16 new missions.
- Four different magic activities: rainbow paths, flower patterns, gentle
  lantern warming and star memory. Hints and retries never cost stars.
- Walking/idle motion, celebration hops, hearts, sparkles, happy friends and
  creature-specific dialogue when helping others.
- Seven wearable accessories: bow, crown, scarf, cape, party hat, wand and
  petal skirt. Each friend remembers its own outfit. Purchases are shared.
- Six furniture items to buy and place/move among six spots at Cloud Home:
  cloud bed, star lamp, flower pot, cozy sofa, tea table and rainbow rug.
- Original synthesized background melody and magic/reward sounds. Sound
  can be switched off in the pause menu; the setting is saved.

These are handheld adaptations using simple native graphics and controls,
not a pixel-for-pixel copy of the browser game. No real-money purchases,
network connection, browser or separate ROM emulator is needed.

UPDATE FROM AN EARLIER DEMO
1. Use START > Save and quit, then shut down the Miyoo.
2. Put its SD card in your computer. Back up the existing folder:
   Roms/PORTS/Games/Cloudwish/saves
3. Extract this ZIP. Merge its Roms folder into the SD card's existing Roms
   folder. Replace the files when prompted. Do NOT delete/replace the whole
   Roms folder, the whole Cloudwish folder, or your saves folder.
4. Safely eject the card, start the Miyoo and open the existing Cloudwish
   entry. The updated run.sh handles audio even with an older shortcut.

Existing CW1/CW2 saves are read automatically. CW1 receives credit for old
completed missions; CW2 retains the stars and outfits already purchased.
The game attempts to keep a pre-0.3-backup.txt copy before first conversion.
The new CW3 format stores all islands, furniture and sound settings. Older
builds cannot read CW3; restore your backup if you choose to downgrade.
This ZIP contains no saves and will not replace your saved game.

FIRST INSTALL
Enable Ports Collection in Apps > Package Manager > Verified, then copy
this package's Roms folder to the root of the SD card. Run Games > Ports >
~Import ports and find Cloudwish under Adventure. Refresh the game list
if necessary. Theme labels can differ.
Expected executable: /Roms/PORTS/Games/Cloudwish/cloudwish
Expected launcher: /Roms/PORTS/Games/Cloudwish/run.sh

CONTROLS
D-pad   Walk, select menu items or puzzle targets
A       Help, connect, choose, buy, equip, or place furniture
B       Back; in Luna's puzzle, undo the latest point before leaving
Y       Mission hint; puzzle hint (Milo replays the star sequence)
X       Clothing/furniture shop while exploring
SELECT  Travel map while exploring
L1/R1   Switch creature; also works when previewing clothes
L2/R2   Alternate creature-switch buttons
START   Pause/resume; controls, sound toggle, Save and quit

SHOP
Up/down chooses an item. Left/right switches outfits and furniture.
A buys and wears an outfit, or buys a decoration. B leaves. Owned outfits
can be equipped again for free. Choose Original look to remove an outfit.
The preview does not spend stars until you press A. Each item unlocks for
all four creatures. Outfit prices: 10, 15, 20, 25, 25, 30, 30 stars.
Every furniture item costs 20 stars. Furniture placement is always free.

ISLANDS AND MISSIONS
Follow the glowing mission marker; use the creature named in the quest.
The original Rainbow Meadow sequence includes Pip's rescue. Return Pip
home to unlock Fairy Garden (you can finish Meadow's other tasks later).
Finish all four Fairy Garden tasks to open Crystal Peaks, then all four
Peak tasks to open Candy Clouds, then all four Candy tasks to open Moon
Meadow. The final Moon task starts a sky celebration.
Cloud Home is always available from SELECT, even before rescuing Pip.

Meadow missions give 10 stars each; new-island missions give 15. You can
earn 310 stars altogether; every outfit and decoration together costs 275.
Rewards are given once per mission. Completed islands remain visitable.
Press A on a completed island for a happy greeting.

LUNA: Use the D-pad to select numbered points on a 3x3 rainbow grid. A
connects the next point. B undoes a point. Y points to the next number.
POPPY: Pick the flower that completes the repeating pattern. Three patterns
make the flowers bloom. Shapes also identify the choices; Y offers a hint.
EMBER: Hold A to build warmth, then release while the marker is in the gold
band. Warm three lights. Too warm/too cool? Try again, with no penalty.
MILO: Watch four stars glow in order. Then use D-pad and A to repeat the
sequence. Y replays it from the beginning. There is no response deadline.

DECORATING
Visit Cloud Home with SELECT. Stand near one of the six floor circles and
press A. Choose an owned decoration and press A to place it. Placing an
item already elsewhere moves it to the new spot. Replaced items remain
owned. Empty space removes an item without losing it. X outside this menu
opens the shop. Your room arrangement saves as soon as you change it.

SAVES AND QUITTING
Progress saves after every mission, purchase, outfit change, decoration,
island trip and sound-setting change. Use START > Save and quit before
turning off the device. Mid-puzzle progress is not saved. Native ports do
not use RetroArch save states; GameSwitcher/suspend behavior is not claimed.
Save: Roms/PORTS/Games/Cloudwish/saves/slot1.txt
If saving fails, transactions are rolled back and the game displays an
error. Check SD free space. An unreadable save is preserved; the game exits
and records the problem in cloudwish.log instead of overwriting it.

VALIDATION AND DEVICE CHECK
The previous display orientation, gameplay and save system were confirmed
working by the user on their Miyoo. The 0.3 additions are compiled and host
tested, but have not yet been tested on a physical device.
Host tests cover all 23 mission steps and four puzzles, island locks,
backward save migration, rewards, purchases, furniture movement, insufficient
funds, failed-save rollback, menus, 32 outfit/creature combinations, all new
screens and the 180-degree display copy. Audio tests verify generated output,
mute, bounded amplitude and the SDL audio callback using a dummy device.
Actual sound output, volume and performance need the physical Miyoo check.

TRY ON THE MIYOO
- Use SELECT to visit Fairy Garden (if Pip is already home).
- Complete one flower puzzle and one lantern-warming task.
- Buy furniture with X, then visit Cloud Home and place it with A.
- Toggle Sound from START, then save/quit and reopen.
- Confirm island progress, outfit, stars and room arrangement return.

If it does not launch, looks wrong, or has no sound, send cloudwish.log from
Roms/PORTS/Games/Cloudwish and describe what appeared. If audio cannot open,
the game continues silently. Use the device's volume controls as usual.

The Source folder includes rebuild instructions and tests. No toolchain or
third-party runtime libraries are bundled; the build uses Onion's native
SDL 1.2, libc and libm. No overclocking is enabled.
