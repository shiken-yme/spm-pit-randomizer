#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/npcdrv.h>

namespace mod {
    struct DanNPCData {
        s32 name;
        s32 odds;
        s32 spr;
        s32 min;
        s32 max;
    };

    struct DanLevelData {
        s32 lv1Limiter;
        s32 lv2Limiter;
        s32 lv3Limiter;
        s32 bonusMin;
        s32 bonusMax;
    };

    struct EnemyConfig {
        s32 name;
        s32 num;
        s32 pos;
    };

    struct FloorData {
        s32 enemyTypes;
        EnemyConfig Enemies[4];
    };

    DanNPCData ** danNpcGetPtr();
    DanLevelData * danLevelDataPtr(s32 level);
    s32 npcGetDanLv(s32 tribeId);

    EVT_DECLARE_USER_FUNC(evt_dan_read_data_new, -1)
    EVT_DECLARE_USER_FUNC(evt_dan_get_enemy_info_new, -1)

    void DanGen_Items(bool onRoomLoad);
    s32 DanGen_Items_SelectItem(s32 itemRarity, s32 * blocklist, s32 blocklistCnt, bool guaranteeSpecial);
    void DanGen_Enemies(s32 roomGens, bool depravity);
    void DanGen_Enemies_Apply();
}
