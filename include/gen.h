#pragma once

#include <common.h>
#include <spm/npcdrv.h>
#include <evt_cmd.h>

namespace mod
{
    struct DanNPCData
    {
        s32 name;
        s32 odds;
        s32 spr;
        s32 min;
        s32 max;
    };

    DanNPCData **danNpcGetPtr();

    struct DanLevelData
    {
        s32 lv1Limiter;
        s32 lv2Limiter;
        s32 lv3Limiter;
        s32 bonusMin;
        s32 bonusMax;
    };

    struct EnemyConfig
    {
        s32 name;
        s32 num;
        s32 pos;
    };

    struct FloorData
    {
        s32 enemyTypes;
        EnemyConfig Enemies[4];
    };

    DanLevelData *danLevelDataPtr(s32 level);
    spm::npcdrv::NPCTribeId *getlv1Tribes();
    spm::npcdrv::NPCTribeId *getlv2Tribes();
    spm::npcdrv::NPCTribeId *getlv3Tribes();
    spm::npcdrv::NPCTribeId *getlv4Tribes();

    EVT_DECLARE_USER_FUNC(evt_dan_read_data_new, -1)
    EVT_DECLARE_USER_FUNC(evt_dan_get_enemy_info_new, -1)

    void DanGen_Enemies(s32 roomGens, bool depravity);
    void DanGen_Enemies_Apply();

    extern s32 rfcItems[4];
}
