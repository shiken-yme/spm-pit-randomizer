#pragma once

#include <common.h>
#include <spm/npcdrv.h>
#include <evt_cmd.h>
#include <spm/item_data.h>
#include <lunatic/localize.h>
#include <mod.h>

namespace mod
{
    using namespace spm;
    using namespace spm::npcdrv;
    using namespace spm::item_data;

    #define RFC_SPECIAL_START 1000

    extern s32 RFCItems_Common_Size;
    extern s32 RFCItems_Uncommon_Size;
    extern s32 RFCItems_Rare_Size;
    extern s32 RFCItems_Legendary_Size;
    extern s32 RFCItems_Common[];
    extern s32 RFCItems_Uncommon[];
    extern s32 RFCItems_Rare[];
    extern s32 RFCItems_Legendary[];
    extern RFCItemData RFC_SpecialItems[];
    extern RFCColorDef RFC_Colors[];
    extern const char *RFCRarityNames[];

    EVT_DECLARE_USER_FUNC(RFCGetPtr, 1)
    EVT_DECLARE_USER_FUNC(RFCGetRarity, 2)
    EVT_DECLARE_USER_FUNC(RFCSetChestCol, 1)
    EVT_DECLARE_USER_FUNC(RFCGetChestKeyParams, 2)
    EVT_DECLARE_USER_FUNC(RFCAnalyzeSpecial, 2)
}
