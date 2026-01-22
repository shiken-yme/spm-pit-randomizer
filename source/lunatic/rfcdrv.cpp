#include <common.h>
#include <evt_cmd.h>
#include <rfcdrv.h>
#include <mod.h>
#include <util.h>
#include <cutscene_helpers.h>
#include <evtpatch.h>
#include <tplpatch.h>

#include <spm/rel/aa1_01.h>
#include <spm/rel/mi4.h>
#include <spm/rel/relocatable_module.h>
#include <spm/temp_unk.h>
#include <spm/animdrv.h>
#include <spm/npcdrv.h>
#include <spm/bgdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/npc_ninja.h>
#include <spm/eff/eff_fire.h>
#include <spm/eff/eff_small_star.h>
#include <spm/eff/eff_spm_confetti.h>
#include <spm/eff/eff_zunbaba.h>
#include <spm/eff/eff_spm_recovery.h>
#include <spm/eff/eff_spm_spindash.h>
#include <spm/eff/eff_spm_hit.h>
#include <spm/evt_cam.h>
#include <spm/evt_case.h>
#include <spm/evt_dimen.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_frame.h>
#include <spm/evt_guide.h>
#include <spm/evt_hit.h>
#include <spm/evt_img.h>
#include <spm/evt_item.h>
#include <spm/evt_map.h>
#include <spm/evt_mario.h>
#include <spm/evt_msg.h>
#include <spm/evt_mobj.h>
#include <spm/evt_npc.h>
#include <spm/evt_offscreen.h>
#include <spm/evt_paper.h>
#include <spm/evt_pouch.h>
#include <spm/evt_shop.h>
#include <spm/evt_snd.h>
#include <spm/evt_seq.h>
#include <spm/evt_sub.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_door.h>
#include <spm/fontmgr.h>
#include <spm/framedrv.h>
#include <spm/hitdrv.h>
#include <spm/hud.h>
#include <spm/itemdrv.h>
#include <spm/item_data.h>
#include <spm/lz_embedded.h>
#include <spm/map_data.h>
#include <spm/mapdrv.h>
#include <spm/mario.h>
#include <spm/mario_fairy.h>
#include <spm/mario_motion.h>
#include <spm/mario_pouch.h>
#include <spm/mario_status.h>
#include <spm/mario_hit.h>
#include <spm/mobjdrv.h>
#include <spm/mot_damage.h>
#include <spm/mot_fairy_mario.h>
#include <spm/memory.h>
#include <spm/msgdrv.h>
#include <spm/npc_tile.h>
#include <spm/parse.h>
#include <spm/pausewin.h>
#include <spm/seq_mapchange.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <spm/seq_title.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <spm/npc_dimeen_l.h>
#include <spm/winmgr.h>
#include <spm/rel/dan.h>
#include <spm/rel/machi.h>
#include <wii/os/OSError.h>
#include <wii/cx.h>
#include <wii/gx.h>
#include <wii/tpl.h>
#include <wii/wpad.h>
#include <msl/math.h>
#include <msl/stdio.h>
#include <msl/string.h>
#include <cstdio>

namespace mod
{
    using namespace spm;
    using namespace spm::npcdrv;
    using namespace spm::item_data;

    s32 RFCItems_Common[] = {
        ITEM_ID_USE_HONOO_SAKURETU,
        ITEM_ID_USE_STAR_MEDAL,
        ITEM_ID_USE_KOURA_DE_PON,
        ITEM_ID_USE_KOORI_NO_IBUKI,
        ITEM_ID_USE_BARIA_FRAME,
        ITEM_ID_COOK_BOMB_EGG,
        ITEM_ID_USE_POW_BLOCK,
        ITEM_ID_USE_TOROPICO_MANGO,
        ITEM_ID_USE_MILD_CACAO,
        ITEM_ID_USE_ROW_PASTA,
        ITEM_ID_USE_INGREDIENTS_OF_SWEET,
        ITEM_ID_USE_TUKUSHINBO,
        ITEM_ID_USE_HERB,
        ITEM_ID_USE_SHINABITA_KINOKO,
        ITEM_ID_COOK_FAIL_COOKING1,
        ITEM_ID_COOK_FAIL_COOKING2,
        ITEM_ID_COOK_KAME_TEA,
        ITEM_ID_COOK_HERB_TEA,
        ITEM_ID_COOK_TRIAL_PAN/*,
        (SPIRIT_1 + RFC_SPECIAL_START),
        (SOUL_1 + RFC_SPECIAL_START),
        (AUSPICE_1 + RFC_SPECIAL_START)*/};

