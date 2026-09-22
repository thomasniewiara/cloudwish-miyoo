# Cloudwish Miyoo changelog

## v0.2

Mission rewards and clothing shop.

- Earn 10 stars per mission; import completed CW1 missions with their earned credit.
- Added bow, crown and scarf, previews and shared item ownership.
- Each creature remembers its equipped accessory; owned looks can be worn again for free.
- X opens the shop; purchases and outfit changes save immediately.
- Introduced CW2 saves, retaining CW1 import.
- Added affordability, repeat-purchase, persistence and failed-write rollback tests.

Validation: host tests passed. No explicit physical-device confirmation for this release is recorded.

## v0.1.2

Corrected upside-down output on the Miyoo.

- Added pitch-aware 180-degree frame rotation for handheld builds.
- Preserved normal orientation for desktop builds.
- Extended rendering regression tests to verify rotation.

Device feedback: the user confirmed the game worked, then confirmed the gameplay/save check worked. This is the last release with explicit device confirmation in the conversation.

## v0.1.1

Display-format correction.

- Replaced the 16-bit display request with a 32-bit hardware display and a separate drawing canvas.
- Matched Onion 4.3.1's SDL library search order.
- Added video diagnostics and pixel-copy regression tests.
- Preserved CW1 save compatibility.

Device feedback: the next reported issue was upside-down output. Superseded by 0.1.2.

## v0.1

Initial native demo for Miyoo Mini Plus / Onion OS 4.3.1.

- Rainbow Meadow, four playable creatures, seven mission steps and Pip's rescue.
- D-pad movement, creature switching, connect-the-points magic and hints.
- CW1 local autosaves, pause menu and save/quit.
- ARM binary, artwork, launcher, source and installation instructions.

Known issue: the user reported severely scrambled display rows. Superseded by 0.1.1. No shop or audio in this release.
