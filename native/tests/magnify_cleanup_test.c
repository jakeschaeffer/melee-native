#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <melee/gr/ground.h>
#include <melee/if/ifmagnify.h>
#include <melee/if/types.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjplink.h>

#define CHECK(value)                                                          \
    do {                                                                      \
        if (!(value)) {                                                       \
            fprintf(stderr, "FAIL: %s\n", #value);                            \
            abort();                                                          \
        }                                                                     \
    } while (0)

static HSD_GObj objects[6];
static unsigned freed[6];

// ELF ASan retains the unused stage-color callback table from ifmagnify.c.
// Cleanup must never call those callbacks; keep that dependency explicit.
GXColor* Ground_801C0604(void)
{
    abort();
}
GXColor* Ground_801C0690(void)
{
    abort();
}
GXColor* Ground_801C0618(void)
{
    abort();
}
GXColor* Ground_801C0654(void)
{
    abort();
}
GXColor* Ground_801C06A4(void)
{
    abort();
}
GXColor* Ground_801C0668(void)
{
    abort();
}
GXColor* Ground_801C062C(void)
{
    abort();
}
GXColor* Ground_801C067C(void)
{
    abort();
}
GXColor* Ground_801C0640(void)
{
    abort();
}

void HSD_GObjFree(HSD_GObj* gobj)
{
    for (unsigned i = 0; i < 6; ++i) {
        if (gobj == &objects[i]) {
            CHECK(++freed[i] == 1);
            return;
        }
    }
    abort();
}

int main(void)
{
    CHECK(sizeof(void*) == 8);
    CHECK((uintptr_t) &ifMagnify_804A1DE0 > UINT32_MAX);
    memset(&ifMagnify_804A1DE0, 0, sizeof(ifMagnify_804A1DE0));
    ifMagnify_804A1DE0.x4 = 123;
    for (unsigned i = 0; i < 6; ++i) {
        ifMagnify_804A1DE0.player[i].gobj = &objects[i];
    }
    ifMagnify_802FC750();
    for (unsigned i = 0; i < 6; ++i) {
        CHECK(freed[i] == 1);
        CHECK(ifMagnify_804A1DE0.player[i].gobj == NULL);
    }
    CHECK(ifMagnify_804A1DE0.x4 == 123);
    ifMagnify_802FC750(); // Empty/repeated cleanup must not free twice.
    puts("PASS: magnifier cleanup preserves 64-bit pointers across all slots");
}
