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
