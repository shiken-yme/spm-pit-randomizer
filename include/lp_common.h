#pragma once
#include <common.h>
#include <wii/os.h>

namespace mod
{

    s32 round(f32 in);
    s32 clamp(s32 input, s32 min, s32 max);
    void lpAddAtk(s32 atk);
    void lpAddHp(s32 maxHp, s32 hp);
    void lpAddCrit(s32 rate, f32 mult);
    const char *npcTribeToName(s32 tribeId);

}