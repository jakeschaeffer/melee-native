# Native Classic round-win fix — 2026-09-12

The reported crash occurred after a Classic victory in the integration build,
in `ifMagnify_802FC750`. A 64-bit global address was truncated to `u32` before
dereferencing the player's magnifier object. The native regression test reproduced
the same AddressSanitizer failure before the fix and passed afterwards.

Following the complete transition exposed other console-layout assumptions:

- Result/bonus dispatch passed `MatchEnd*` through `int` arguments and casts.
- The bonus screen copied eight 32-bit scene words instead of host pointers.
- Two overlapping bonus-state views expanded differently on a 64-bit host.
- Cached bonus values overwrote text pointers; the score cache must follow all
  eight host-sized text handles.
- The blur callback used a console-offset overlay rather than `CameraBlurData`,
  and its constructor lacked an explicit native return value.
- Stock animation accesses used the console's `0x204` array offset, overwriting
  HUD pointers when the next match loaded additional fighters.

## Upstream compatibility

All layout and callback adaptations are guarded by `MELEE_NATIVE`. The original
console bodies remain available. Result arguments use `intptr_t`, which the
decomp's `src/MSL/stddef.h` defines as `signed int`; no new game-field names or
rewritten upstream history are required. A byte-matching GameCube build was not
performed, so this is not a claim of verified matching output.

## Regressions

Four asset-free, ASan/UBSan component tests exercise actual game code:

- `native_magnify_cleanup`: all six slots, high addresses, repeated cleanup.
- `native_classic_result`: pointer-width API checks and real Classic bonus writes.
- `native_regclear_layout`: scene pointers, shared views, all seven cached scores,
  repeated bonus-list updates, and the blur callback's actual host layout.
- `native_stock_layout`: every player's animation writes leave HUD pointers intact.

The disc-backed `classic-win.input` enters Classic through its normal menus.
With the explicit test environment below, a native hook triggers one opponent
blast-zone KO after five seconds of the first match. The real death, victory,
bonus screen, Start input, next-round intro and ten seconds of the second fight
must complete. This tests transitions; it is not an unassisted combat test or a
complete Classic playthrough. The hook requires both the environment option and
an input script, and is inactive in normal app launches.

```sh
MELEE_TEST_CLASSIC_WIN=1 MELEE_WIDESCREEN=1 MELEE_TEST_CAPTURE=0 \
  MELEE_TEST_APP=build/native-app/melee_mac.app/Contents/MacOS/melee_mac \
  ruby native/tools/run_input_test.rb native/tests/classic-win.input 210 /path/to/owned-disc.ciso
```

Repeat with `MELEE_WIDESCREEN=0` and the packaged executable. Also rerun normal
`match-controls.input` without the Classic test option. Local run records remain
under ignored `build/native-runs/`; no game assets or captures are published.

Initial verified 21:9 win/advance run: `20260912-182642`. The final component suite
passed all 31 tests on Apple M5 Pro / macOS 26.6.2. Package and follow-up gameplay
results are recorded in `INTEGRATION_VALIDATION.md`.
