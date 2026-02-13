#pragma once

#include <common.h>
#include <spm/npcdrv.h>
#include <evt_cmd.h>
#include <spm/item_data.h>
#include <lunatic/localize.h>

namespace mod
{
    using namespace spm;
    using namespace spm::npcdrv;
    using namespace spm::item_data;

    /*
        Vouchers
    */

    #define VOUCHER_MAX 8
    
    enum VoucherState : s32
    {
        V_INACTIVE,
        V_ACTIVE,
        V_TORN
    };

    void VoucherCallAction(s32 itemId);

    VoucherState VoucherGetStateById(s32 itemId, s32 *idx);
    
    EVT_DECLARE_USER_FUNC(EvtVoucherCallAction, 1)
    EVT_DECLARE_USER_FUNC(ThunderVoucherIncrementCtr, 1)

    struct VCakeWork
    {
        u8 rooms;
    };

    struct VThunderWork
    {
        u8 enemies;
        s32 critRateBonus;
        f32 critMultBonus;
        s32 atkBonus;
    };

    struct VStellarWork
    {
        bool placeholder;
    };

    struct VJudgementWork
    {
        bool placeholder;
    };

    /*
        RFC Driver
    */
    
    #define RFC_SPECIAL_START 1000

    extern s32 RFCItems_Common_Size;
    extern s32 RFCItems_Uncommon_Size;
    extern s32 RFCItems_Rare_Size;
    extern s32 RFCItems_Legendary_Size;
    extern s32 RFCItems_Common[];
    extern s32 RFCItems_Uncommon[];
    extern s32 RFCItems_Rare[];
    extern s32 RFCItems_Legendary[];
    extern const char *RFCRarityNames[];

    void *RFCSpecialGetPtr();
    void *RFCColorsGetPtr();
    void RFCDRVPatches();

    EVT_DECLARE_USER_FUNC(RFCGetPtr, 1)
    EVT_DECLARE_USER_FUNC(RFCGetRarity, 2)
    EVT_DECLARE_USER_FUNC(RFCGetChestKeyParams, 2)
    EVT_DECLARE_USER_FUNC(RFCSetChestKeys, 1)
    EVT_DECLARE_USER_FUNC(RFCBakudan, 0)
    EVT_DECLARE_USER_FUNC(RFCAnalyzeSpecial, 2)
    EVT_DECLARE_USER_FUNC(RFCGenerate, 2)
    EVT_DECLARE_USER_FUNC(RFCReroll, 0)
}