    s32 RFCItems_Uncommon[] = {
        ITEM_ID_COOK_HUNNY_KINOKO,
        ITEM_ID_COOK_HOT_COCOA,
        ITEM_ID_COOK_KINOKO_FRY,
        ITEM_ID_USE_BIG_EGG,
        ITEM_ID_USE_NANIGA_OKORUKANA,
        ITEM_ID_USE_KAMINARI_DOKKAN,
        ITEM_ID_USE_TUYOTUYO_DRINK,
        ITEM_ID_COOK_HOTDOG,
        ITEM_ID_COOK_HANAJIRU_SYRUP,
        ITEM_ID_USE_KINKYU_KINOKO,
        ITEM_ID_USE_POWERFUL_MEET,
        ITEM_ID_USE_PRIMITIVENUT,
        ITEM_ID_USE_SHINABITA_KINOKO,
        ITEM_ID_COOK_TRIAL_PAN,
        (VOUCHER_CAKE + RFC_SPECIAL_START),
        (VOUCHER_THUNDER + RFC_SPECIAL_START)/*,
        (SPIRIT_2 + RFC_SPECIAL_START),
        (SOUL_2 + RFC_SPECIAL_START),
        (AEGIS_1 + RFC_SPECIAL_START)*/};

    s32 RFCItems_Rare[] = {
        ITEM_ID_COOK_KINOKO_HOILE_FRY,
        ITEM_ID_USE_KIRAKIRA_OTOSHI,
        ITEM_ID_COOK_DINNER,
        ITEM_ID_COOK_HANA_DANGO,
        ITEM_ID_COOK_MANGO_PUDDING,
        ITEM_ID_COOK_GOLD_CHOKO,
        ITEM_ID_USE_GOLD_MEDAL,
        ITEM_ID_COOK_LOVE_NOODLE,
        ITEM_ID_COOK_EMERGENCY_MEAL,
        ITEM_ID_COOK_FRUITS_HUMBURG,
        ITEM_ID_USE_KINKYU_KINOKO,
        ITEM_ID_COOK_PEACH_TART,
        ITEM_ID_COOK_MIX_SHAKE,
        ITEM_ID_COOK_MOUSSE_CAKE,
        ITEM_ID_COOK_CHOCOLA_CAKE,
        ITEM_ID_USE_SHINABITA_KINOKO,
        ITEM_ID_COOK_TRIAL_PAN,
        (VOUCHER_STELLAR + RFC_SPECIAL_START),
        (VOUCHER_JUDGEMENT + RFC_SPECIAL_START)/*,
        (SPIRIT_3 + RFC_SPECIAL_START),
        (SOUL_3 + RFC_SPECIAL_START),
        (AUSPICE_2 + RFC_SPECIAL_START)*/};

    s32 RFCItems_Legendary[] = {
        ITEM_ID_USE_ULTRA_DRINK,
        ITEM_ID_COOK_ICHIGO_RABBIT,
        ITEM_ID_COOK_SNOW_RABBIT,
        ITEM_ID_COOK_TRIAL_PAN/*,
        (VOUCHER_RED + RFC_SPECIAL_START),
        (VOUCHER_ORANGE + RFC_SPECIAL_START),
        (VOUCHER_YELLOW + RFC_SPECIAL_START),
        (VOUCHER_GREEN + RFC_SPECIAL_START),
        (VOUCHER_CYAN + RFC_SPECIAL_START),
        (VOUCHER_BLUE + RFC_SPECIAL_START),
        (VOUCHER_PURPLE + RFC_SPECIAL_START),
        (SPIRIT_4 + RFC_SPECIAL_START),
        (SOUL_4 + RFC_SPECIAL_START),
        (AEGIS_2 + RFC_SPECIAL_START)*/};

