#pragma once
#include <cmath>

namespace melee {
constexpr float nativeAspect = 4.f / 3.f;
constexpr float ultrawideAspect = 21.f / 9.f;
struct DisplayFit { float x = 1.f, y = 1.f; };
// Fractions of the surface occupied by a centered, exact 21:9 rectangle.
inline DisplayFit ultrawideFit(unsigned width, unsigned height) {
    if (!width || !height) return {};
    const float aspect = static_cast<float>(width) / height;
    return aspect > ultrawideAspect ? DisplayFit{ultrawideAspect / aspect, 1.f}
                                   : DisplayFit{1.f, aspect / ultrawideAspect};
}
inline DisplayFit projectionScale(bool enabled, int scene, unsigned width, unsigned height) {
    if (!enabled || scene != 2 || !width || !height) return {};
    const auto fit = ultrawideFit(width, height);
    return {fit.x * nativeAspect / ultrawideAspect, fit.y};
}
// Change only the presentation copy. HUD cameras retain their 4:3 safe area.
inline void widenProjection(float projection[4][4], DisplayFit scale) {
    if (!std::isfinite(scale.x) || !std::isfinite(scale.y) ||
        scale.x <= 0.f || scale.x > 1.f || scale.y <= 0.f || scale.y > 1.f) return;
    for (unsigned column = 0; column < 4; ++column) {
        projection[0][column] *= scale.x;
        projection[1][column] *= scale.y;
    }
}
}
