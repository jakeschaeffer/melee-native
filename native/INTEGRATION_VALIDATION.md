# Decomp integration validation — 2026-09-12

Scope: `integrate/decomp-2026-09-12`; exact upstream commits are recorded in
`upstream-lock.json`. This is an experimental integration, not a stable release.

## Local checks

Apple M5 Pro, macOS 26.6.2, Xcode 26.6, native arm64 / Metal.

- Full native app and test build passed in a separate build directory.
- All 27 component and disc-backed asset tests passed, including host
  `setjmp`/`longjmp` through the shared runtime header, display-aspect math,
  and Mute City pointer-width coverage.
- 21:9 VS match-controls script passed: normal menu route, character/stage
  selection, CPU fight, movement, attacks, and pause/resume.
- Classic reached a real CPU fight through the normal menus. A longer run
  exposed an existing Mute City live-item pointer truncation; it was fixed.
- Post-fix Native (4:3) Classic passed the complete loss, Continue, intro,
  and second-fight sequence through normal menus, without matrix overrides.
- A targeted 21:9 Mute City soak passed 3,600 gameplay frames (one minute)
  with two CPU fighters; the matrix hook verified the actual stage and fighters.
  A captured frame showed gameplay and an undistorted HUD, but also background
  geometry artifacts. Their origin has not been isolated; this is not a claim
  of stage-wide rendering correctness.
- Both pinned upstream commits are actual Git ancestors of the integration
  merge, verified by `ruby native/tools/check_upstreams.rb`.
- Packaging passed relocation/signature checks: four bundled libraries, no
  disc image or extracted assets. The local ad-hoc-signed package is in
  `dist/decomp-integration`; its manifest identifies the packaged commit.

Local evidence (ignored build output, not published game assets):

| Check | Run directory under `build/native-runs/` |
| --- | --- |
| VS controls, 21:9 | `20260912-174848` |
| Classic/Continue, Native (4:3), after pointer fix | `20260912-175602` |
| Mute City, 21:9, one-minute soak | `20260912-175844` |

Reproduce the targeted stage check with your own disc:

```sh
MELEE_MATRIX_TEST=1 MELEE_TEST_FORCE_STAGE=1 MELEE_TEST_STAGE=10 \
  MELEE_WIDESCREEN=1 MELEE_TEST_CAPTURE=0 \
  MELEE_TEST_APP=build/native-app/melee_mac.app/Contents/MacOS/melee_mac \
  ruby native/tools/run_input_test.rb native/tests/mute-city-soak.input 180 /path/to/owned-disc.ciso
```

## Limits

Script completion establishes scene/input progress, not visual correctness.
The pre-integration visual checks in `WIDESCREEN_PROTOTYPE.md` are historical,
not a substitute for rechecking every stage, effect, and camera after this merge.
No Linux gameplay, Intel Mac, older macOS, or byte-matching GameCube validation
has been performed here. Local Homebrew libraries target macOS 26.0 even though
the app deployment target is 15.5; this build does not establish 15.5 compatibility.

Public macOS/Linux CI must pass before promoting this branch. CI has no game
disc/assets, so it cannot run the disc-backed gameplay tests. Your existing
installed apps, dirty prototype checkout, fork `main`, and focused Classic PR
were not replaced by this integration.