    RFCItemData RFC_SpecialItems[] = {
        {ICON_VOUCHER_CAKE, cakeVName, cakeVDesc, nullptr, nullptr},
        {ICON_VOUCHER_THUNDER, thunderVName, thunderVDesc, nullptr, nullptr},
        {ICON_VOUCHER_STELLAR, stellarVName, stellarVDesc, nullptr, nullptr},
        {ICON_VOUCHER_JUDGEMENT, judgementVName, judgementVDesc, nullptr, nullptr},
        {ICON_VOUCHER_RED, redVName, redVDesc, nullptr, nullptr},
        {ICON_VOUCHER_ORANGE, orangeVName, orangeVDesc, nullptr, nullptr},
        {ICON_VOUCHER_YELLOW, yellowVName, yellowVDesc, nullptr, nullptr},
        {ICON_VOUCHER_GREEN, greenVName, greenVDesc, nullptr, nullptr},
        {ICON_VOUCHER_CYAN, cyanVName, cyanVDesc, nullptr, nullptr},
        {ICON_VOUCHER_BLUE, blueVName, blueVDesc, nullptr, nullptr},
        {ICON_VOUCHER_PURPLE, purpleVName, purpleVDesc, nullptr, nullptr},
        {ICON_VOUCHER_WHITE, whiteVName, whiteVDesc, nullptr, nullptr},
        {ICON_VOUCHER_BLACK, blackVName, blackVDesc, nullptr, nullptr}, // kek
        {ICON_SOUL_1, soul1Name, soul1Desc, nullptr, nullptr},
        {ICON_SOUL_2, soul2Name, soul2Desc, nullptr, nullptr},
        {ICON_SOUL_3, soul3Name, soul3Desc, nullptr, nullptr},
        {ICON_SOUL_4, soul4Name, soul4Desc, nullptr, nullptr},
        {ICON_SPIRIT_1, spirit1Name, spirit1Desc, nullptr, nullptr},
        {ICON_SPIRIT_2, spirit2Name, spirit2Desc, nullptr, nullptr},
        {ICON_SPIRIT_3, spirit3Name, spirit3Desc, nullptr, nullptr},
        {ICON_SPIRIT_4, spirit4Name, spirit4Desc, nullptr, nullptr},
        {ICON_AEGIS_1, aegis1Name, aegis1Desc, nullptr, nullptr},
        {ICON_AEGIS_2, aegis2Name, aegis2Desc, nullptr, nullptr},
        {ICON_AUSPICE_1, auspice1Name, auspice1Desc, nullptr, nullptr},
        {ICON_AUSPICE_2, auspice2Name, auspice2Desc, nullptr, nullptr}};

    RFCColorDef RFC_Colors[] = {
        {{10, 10, 10, 255}, {0, 0, 0, 255}},          // Common
        {{128, 144, 160, 255}, {217, 217, 217, 255}}, // Uncommon -- Iron
        {{173, 150, 75, 255}, {250, 214, 74, 255}},   // Rare -- Gold
        {{63, 202, 179, 255}, {74, 237, 210, 255}}    // Legendary -- Diamond
    };

    const char *RFC_ChestNames[3] = {"MOBJ_dan_u_big", "MOBJ_dan_r_big", "MOBJ_dan_l_big"};

    const char *RFCRarityNames[4] = {"Common", "Uncommon", "Rare", "Legendary"};

    s32 RFCItems_Common_Size = sizeof(RFCItems_Common) / 4;
    s32 RFCItems_Uncommon_Size = sizeof(RFCItems_Uncommon) / 4;
    s32 RFCItems_Rare_Size = sizeof(RFCItems_Rare) / 4;
    s32 RFCItems_Legendary_Size = sizeof(RFCItems_Legendary) / 4;
    s32 RFC_SpecialItems_Size = sizeof(RFC_SpecialItems) / sizeof(RFCItemData);

    s32 RFCGetPtr(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], (s32)&Lunatic->RFC.rfcItems);
        return 2;
    }

    s32 RFCGetRarity(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], Lunatic->RFC.chestRarity);
        evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)RFCRarityNames[Lunatic->RFC.chestRarity]);
        return 2;
    }

    s32 RFCSetChestCol(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 rarity = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        if (rarity == 0)
            return 2;
        mobjdrv::MobjEntry *mobj = mobjdrv::mobjNameToPtr("box");
        s32 animpose = animdrv::animPoseEntry(RFC_ChestNames[rarity-1], 0);
        s32 prevAnimPoseId = mobj->animPoseId;
        mobj->animPoseId = animpose;
        animdrv::animPoseRelease(prevAnimPoseId);
        mobjdrv::mobjHitEntry(mobj, 6);
        mobjdrv::mobjCalcMtx(mobj);
        return 2;
    }

    s32 RFCGetChestKeyParams(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], Lunatic->RFC.chestKeys); // chest keys required
        evtmgr_cmd::evtSetValue(evtEntry, args[1], 0);                      // chest keys owned
        return 2;
    }

    s32 RFCAnalyzeSpecial(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 idx = evtmgr_cmd::evtGetValue(evtEntry, args[0]); // CWSelect item idx
        if (idx < 0)
            return 2;
        s32 trueIdx = customwin::GlobalCW->Select[customwin::GlobalCW->activeSelect]->Descs[idx].iconId - ICON_VOUCHER_CAKE - TPLPATCH_ICON_REDIRECT; // converts LPIcon to LPCustomItem index
        if (RFC_SpecialItems[trueIdx].useFunc != nullptr)
            (RFC_SpecialItems[trueIdx].useFunc)();
        if (RFC_SpecialItems[trueIdx].useMsg != nullptr)
            evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)RFC_SpecialItems[trueIdx].useMsg);
        else
            evtmgr_cmd::evtSetValue(evtEntry, args[1], -1);
        return 2;
    }
}
