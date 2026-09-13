// Exercise the actual private scene loader and blur callback, not a copied
// layout.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <melee/gm/gmregclear.c>

#define CHECK(value)                                                          \
    do {                                                                      \
        if (!(value)) {                                                       \
            fprintf(stderr, "FAIL: %s\n", #value);                            \
            abort();                                                          \
        }                                                                     \
    } while (0)

static HSD_ImageDesc image;
static HSD_GObj blur;
static unsigned copies;
static HSD_Text texts[8];
int fn_80171A88(void)
{
    return 3;
}
MatchEnd* gm_8016B774(void)
{
    return NULL;
}
int fn_8016F39C(HSD_Text** rows, void* result, u8 count, u16 start, u8 mask,
                u8 player)
{
    CHECK(count == 7);
    for (unsigned i = 0; i < 7; ++i) {
        CHECK(rows[i] == &texts[i + 1]);
    }
    return 7;
}
int fn_8016F548(void* result, u16 start, u8 mask, u8 player)
{
    return start < 7 ? start : -1;
}
int fn_8016FAD4(MatchEnd* result, int index, int mask, u8 player)
{
    return 5000 + index;
}
int fn_8016FFD4(MatchEnd* result, int mask, u8 player)
{
    return 10001;
}
s32 HSD_SisLib_803A70A0(HSD_Text* text, s32 index, char* format, ...)
{
    CHECK(text == &texts[0]);
    return 0;
}
void lb_800122C8(HSD_ImageDesc* desc, u16 x, u16 y, bool clear)
{
    CHECK(desc == &image && x == 0 && y == 0 && clear);
    ++copies;
}
void lb_800138D8(HSD_GObj* gobj, s8 mode)
{
    CHECK(gobj == &blur);
}

int main(void)
{
    static HSD_Joint joint;
    DynamicModelDesc model = { .joint = &joint };
    DynamicModelDesc* models[] = { &model, NULL };
    struct SceneCameraDesc camera = { .desc = (HSD_CObjDesc*) &joint };
    LightList* lights[] = { NULL };
    SceneDesc scene = { .models = models,
                        .cameras = &camera,
                        .lights = lights };
    struct lbl_80472D28_t state = { 0 };
    CHECK((uintptr_t) &joint > UINT32_MAX);
    fn_80180630_LoadScene(&state, &scene);
    CHECK(state.x4C.joint == &joint);
    CHECK(state.x5C == lights && state.x60 == camera.desc);
    fn_8017FA1C_arg* view = &state;
    view->x6C = (HSD_Text*) &joint;
    view->xD0 = 123;
    CHECK(state.x6C == (HSD_Text*) &joint && state.xD0 == 123);
    CHECK(offsetof(struct lbl_80472D28_t, x28) ==
          offsetof(struct lbl_80472D28_t, x4) + 9 * sizeof(void*));

    HSD_Text** rows = &state.x84;
    for (unsigned i = 0; i < 8; ++i) {
        rows[i] = &texts[i];
    }
    fn_8017F47C(rows, 0);
    fn_8017F47C(rows, 0); // Repainting must not read numbers as text pointers.
    for (unsigned i = 0; i < 8; ++i) {
        CHECK(rows[i] == &texts[i]);
    }
    for (unsigned i = 0; i < 7; ++i) {
        CHECK(((s32*) (rows + 8))[i] == 5000 + i);
    }
    CHECK(state.xC0 == 0);

    struct CameraBlurData data = { .efb_copy = &image };
    blur.user_data = &data;
    lbl_80472D28.x2C = &blur;
    lbl_80472D28.x110 = 20;
    fn_8017FE54(&blur);
    CHECK(copies == 1 && data.efb_copy == &image);
    CHECK(data.tint_factor > 0.27f && data.tint_factor < 0.28f);
    puts("PASS: bonus scene, shared views and blur callback use host layouts");
}
