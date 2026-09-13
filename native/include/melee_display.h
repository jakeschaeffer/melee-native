#ifndef MELEE_NATIVE_DISPLAY_H
#define MELEE_NATIVE_DISPLAY_H
#ifdef __cplusplus
extern "C" {
#endif

void MeleeNativeDisplayInitialize(void* window, int widescreen);
void MeleeNativeDisplayScene(int scene);
void MeleeNativeDisplayUpdate(void);
int MeleeNativeWidescreenEnabled(void);
void MeleeNativeSetWidescreen(int enabled);
void MeleeNativeDisplayMask(void);
void MeleeNativeToggleWidescreen(void);
void MeleeNativeToggleFullscreen(void);
void MeleeNativeAdjustProjection(float projection[4][4]);

#ifdef __cplusplus
}
#endif
#endif
