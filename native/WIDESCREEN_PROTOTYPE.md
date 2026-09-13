# macOS widescreen prototype

Originally developed on `prototype/macos-widescreen`, based on the Classic
crash fixes, and included in `integrate/decomp-2026-09-12`.
This is a separate experiment, not part of the Classic pull request.

## Use

The original local prototype was installed as **Melee Native Widescreen.app**.
Build or package this branch separately; that installed app is not automatically
updated by the integration. See [current validation](INTEGRATION_VALIDATION.md).
Open **View > Aspect Ratio > 21:9** to enable it, or choose **Native (4:3)**.
F8 switches between those choices. Both menu and keyboard choices are remembered.
21:9 starts a 1680 × 720 window; Native starts a 960 × 720 window. Resizing or
fullscreen preserves the exact selected aspect ratio with black bars as needed.
**View > Toggle Full Screen**, Control-Command-F, or F11 toggles fullscreen.
Some Mac keyboards require Fn with the function keys.

Return pauses/resumes; WASD moves; X attacks/confirms; Z specials/goes back;
C/V jumps. The original app and Classic build are not replaced.

## Scope

- Widen the horizontal gameplay view while retaining original vertical framing
  and fighter proportions. Do not stretch a finished 4:3 image.
- The wide view is exactly 21:9, not an arbitrary monitor/window aspect ratio.
  Non-21:9 surfaces fit the image without distortion. GPU projection and black
  surface masks use the same aspect-fit calculation.
- Keep the HUD in its original centered 4:3 region. Menus, character selection,
  and non-match scenes remain 4:3 with side bars as appropriate.
- Leave camera-follow parameters, physics, collision, and blast zones unchanged.
- Restore the previous render pass after fighter shadow rendering. The original
  end-render function is a no-op, leaving subsequent on-screen cameras on the
  offscreen path and bypassing the projection correction.
- Keep actual offscreen shadow projections unmodified.

This is not a complete widescreen conversion. CPU-side visibility decisions,
offscreen indicators, screen-space effects, unusual cameras, and every stage or
game mode have not been exhaustively checked. The original camera can still
frame action conservatively. Toggle widescreen off if an effect looks wrong.
Progress saving remains unsupported by the underlying experimental app.

## Reproduce

```sh
cmake --build build/native-app --target melee_mac all --parallel 4
ctest --test-dir build/native-app -L melee --output-on-failure
MELEE_WIDESCREEN=1 MELEE_TEST_CAPTURE=0 \
  MELEE_TEST_APP=build/native-app/melee_mac.app/Contents/MacOS/melee_mac \
  ruby native/tools/run_input_test.rb native/tests/match-controls.input 180 /path/to/owned-US-1.02-disc.ciso
```

`MELEE_WIDESCREEN=0` forces original framing; `MELEE_FULLSCREEN=1` starts
fullscreen. Environment overrides apply to command-line launches.

## Original prototype validation (2026-09-12, before decomp integration)

- Apple M5 Pro / macOS 26.6.2 / Metal; native arm64 build.
- 27 component and asset tests passed, including sanitized projection tests.
- Classic reached an actual CPU fight through the normal menus.
- Paused-frame 4:3/widescreen comparison on Great Bay: fighter, scenery, and HUD
  positions in the center remain unchanged; additional world appears at both
  edges. The earlier prototype was checked at 1280 × 720 and fullscreen;
  the 21:9 revision adds aspect-fit tests for 1680 × 720, 1920 × 1080,
  3440 × 1440, 960 × 720, and portrait surfaces, plus menu-scene invariance.
- Package signature verification passed. Four dependent libraries bundled;
  no disc image or extracted game assets included.
- Installed package completed the VS match-controls input test (movement,
  attacks, CPU opponent, and pause/resume). Log:
  `build/native-runs/20260912-172946/game.log`.
- Graphical launch and View-menu widescreen preference verified; widescreen
  was left enabled for the next launch.
- 21:9 revision: all 27 tests passed again. Classic normal-menu input completed
  (`/tmp/melee-21-classic.log`). Installed View > Aspect Ratio dropdown tested
  in both directions; main-menu artwork retained identical dimensions in a
  centered 960 × 720 region of the 1680 × 720 window. Character selection and
  its icons were checked in both windowed and fullscreen presentation.
- Updated package: `dist/widescreen-21x9`. Previous installed prototype retained
  as `/Applications/Melee Native Widescreen 16x9 Backup.app`.

The installed package is ad-hoc signed, not notarized; older macOS versions
and other Macs have not been tested.
