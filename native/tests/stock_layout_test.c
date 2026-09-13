#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <melee/if/ifstock.c>

#define CHECK(value)                                                          \
    do {                                                                      \
        if (!(value)) {                                                       \
            fprintf(stderr, "FAIL: %s\n", #value);                            \
            abort();                                                          \
        }                                                                     \
    } while (0)

int main(void)
{
    struct ifStock_804A1378* stock = &ifStock_804A1378;
    unsigned char before[sizeof(stock->player)];
    memset(stock, 0, sizeof(*stock));
    memset(stock->player, 0xA5, sizeof(stock->player));
    memcpy(before, stock->player, sizeof(before));
    for (unsigned slot = 0; slot < 6; ++slot) {
        struct ifStock_804A1378_x204* data =
            ifStock_802F8298_player_data(slot);
        CHECK(data == &stock->x204[slot]);
        CHECK(ifStock_802F8298_data_in(unused, slot) == data);
        data->flag = 1;
        memset(data->anim, 10, sizeof(data->anim));
    }
    CHECK(memcmp(before, stock->player, sizeof(before)) == 0);
    CHECK(sizeof(struct IfStockData) == sizeof(stock->x204[0]));
    CHECK(offsetof(struct IfStockData, anim) ==
          offsetof(struct ifStock_804A1378_x204, steal));
    puts("PASS: stock animation writes cannot overlap native HUD pointers");
}
