#include "display_math.hpp"
#include <cassert>
#include <limits>

static bool close(float a, float b) { return std::abs(a - b) < 1e-5f; }
int main() {
    const unsigned sizes[][2] = {{1680,720}, {1920,1080}, {3440,1440}, {960,720}, {600,900}};
    for (const auto& size : sizes) {
        const unsigned w = size[0], h = size[1];
        const auto fit = melee::ultrawideFit(w, h);
        assert(close(w * fit.x / (h * fit.y), melee::ultrawideAspect));
        const auto scale = melee::projectionScale(true, 2, w, h);
        assert(close(w * scale.x / (h * scale.y), melee::nativeAspect));
        const int menus[] = {-1, 0, 1, 8, 9, 32, 34};
        for (int scene : menus) {
            const auto menu = melee::projectionScale(true, scene, w, h);
            assert(menu.x == 1.f && menu.y == 1.f);
        }
        const auto native = melee::projectionScale(false, 2, w, h);
        assert(native.x == 1.f && native.y == 1.f);
    }
    const auto wide = melee::projectionScale(true, 2, 1680, 720);
    assert(close(wide.x, 4.f / 7.f) && close(wide.y, 1.f));
    const auto narrow = melee::projectionScale(true, 2, 1920, 1080);
    assert(close(narrow.x, 4.f / 7.f) && close(narrow.y, 16.f / 21.f));
    assert(melee::projectionScale(true, 2, 0, 0).x == 1.f);
    assert(melee::projectionScale(true, 2, 1920, 0).y == 1.f);
    // Every X coefficient is corrected (including off-center translations),
    // leaving Y, depth, and perspective divide untouched. Input is a copy.
    float p[4][4] = {{2, 0, .2f, 1}, {0, 3, 0, 2}, {0, 0, 4, 3}, {0, 0, -1, 0}};
    melee::widenProjection(p, {.5f, .75f});
    assert(p[0][0] == 1.f && p[0][3] == .5f);
    assert(p[1][1] == 2.25f && p[1][3] == 1.5f);
    assert(p[2][2] == 4 && p[3][2] == -1);
    melee::widenProjection(p, {std::numeric_limits<float>::quiet_NaN(), 1.f});
    melee::widenProjection(p, {0.f, 1.f});
    melee::widenProjection(p, {1.f, 2.f});
    assert(p[0][0] == 1.f && p[1][1] == 2.25f);
}
