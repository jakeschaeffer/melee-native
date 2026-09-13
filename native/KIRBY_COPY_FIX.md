# Native match-start copy-asset fix — 2026-09-12

The installed Integration build aborted during Classic/VS match setup in
`ftKb_SpecialN_800EED50` → `MeleeNativeArchivePublic`. The user's VS case was
Kirby on Mute City; the log named `ftDataKirbyCopyCaptain`, with a model count of
1 being interpreted as a pointer. Copy assets are preloaded for the match, so
this failure can occur before Kirby uses an ability.

Only five copy roots had explicit native schemas. All other `ftDataKirbyCopy*`
names incorrectly fell through to the ordinary `ftData` fighter schema.

## Fix and upstream scope

- Dispatch all 25 copy roots explicitly; unknown copy roots fail with their name.
- Preserve the distinct hat-model and body-replacement prefix layouts, verified
  against the C views used by `LOAD_HAT`, including pointer-sized mask slots.
- Convert article, joint, animation, visibility and dynamics pointers by their
  actual types. Link arrows and Game & Watch articles need specialized layouts;
  packed Game & Watch copy colors remain bytes.
- Changes are entirely in `native/`; no decomp game-source changes or new assets.

## Regression coverage

`native_runtime_archive` includes generated, asset-free fixtures for all 25
names, checking both prefix layouts through actual C game types, cached lookup,
joint pointers and scalar masks. CI runs these under ASan/UBSan.

When owned assets are available, `native_kirby_copy_assets` loads all
`PlKbCp*.dat` roots and their referenced objects under the same sanitizers.
The old loader reproduced the exact Captain failure; the updated suite passed
all 32 local tests. Initial VS smoke samples passed for Captain Falcon,
Game & Watch, Mewtwo and Jigglypuff (matrix `20260912-190817-characters`).

Reproduce the user's stage/character combination with the packaged binary:

```sh
MELEE_MATRIX_TEST=1 MELEE_TEST_FORCE_STAGE=1 MELEE_TEST_STAGE=10 \
  MELEE_TEST_CHARACTER=4 MELEE_TEST_OPPONENT=0 MELEE_WIDESCREEN=1 \
  MELEE_TEST_CAPTURE=0 \
  MELEE_TEST_APP='dist/kirby-copy-fix/Melee Native.app/Contents/MacOS/melee_mac' \
  ruby native/tools/run_input_test.rb native/tests/mute-city-soak.input 180 /path/to/owned-disc.ciso
```

Repeat with `MELEE_TEST_PLAYERS=4`, character 2 and opponent 8 to add Captain
Falcon and Kirby as the third/fourth CPU slots. The hook verifies all four
fighters are actually loaded. These options require `MELEE_MATRIX_TEST` and
do not affect normal play. Also run `classic-win.input` using the instructions
in `CLASSIC_WIN_FIX.md`; its log now records each round's actual lineup.

These are loading/transition and limited combat checks, not exhaustive testing
of every copied move, every stage or a complete Classic playthrough. Run records
and private captures stay in ignored `build/`; the package includes no game data.
