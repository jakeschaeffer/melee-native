#include "include/melee_display.h"
#include "display_math.hpp"
#include <SDL3/SDL.h>
#include <dolphin/gx/GXAurora.h>
#include <cstdio>
#include <imgui.h>
#ifdef __APPLE__
#include "platform_launcher.h"
#endif

namespace {
SDL_Window* game_window;
bool enabled;
int current_scene = -1;
int requested_policy = -1;
}

extern "C" void MeleeNativeDisplayInitialize(void* window, int widescreen) {
    game_window = static_cast<SDL_Window*>(window);
    enabled = widescreen != 0;
    SDL_SetWindowTitle(game_window, "Melee Native — Widescreen Prototype");
    MeleeNativeDisplayUpdate();
}
extern "C" int MeleeNativeWidescreenEnabled(void) { return enabled; }
extern "C" void MeleeNativeDisplayScene(int scene) {
    current_scene = scene;
    MeleeNativeDisplayUpdate();
}
extern "C" void MeleeNativeDisplayUpdate(void) {
    if (!game_window) return;
    int width = 0, height = 0;
    SDL_GetWindowSizeInPixels(game_window, &width, &height);
    const bool wide = enabled && current_scene == 2;
    const auto policy = wide ? AURORA_VIEWPORT_STRETCH : AURORA_VIEWPORT_FIT;
    if (requested_policy != policy) {
        // Aurora applies this at a frame boundary and resizes its render target.
        AuroraSetViewportPolicy(policy);
        requested_policy = policy;
        std::fprintf(stderr, "[display] scene=%d mode=%s surface=%dx%d\n",
                     current_scene, wide ? "21:9" : "original-4:3", width, height);
    }
}
extern "C" void MeleeNativeSetWidescreen(int value) {
    enabled = value != 0;
    if (game_window && !(SDL_GetWindowFlags(game_window) & SDL_WINDOW_FULLSCREEN))
        SDL_SetWindowSize(game_window, enabled ? 1680 : 960, 720);
#ifdef __APPLE__
    MeleeSaveWidescreenPreference(enabled);
#endif
    std::fprintf(stderr, "[display] widescreen preference=%d\n", enabled);
    MeleeNativeDisplayUpdate();
}
extern "C" void MeleeNativeToggleFullscreen(void) {
    if (game_window)
        SDL_SetWindowFullscreen(game_window, !(SDL_GetWindowFlags(game_window) & SDL_WINDOW_FULLSCREEN));
}
extern "C" void MeleeNativeToggleWidescreen(void) {
    MeleeNativeSetWidescreen(!enabled);
}
extern "C" void MeleeNativeAdjustProjection(float projection[4][4]) {
    if (!enabled || current_scene != 2) return;
    unsigned width = 0, height = 0;
    AuroraGetRenderSize(&width, &height);
    const auto scale = melee::projectionScale(enabled, current_scene, width, height);
    static melee::DisplayFit last_scale;
    if (scale.x != last_scale.x || scale.y != last_scale.y) {
        std::fprintf(stderr, "[display] projection scale=%.6f,%.6f render=%ux%u\n", scale.x, scale.y, width, height);
        last_scale = scale;
    }
    melee::widenProjection(projection, scale);
}
extern "C" void MeleeNativeDisplayMask(void) {
    if (!enabled || current_scene != 2) return;
    unsigned width = 0, height = 0;
    AuroraGetRenderSize(&width, &height);
    const auto fit = melee::ultrawideFit(width, height);
    const auto size = ImGui::GetIO().DisplaySize;
    const float x = size.x * (1.f - fit.x) * .5f;
    const float y = size.y * (1.f - fit.y) * .5f;
    auto* draw = ImGui::GetForegroundDrawList();
    // Cover unused surface after GX rendering, including screen-space FX.
    if (x > 0.f) {
        draw->AddRectFilled(ImVec2(0, 0), ImVec2(x, size.y), IM_COL32(0,0,0,255));
        draw->AddRectFilled(ImVec2(size.x - x, 0), size, IM_COL32(0,0,0,255));
    }
    if (y > 0.f) {
        draw->AddRectFilled(ImVec2(0, 0), ImVec2(size.x, y), IM_COL32(0,0,0,255));
        draw->AddRectFilled(ImVec2(0, size.y - y), size, IM_COL32(0,0,0,255));
    }
}
