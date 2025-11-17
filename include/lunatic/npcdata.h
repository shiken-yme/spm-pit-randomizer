#pragma once
#include <common.h>
#include <spm/npcdrv.h>

namespace mod
{
    // npcdata.h is a custom header to help streamline the custom Pit's enemy generation process.
    // It contains data on NPCs, the scaling "levels," and difficulty settings.


    // Obtained from
    struct DanNPCData
    {
        s32 name;
        s32 odds;
        s32 spr;
        s32 min;
        s32 max;
    };

    DanNPCData **danNpcGetPtr();

    //
    struct DanLevelData
    {
        s32 lv1Limiter;
        s32 lv2Limiter;
        s32 lv3Limiter;
        s32 bonusMin;
        s32 bonusMax;
    };

    DanLevelData *danLevelDataPtr(s32 level);
    spm::npcdrv::NPCTribeId * getlv1Tribes();
    spm::npcdrv::NPCTribeId * getlv2Tribes();
    spm::npcdrv::NPCTribeId * getlv3Tribes();
    spm::npcdrv::NPCTribeId * getlv4Tribes();
}