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
  Post-fix Continue and extended Mute City checks are still in progress.
- Both pinned upstream commits are actual Git ancestors of the integration
  merge, verified by `ruby native/tools/check_upstreams.rb`.

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
