#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <melee/gm/gm_18A1.h>
#include <melee/gm/gmmain_lib.h>
#include <melee/gm/gmregclear.h>
#include <melee/gm/gmvs.h>
#include <melee/gm/types.h>
#include <melee/pl/player.h>

#define CHECK(value)                                                          \
    do {                                                                      \
        if (!(value)) {                                                       \
            fprintf(stderr, "FAIL: %s\n", #value);                            \
            abort();                                                          \
        }                                                                     \
    } while (0)
#define CHECK_POINTER_ARG(fn)                                                 \
    _Static_assert(_Generic(&(fn), void (*)(intptr_t): 1, default: 0),        \
                   #fn " must preserve host pointers")
CHECK_POINTER_ARG(fn_8016C46C);
CHECK_POINTER_ARG(fn_8017EE40);
CHECK_POINTER_ARG(fn_8017E8A4);
CHECK_POINTER_ARG(fn_8018A364);

static struct StartMeleeRules rules;
struct StartMeleeRules* gm_GetStartMeleeRules(void)
{
    return &rules;
}
int fn_8017E318(void)
{
    return 1;
}
u32 gm_GetFrameCount(void)
{
    return 0;
}
struct gmm_x0_528_t* gmMainLib_8015CDC8(void)
{
    abort();
}
Gm_PKind Player_GetPlayerSlotType(s32 slot)
{
    return Gm_PKind_NA;
}
CharacterKind Player_GetPlayerCharacter(int slot)
{
    abort();
}

int main(void)
{
    static MatchEnd result;
    CHECK((uintptr_t) &result > UINT32_MAX);
    result._x448[2] = 1;
    fn_8017EE40((intptr_t) &result);
    // Existing source bitfield b3 adds 0x10 without overwriting adjacent bits.
    CHECK(result._x448[2] == 0x11);
    CHECK(result._x448[0] == 0 && result._x448[1] == 0);
    puts("PASS: Classic bonus handoff preserves a 64-bit MatchEnd pointer");
}
