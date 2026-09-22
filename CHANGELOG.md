# v0.4 — Companions & little adventures

- Choose a baby dragon, treasure bunny or garden fairy; companion magic lights helpful targets and opens matching secret doors.
- Five island activities using four challenge types: cloud-sheep guidance, rain-channel turning, moonlight crystal turning and floating picnic catches. No lives or penalties; repeat play does not duplicate rewards.
- Earn a themed outfit and 20 stars for each first completion. Companions can wear owned accessories; the existing shop previews outfits before spending stars.
- Discover five secret rooms, each with a unique display treasure and a one-time 10-star reward.
- Persistent outcomes include a rescued sheep, blooming garden, lit crystals, Pip's picnic and Bonbon's bow; friends welcome you back.
- Cottage play: arrange a picnic at a placed table, tuck your companion into a placed bed, display treasures and invite rescued friends who react to furniture.
- B opens the companions/adventures menu. X lets the companion help in new activities. Y gives increasingly specific hints. Optional offline spoken hints are off by default and can be enabled separately from music.
- CW4 saves import CW1/CW2/CW3 without clearing stars, purchases, missions or furniture. Legacy saves receive a pre-0.4 backup attempt. Save failures roll back new rewards and cottage changes.
- Preserves the verified 640×480 display mode and pitch-aware 180-degree presentation.

Validation: all original and new host regression tests passed, including five activity completions, reward idempotence, secret companion gates, old-save migration, cottage persistence, failed writes, rendered screens, rotation and spoken-audio playback. ARM build passed. A physical Miyoo test of the new content and spoken audio is still needed.

The website received the corresponding update. The platforms keep separate local saves and use controls suited to each device.

---

# Cloudwish Miyoo changelog

## v0.3

Expanded handheld adventure.

- Added Fairy Garden, Crystal Peaks, Candy Clouds and Moon Meadow, plus Cloud Home.
- Added 16 missions for 23 total, sequential island unlocks and a SELECT travel map.
- Added distinct magic activities: Luna's rainbow paths, Poppy's flower patterns, Ember's warming challenge and Milo's star memory.
- Added idle/walking motion, celebration hops, hearts, sparkles and contextual dialogue.
- Expanded to seven wearable accessories with individual creature equipment.
- Added six furniture items and six home-decorating spots, with free placement/movement.
- Added original synthesized background music and magic/reward sounds, with a saved sound toggle.
- Introduced CW3 saves with CW1/CW2 migration and a legacy backup attempt.
- Preserved the verified display setup and 180-degree rotation.
- Added save protection, transaction rollback, full progression, rendering and audio tests.

Validation: ARM build and host tests passed. New content, audio and performance still require a physical-device check. This is a handheld adaptation, not a pixel-for-pixel copy of the browser game.

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
