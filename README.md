# Cloudwish Kingdom for Miyoo Mini Plus

Native SDL 1.2 game for **Miyoo Mini Plus / Onion OS 4.3.1**.

Latest version in this snapshot: **v0.4**.

## Companion adventures
Choose a companion with **B**, explore five new activities and secret rooms, earn outfits, and play at your cottage. Spoken hints are available offline and start switched off.

[Play the website](https://cloudwish-kingdom.thomas823516.chatgpt.site). Saves stay separate on each device.

## Install
Download the matching ZIP under [releases](releases/), extract it and merge its
`Roms` folder onto your SD card. Preserve and back up the existing `saves` folder.
See [installation and controls](README.txt) for the full instructions.

## Version archive
Every original released ZIP is retained unchanged, with per-version notes and
SHA-256 checksums. See [CHANGELOG.md](CHANGELOG.md) and [release index](releases/index.json).
Git tags identify the source snapshot for each release. History was imported
from the released archives; these commits are archival imports, not the original
chronological development commits. No original timestamps have been fabricated.

## Build and tests
See [Source/BUILD.txt](Source/BUILD.txt). ARM binaries are included in `Roms` and
the release ZIPs. Toolchains and runtime libraries are not bundled.

## Validation
Version 0.1.2 was confirmed working on the user's Miyoo, including the gameplay/save
check. Later release notes distinguish host-tested features from device validation.
Versions 0.1 and 0.1.1 are retained for history; they have known display defects.

## Artwork and third-party notices
Creature artwork comes from the Cloudwish game project. The bundled bitmap font
is derived from DejaVu Sans; see [font notices](Source/FONT-LICENSE.txt).
No new open-source license grant is implied by this archive.
