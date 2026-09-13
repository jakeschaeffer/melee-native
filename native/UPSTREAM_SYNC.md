# Maintaining this integration fork

## Repository relationships

Keep `jakeschaeffer/melee-native`: GitHub already identifies its parent as
`jonrosner/melee-native` and its original source as `doldecomp/melee`. Creating
another decomp fork would not remove any integration conflicts. Preserve both
upstream histories in this fork and merge their updates explicitly.

`main` is our primary native development target. The integration was prepared
on `integrate/decomp-2026-09-12` and promoted without rewriting either upstream's
history. `fix/macos-classic-crashes` (upstream PR #2) remains separate: do not
add this integration to that focused crash-fix PR. The original local prototype
worktree is unchanged.

## Included snapshots

`upstream-lock.json` records the exact native and decomp commits integrated.
The decomp snapshot was 39 commits ahead of the common ancestor, while the
native fork had 12 independent commits. This is a real integration merge with
both parents, not a squash or an `ours`-strategy merge. All upstream commits
remain inspectable and attributable. Native CI checks their ancestry.

The separate prototype commit precedes the integration merge. It includes our
21:9 controls and shadow-pass restoration, on top of the earlier Classic fix.

## Resolution decisions

- Preserve host `jmp_buf` behind `MELEE_NATIVE`; adopt upstream's standard
  `setjmp(env)` / `longjmp(env, value)` call shape. Test through the actual
  shared `Runtime/Gecko_setjmp.h` header, not a console-layout imitation.
- Accept upstream's complete top-half projection matrix and blur color bounds
  fixes. Retain the opt-in on-screen widescreen projection hook, leaving real
  shadow projections unchanged.
- Accept typed CSS globals and model tables, replacing the port's equivalent
  global-overlay workaround. Keep native archive materialization of pointers.
- Accept upstream's fixed-width common fighter fields and `FallCommon`, actual
  `MatchEnd` type, renamed scene APIs, and size-based clearing/allocation.
- Retain native script-pointer lookup for packed stage parameters; adapt it to
  upstream's `void*` material API. Stage records still contain 32-bit source
  words on a 64-bit host.
- Retain native GX array submission, player-mapping access, animation table
  storage, expanded item dynamics, and stats-boundary workarounds where upstream
  still relies on original linker layout or GameCube-specific behavior.
- Share updated event declarations with the native asset schema. Field renames
  must also reach the decoder; changing only the game code is insufficient.
- Keep Mute City's live item handle pointer-sized on native builds. Classic
  testing exposed an existing 32-bit truncation when a racing car item despawned;
  the native layout test now checks this field.

## Working from a fresh clone

```sh
git clone https://github.com/jakeschaeffer/melee-native.git
cd melee-native
git switch main
git remote add native-upstream https://github.com/jonrosner/melee-native.git
git remote add decomp https://github.com/doldecomp/melee.git
git fetch native-upstream main
git fetch decomp master
ruby native/tools/check_upstreams.rb
```

In the original local checkout, the remote names differ: `fork` is Jake's repo,
`origin` is Jon's repo, and `decomp` is the decomp repo. Do not blindly replace
those URLs; use `git remote -v` first.

## Future syncs

1. Start from the latest validated integration commit in a **clean new branch
   and worktree**, not a dirty feature branch or an upstream PR branch.
2. Fetch both remotes and inspect each divergence with
   `git log --left-right --oneline HEAD...decomp/master` (and the native ref).
3. Merge one upstream at a time with `git merge --no-commit --no-ff <ref>`.
   Resolve semantically: do not blanket-select one side, squash upstream history,
   or use `git merge -s ours`. Keep host-layout guards where they are still needed.
4. Rebuild and run the tests below. Inspect warnings and archive schema changes,
   not just textual conflicts. Update the lock only to commits actually included.
5. Commit the merge, run the ancestry checker, and push the new integration branch
   for both native CI jobs. No automatic merge, release, or update to `main`.
6. Promote to your fork's `main` only after reviewing the diff and checks. Send
   small, independently useful fixes back to the appropriate upstream separately.

Native-only layout adaptations stay under `MELEE_NATIVE`; original console
layout paths remain available. Pointer-carrying result arguments use `intptr_t`,
which is still `signed int` in the decomp's 32-bit MSL headers. Preserve upstream
names and APIs when integrating changes; do not rename unidentified game fields.

The standard build/bootstrap commands remain in the root README. The original
decompilation instructions remain in `.github/UPSTREAM_README.md`.

## Regression gate

```sh
cmake --build build/native-app --target melee_mac all --parallel 4
ctest --test-dir build/native-app -L melee --output-on-failure
```

Use `native/tools/run_input_test.rb` with `MELEE_TEST_APP` pointing at this
branch's executable, `MELEE_TEST_CAPTURE=0`, and your locally owned disc path:

- `native/tests/match-controls.input`: VS CPU, movement, attacks, pause/resume.
- `native/tests/classic-start.input`: normal menus into a Classic fight.
- `native/tests/classic-continue.input`: lose, choose Continue, reach another fight.
- `native/tests/classic-win.input` with `MELEE_TEST_CLASSIC_WIN=1`: a test-only
  enemy KO followed by real victory/bonus cleanup and advancement to the next fight.
- `native/tests/mute-city-soak.input`: one minute of racing-car hazards, with
  the explicit matrix environment listed in `INTEGRATION_VALIDATION.md`.
- Exercise both `MELEE_WIDESCREEN=0` and `1`; manually inspect Native (4:3), 21:9,
  fullscreen, and menu/HUD proportions. Scripts do not prove visual fidelity.

Disc-backed tests only run locally when assets are present. Public CI contains
no disc or assets and cannot certify gameplay. macOS testing does not establish
Linux runtime behavior or byte-matching GameCube builds.

See `INTEGRATION_VALIDATION.md` for this snapshot's measured results and limits.
