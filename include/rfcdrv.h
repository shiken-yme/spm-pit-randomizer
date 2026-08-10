#pragma once

#include <common.h>
#include <spm/npcdrv.h>
#include <evt_cmd.h>
#include <lp_common.h>
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

    #define VOUCHER_ALPHA_BASE 180

    #define VOUCHER_NUM RANGE(VOUCHER_CAKE, VOUCHER_BLACK)
    
    enum VoucherState : s32
    {
        V_INACTIVE,
        V_ACTIVE,
        V_TORN
    };

    void VoucherProc(s32 itemId);

    VoucherState VoucherGetStateById(s32 itemId);
    
    EVT_DECLARE_USER_FUNC(EvtVoucherProc, 1)
    EVT_DECLARE_USER_FUNC(ThunderVoucherIncrementCtr, 1)

    struct VCakeWork
    {
        s32 hpGain;
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

    struct VRedWork
    {
        f32 cmGain;
        s32 crGain;
    };

    struct VOrangeWork
    {
        bool placeholder;
    };

    struct VYellowWork
    {
        bool placeholder;
    };

    struct VGreenWork
    {
        bool placeholder;
    };

    struct VCyanWork
    {
        bool placeholder;
    };

    struct VBlueWork
    {
        bool placeholder;
    };

    struct VPurpleWork
    {
        bool placeholder;
    };

    struct VWhiteWork
    {
        bool placeholder;
    };

    struct VBlackWork
    {
        bool placeholder;
    };

    s32 VoucherGetTearChance(s32 baseChance);

    bool VoucherChkTorn(s32 itemId);

    EVT_DECLARE_USER_FUNC(EvtVoucherChkTorn, 2)

    extern s32 VoucherTearChances[];
    extern s32 VoucherGuaranteeTrigs[];

    /*
        RFC Driver
    */
    
    #define RFC_SPECIAL_START 1000

    #define RFC_ITEM(idx) \
        (idx + RFC_SPECIAL_START)

    extern s32 RFCItems_Common_Size;
    extern s32 RFCItems_Uncommon_Size;
    extern s32 RFCItems_Rare_Size;
    extern s32 RFCItems_Legendary_Size;
    extern s32 RFCItems_Common[];
    extern s32 RFCItems_Uncommon[];
    extern s32 RFCItems_Rare[];
    extern s32 RFCItems_Legendary[];
    extern s32 RFCItems_Artifacts[];
    extern const char *RFCRarityNames[];

    void *RFCSpecialGetPtr();
    void *RFCColorsGetPtr();
    void RFCDRVPatches();

    EVT_DECLARE_USER_FUNC(RFCGetPtr, 1)
    EVT_DECLARE_USER_FUNC(RFCGetRarity, 2)
    EVT_DECLARE_USER_FUNC(RFCGetChestKeyParams, 3)
    EVT_DECLARE_USER_FUNC(RFCSetChestKeys, 1)
    EVT_DECLARE_USER_FUNC(RFCBakudan, 0)
    EVT_DECLARE_USER_FUNC(RFCProcEffect, 1)
    EVT_DECLARE_USER_FUNC(RFCAnalyzeSpecial, 2)
    EVT_DECLARE_USER_FUNC(RFCGenerate, 2)
    EVT_DECLARE_USER_FUNC(RFCReroll, 0)
}
