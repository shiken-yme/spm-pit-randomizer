#include <common.h>
#include <lp_common.h>
#include <evt_cmd.h>
#include <rfcdrv.h>
#include <mod.h>
#include <util.h>
#include <cutscene_helpers.h>
#include <evtpatch.h>
#include <tplpatch.h>

#include "globalop.h"
#include "effpatch.h"

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
#include <spm/eff/eff_pansy_kirakira.h>
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
    /*
        This file contains code for Rest Floor Chests & the special items contained with them
        RFC is powered heavily by CustomWin and IconPatch. I'm so glad that developing those libraries is paying off now!
    */

    using namespace spm;
    using namespace spm::npcdrv;
    using namespace spm::item_data;

    s32 RFCItems_Common[] = {
        ITEM_ID_USE_STAR_MEDAL,
        ITEM_ID_USE_KOURA_DE_PON,
        ITEM_ID_USE_KOORI_NO_IBUKI,
        ITEM_ID_USE_BARIA_FRAME,
        ITEM_ID_USE_POW_BLOCK,
        ITEM_ID_USE_TOROPICO_MANGO,
        ITEM_ID_USE_MILD_CACAO,
        ITEM_ID_USE_TUKUSHINBO,
        ITEM_ID_COOK_KAME_TEA,
        RFC_ITEM(SPIRIT_1),
        RFC_ITEM(SPIRIT_1),
        RFC_ITEM(SOUL_1),
        RFC_ITEM(SOUL_1)};

    s32 RFCItems_Uncommon[] = {
        ITEM_ID_COOK_HUNNY_KINOKO,
        ITEM_ID_COOK_HOT_COCOA,
        ITEM_ID_COOK_KINOKO_FRY,
        ITEM_ID_USE_BIG_EGG,
        ITEM_ID_USE_NANIGA_OKORUKANA,
        ITEM_ID_COOK_KARAKARA_COOK,
        ITEM_ID_COOK_HANAJIRU_SYRUP,
        ITEM_ID_USE_KINKYU_KINOKO,
        ITEM_ID_USE_KIRAKIRA_OTOSHI,
        ITEM_ID_USE_POWERFUL_MEET,
        ITEM_ID_USE_PRIMITIVENUT,
        RFC_ITEM(VOUCHER_CAKE),
        RFC_ITEM(VOUCHER_CAKE),
        RFC_ITEM(VOUCHER_THUNDER),
        RFC_ITEM(VOUCHER_THUNDER),
        RFC_ITEM(SPIRIT_2),
        RFC_ITEM(SPIRIT_2),
        RFC_ITEM(SOUL_2),
        RFC_ITEM(SOUL_2),
        RFC_ITEM(AEGIS_1),
        RFC_ITEM(AUSPICE_1)};

    s32 RFCItems_Rare[] = {
        ITEM_ID_COOK_DINNER,
        ITEM_ID_COOK_PANSY_SYRUP,
        ITEM_ID_COOK_GOLD_CHOKO,
        ITEM_ID_COOK_LOVE_NOODLE,
        ITEM_ID_COOK_FRUITS_HUMBURG,
        ITEM_ID_COOK_HOTDOG,
        ITEM_ID_COOK_PEACH_TART,
        ITEM_ID_COOK_MIX_SHAKE,
        ITEM_ID_COOK_MOUSSE_CAKE,
        ITEM_ID_COOK_CHOCOLA_CAKE,
        ITEM_ID_COOK_SUGER_HOUSE,
        RFC_ITEM(VOUCHER_STELLAR),
        RFC_ITEM(VOUCHER_STELLAR),
        RFC_ITEM(SPIRIT_3),
        RFC_ITEM(SOUL_3)};

    s32 RFCItems_Legendary[] = {
        ITEM_ID_USE_ULTRA_DRINK,
        ITEM_ID_COOK_DOROCY_DINNER,
        ITEM_ID_COOK_SNOW_RABBIT,
        ITEM_ID_COOK_TRIAL_PAN,
        RFC_ITEM(VOUCHER_RED),
        RFC_ITEM(VOUCHER_ORANGE),
        RFC_ITEM(VOUCHER_YELLOW),
        RFC_ITEM(VOUCHER_GREEN),
        RFC_ITEM(VOUCHER_CYAN),
        RFC_ITEM(VOUCHER_BLUE),
        RFC_ITEM(VOUCHER_PURPLE),
        RFC_ITEM(SPIRIT_4),
        RFC_ITEM(SOUL_4),
        RFC_ITEM(AUSPICE_2),
        RFC_ITEM(AEGIS_2)};

    s32 RFCItems_Artifacts[] = {
        RFC_ITEM(ARTIFACT_SOUL),
        RFC_ITEM(ARTIFACT_SPIRIT),
        RFC_ITEM(ARTIFACT_AEGIS),
        RFC_ITEM(ARTIFACT_AUSPICE),
        RFC_ITEM(ARTIFACT_DELIGHT),
        RFC_ITEM(ARTIFACT_DEMISE)};

    s32 VoucherTearChances[VOUCHER_BLACK - VOUCHER_CAKE] = {8, 10, 7, 8, 2, 2, 2, 2, 2, 2, 2, 2};

    s32 VoucherAdd(void *wp)
    {
        s32 i;
        for (i = 0; i < VOUCHER_MAX; i += 1)
        {
            if (Lunatic->Voucher.Work[i] == nullptr) // Not in use
                break;
        }
        assertf(i < VOUCHER_MAX, "Voucher limit of %d (VOUCHER_MAX) exceeded", VOUCHER_MAX);
        Lunatic->Voucher.Work[i] = (VoucherWork *)memory::__memAlloc(0, sizeof(VoucherWork));
        msl::string::memset(Lunatic->Voucher.Work[i], 0, sizeof(VoucherWork));
        Lunatic->Voucher.Work[i]->UW.Any = wp;
        Lunatic->Voucher.Work[i]->iconAlpha = 150;
        return i;
    }

    s32 VoucherItemIdToIdx(s32 itemId)
    {
        for (s32 i = 0; i < VOUCHER_MAX; i += 1)
        {
            if (Lunatic->Voucher.Work[i] != nullptr) // In use
            {
                if (Lunatic->Voucher.Work[i]->itemId == itemId)
                    return i;
            }
        }
        return -1;
    }

    VoucherWork *VoucherItemIdToPtr(s32 itemId)
    {
        VoucherWork *Voucher = nullptr;
        for (s32 i = 0; i < VOUCHER_MAX; i += 1)
        {
            Voucher = Lunatic->Voucher.Work[i];
            if (Voucher != nullptr) // In use
            {
                if (Voucher->itemId == itemId)
                    break;
            }
        }
        assertf(Voucher != nullptr, "VoucherWork not found; itemId == %d", itemId);
        return Voucher;
    }

    void VoucherRemove(s32 itemId)
    {
        VoucherWork *Voucher = VoucherItemIdToPtr(itemId);
        msl::string::memset(Voucher, 0, sizeof(VoucherWork));
        memory::__memFree(0, Voucher);
        Voucher = nullptr;
        return;
    }

    VoucherState VoucherGetStateById(s32 itemId, s32 *idx)
    {
        s32 i = VoucherItemIdToIdx(itemId);
        if (idx != nullptr)
            *idx = i;
        if (i == -1)
            return V_INACTIVE;
        if (Lunatic->Voucher.Work[i]->torn)
            return V_TORN;
        return V_ACTIVE;
    }

    void VoucherActionSpin(s32 itemId, s32 deleteIdx)
    {
        if (mario::marioChkKey() == false) // todo: check for hud state and if a fade entry is active
            return;
        VoucherWork *Voucher = VoucherItemIdToPtr(itemId);
        u8 alphaMod;
        Voucher->isSpinning = true;
        if (Voucher->iconRotationTimer < 20)
        {
            alphaMod = (u8)system::intplGetValue(4, 0, 105, Voucher->iconRotationTimer, 20);
            Voucher->iconAlpha = (u8)150 + alphaMod;
        }
        else if (Voucher->iconRotationTimer <= 50)
        {
            alphaMod = (u8)system::intplGetValue(4, 0, 105, (Voucher->iconRotationTimer - 20), 30);
            Voucher->iconAlpha = (u8)255 - alphaMod;
        }
        Voucher->iconRotation = system::intplGetValue(4, 0.0f, 360.0f, Voucher->iconRotationTimer, 60);
        Voucher->iconRotationTimer += 1;
        if (Voucher->iconRotation >= 359.5f)
        {
            Voucher->iconRotation = 0.0f;
            Voucher->iconRotationTimer = 0;
            Voucher->isSpinning = false;
            globalop::globalopDelEntry(deleteIdx);
        }
        return;
    }

    void VoucherTearSpin(s32 itemId, s32 deleteIdx)
    {
        if (mario::marioChkKey() == false) // todo: check for hud state and if a fade entry is active
            return;
        VoucherWork *Voucher = VoucherItemIdToPtr(itemId);
        u8 alphaMod;
        Voucher->isSpinning = true;
        if (Voucher->iconRotationTimer < 30)
        {
            alphaMod = (u8)system::intplGetValue(4, 0, 105, Voucher->iconRotationTimer, 30);
            Voucher->iconAlpha = (u8)150 + alphaMod;
        }
        else if (Voucher->iconRotationTimer > 120)
        {
            alphaMod = (u8)system::intplGetValue(4, 0, 255, (Voucher->iconRotationTimer - 120), 120);
            Voucher->iconAlpha = (u8)255 - alphaMod;
        }
        if (Voucher->iconRotationTimer < 60)
            Voucher->iconRotation = system::intplGetValue(1, 0.0f, 360.0f, Voucher->iconRotationTimer, 60);
        else
            Voucher->iconRotation = system::intplGetValue(0, 0.0f, 360.0f, ((Voucher->iconRotationTimer - 60) % 10), 10);
        Voucher->iconRotationTimer += 1;
        if (Voucher->iconAlpha < 1)
        {
            Voucher->iconAlpha = 0;
            Voucher->iconRotation = 0.0f;
            Voucher->iconRotationTimer = 0;
            Voucher->isSpinning = false;
            VoucherRemove(Voucher->itemId);
            globalop::globalopDelEntry(deleteIdx);
        }
        return;
    }

    void VoucherSpin(s32 itemId, bool tearSpin)
    {
        VoucherWork *Voucher = VoucherItemIdToPtr(itemId);
        if (Voucher->isSpinning) // If a spin is active, force-reset it unequivocally
        {
            globalop::globalopDelEntry(Voucher->spinDeleteFuncIdx);
            Voucher->iconAlpha = 150;
            Voucher->iconRotation = 0.0f;
            Voucher->iconRotationTimer = 0;
            Voucher->isSpinning = false;
            Voucher->spinDeleteFuncIdx = 0;
        }
        if (!tearSpin)
            Voucher->spinDeleteFuncIdx = globalop::globalopAddEntry((void *)VoucherActionSpin, (void *)itemId);
        else
            Voucher->spinDeleteFuncIdx = globalop::globalopAddEntry((void *)VoucherTearSpin, (void *)itemId);
        // FRIEREN BURGER
        return;
    }

    bool VoucherTryTear(s32 itemId)
    {
        VoucherWork *Voucher = VoucherItemIdToPtr(itemId);
        s32 odds = system::irand(99);
        if (odds < Voucher->tearChance)
            return true;
        return false;
    }

    void VoucherDoTear(s32 itemId)
    {
        VoucherWork *Voucher = VoucherItemIdToPtr(itemId);
        Voucher->torn = true;
        VoucherSpin(itemId, true);
        (Voucher->tearFunc)();
        return;
    }

    s32 VoucherGetTearChance(s32 baseChance)
    {
        s32 difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 1:
            baseChance *= 1.25f;
            break;
        case 2:
            baseChance *= 1.5;
            break;
        case 3:
            baseChance *= 1.75f;
            break;
        default:
            break;
        }
        return baseChance;
    }

    void VoucherCallAction(s32 itemId)
    {
        VoucherState state = VoucherGetStateById(itemId, nullptr);
        if (state == V_TORN) // Don't redundantly call a voucher action once it's already torn
            return;
        if (state == V_ACTIVE)
        {
            VoucherWork *Voucher = VoucherItemIdToPtr(itemId);
            if (Voucher->isSpinning == true)
                return;
            if (Voucher->guaranteeTrig == 0)
            {
                if (VoucherTryTear(itemId) == true)
                    VoucherDoTear(itemId);
                else
                    (Voucher->actionFunc)();
            }
            else
            {
                Voucher->guaranteeTrig -= 1;
                (Voucher->actionFunc)();
            }
        }
        return;
    }

    s32 EvtVoucherCallAction(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 itemId = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        VoucherCallAction(itemId);
        return 2;
    }

    void CakeVoucherTear()
    {
        VoucherWork *Voucher = VoucherItemIdToPtr(VOUCHER_CAKE);
        s32 hp = Voucher->UW.Cake->hpGain;
        lpAddHp(-(hp / 2), hp); // Remove half of the max HP bonus, but add total bonus to reg HP
        return;
    }

    void CakeVoucherAction()
    {
        VoucherWork *Voucher = VoucherItemIdToPtr(VOUCHER_CAKE);
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        s32 maxHp = pouch->maxHp;
        lpAddHp(2, 2);
        if (maxHp == pouch->maxHp) // If max HP cannot increment, you are a fatass. No more cake for you
            VoucherDoTear(VOUCHER_CAKE);
        else
        {
            Voucher->UW.Cake->hpGain += (pouch->maxHp - maxHp);
            VoucherSpin(VOUCHER_CAKE, false);
        }
        return;
    }

    void CakeVoucherUse()
    {
        VCakeWork *wp = (VCakeWork *)memory::__memAlloc(0, sizeof(VCakeWork));
        msl::string::memset(wp, 0, sizeof(VCakeWork));
        s32 idx = VoucherAdd(wp);
        VoucherWork *Voucher = Lunatic->Voucher.Work[idx];
        Voucher->iconId = ICON_VOUCHER_CAKE;
        Voucher->itemId = VOUCHER_CAKE;
        Voucher->tearFunc = CakeVoucherTear;
        Voucher->actionFunc = CakeVoucherAction;
        Voucher->tearChance = VoucherGetTearChance(VoucherTearChances[Voucher->itemId]);
        Voucher->guaranteeTrig = 5;
        return;
    }

    void ThunderVoucherTear()
    {
        VoucherWork *Voucher = VoucherItemIdToPtr(VOUCHER_THUNDER);
        lpAddAtk(-(round(Voucher->UW.Thunder->atkBonus / 2)) + 1);
        lpAddCrit(-(round((f32)Voucher->UW.Thunder->critRateBonus / 2)) + 2, -(msl::math::floor(Voucher->UW.Thunder->critMultBonus / 2.0f)) + 8.0f);
        return;
    }

    void ThunderVoucherAction()
    {
        VoucherWork *Voucher = VoucherItemIdToPtr(VOUCHER_THUNDER);
        s32 odds = system::rand() % 10;
        if (odds < 2) // 20%
            lpAddAtk(Voucher->UW.Thunder->atkBonus += 1);
        else if (odds < 7) // 50%
            lpAddCrit(0, Voucher->UW.Thunder->critMultBonus += 8.0f);
        else // 30%
            lpAddCrit(Voucher->UW.Thunder->critRateBonus += 2, 0);
        VoucherSpin(VOUCHER_THUNDER, false);
        return;
    }

    void ThunderVoucherUse()
    {
        VThunderWork *wp = (VThunderWork *)memory::__memAlloc(0, sizeof(VThunderWork));
        msl::string::memset(wp, 0, sizeof(VThunderWork));
        s32 idx = VoucherAdd(wp);
        VoucherWork *Voucher = Lunatic->Voucher.Work[idx];
        Voucher->iconId = ICON_VOUCHER_THUNDER;
        Voucher->itemId = VOUCHER_THUNDER;
        Voucher->tearFunc = ThunderVoucherTear;
        Voucher->actionFunc = ThunderVoucherAction;
        Voucher->tearChance = VoucherGetTearChance(VoucherTearChances[Voucher->itemId]);
        Voucher->guaranteeTrig = 3;
        return;
    }

    s32 ThunderVoucherIncrementCtr(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 hp = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        VoucherState vState = VoucherGetStateById(VOUCHER_THUNDER, nullptr);
        if (hp <= 1 || vState != V_ACTIVE)
            return 2;
        VoucherWork *Voucher = VoucherItemIdToPtr(VOUCHER_THUNDER);
        Voucher->UW.Thunder->enemies += 1;
        if (Voucher->UW.Thunder->enemies == 30)
        {
            Voucher->UW.Thunder->enemies = 0;
            VoucherCallAction(VOUCHER_THUNDER);
        }
        return 2;
    }

    void StellarVoucherTear()
    {
        Lunatic->RFC.chestKeysOwned += 3;
        return;
    }

    void StellarVoucherAction()
    {
        VoucherSpin(VOUCHER_STELLAR, false);
        return;
    }

    void StellarVoucherUse()
    {
        VStellarWork *wp = (VStellarWork *)memory::__memAlloc(0, sizeof(VStellarWork));
        msl::string::memset(wp, 0, sizeof(VStellarWork));
        s32 idx = VoucherAdd(wp);
        VoucherWork *Voucher = Lunatic->Voucher.Work[idx];
        Voucher->iconId = ICON_VOUCHER_STELLAR;
        Voucher->itemId = VOUCHER_STELLAR;
        Voucher->tearFunc = StellarVoucherTear;
        Voucher->actionFunc = StellarVoucherAction;
        Voucher->tearChance = VoucherGetTearChance(VoucherTearChances[Voucher->itemId]);
        Voucher->guaranteeTrig = 4;
        return;
    }

    void JudgementVoucherTear()
    {
        return;
    }

    void JudgementVoucherAction()
    {
        return;
    }

    void JudgementVoucherUse()
    {
        VJudgementWork *wp = (VJudgementWork *)memory::__memAlloc(0, sizeof(VJudgementWork));
        msl::string::memset(wp, 0, sizeof(VJudgementWork));
        s32 idx = VoucherAdd(wp);
        VoucherWork *Voucher = Lunatic->Voucher.Work[idx];
        Voucher->iconId = ICON_VOUCHER_JUDGEMENT;
        Voucher->itemId = VOUCHER_JUDGEMENT;
        Voucher->tearFunc = JudgementVoucherTear;
        Voucher->actionFunc = JudgementVoucherAction;
        Voucher->tearChance = VoucherGetTearChance(VoucherTearChances[Voucher->itemId]);
        Voucher->guaranteeTrig = 0;
        return;
    }

    void AegisEndowmentUse()
    {
        Lunatic->Stats.AegisDef += 1;
        return;
    }

    void AegisInvocationUse()
    {
        Lunatic->Stats.AegisDef += 2;
        return;
    }

    void AuspiceEndowmentUse()
    {
        Lunatic->Stats.AuspiceDR += 20.0f;
        return;
    }

    void AuspiceInvocationUse()
    {
        Lunatic->Stats.AuspiceDR += 30.0f;
        return;
    }

    void SoulDropUse()
    {
        Lunatic->Stats.CritRate += 4;
        return;
    }

    void SoulBoonUse()
    {
        Lunatic->Stats.CritRate += 8;
        return;
    }

    void SoulEpiphanyUse()
    {
        Lunatic->Stats.CritRate += 16;
        return;
    }

    void SoulLegacyUse()
    {
        Lunatic->Stats.CritRate += 24;
        return;
    }

    void SpiritDropUse()
    {
        Lunatic->Stats.CritMult += 25.0f;
        return;
    }

    void SpiritBoonUse()
    {
        Lunatic->Stats.CritMult += 50.0f;
        return;
    }

    void SpiritEpiphanyUse()
    {
        Lunatic->Stats.CritMult += 100.0f;
        return;
    }

    void SpiritLegacyUse()
    {
        Lunatic->Stats.CritMult += 150.0f;
        return;
    }

    void SoulArtifactUse()
    {
        Lunatic->Stats.CritRate += 12;
        swdrv::swSet(1654);
        return;
    }

    void SpiritArtifactUse()
    {
        Lunatic->Stats.CritMult += 100.0f;
        swdrv::swSet(1655);
        return;
    }

    void AegisArtifactUse()
    {
        Lunatic->Stats.AegisDef += 1;
        swdrv::swSet(1656);
        return;
    }

    void AuspiceArtifactUse()
    {
        Lunatic->Stats.AuspiceDR += 25.0f;
        swdrv::swSet(1657);
        return;
    }

    void DelightArtifactUse()
    {
        lpAddHp(30, 0);
        swdrv::swSet(1658);
        return;
    }

    void DemiseArtifactUse()
    {
        lpAddAtk(2);
        swdrv::swSet(1659);
        return;
    }

    RFCItemData RFC_SpecialItems[] = {
        {ICON_VOUCHER_CAKE, cakeVName, cakeVDesc, nullptr, CakeVoucherUse, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_THUNDER, thunderVName, thunderVDesc, nullptr, ThunderVoucherUse, {255, 142, 43, 100}, {191, 119, 55, 255}},
        {ICON_VOUCHER_STELLAR, stellarVName, stellarVDesc, nullptr, StellarVoucherUse, {248, 255, 43, 100}, {168, 171, 77, 255}},
        {ICON_VOUCHER_JUDGEMENT, judgementVName, judgementVDesc, nullptr, JudgementVoucherUse, {81, 140, 189, 100}, {46, 81, 97, 255}},
        {ICON_VOUCHER_RED, redVName, redVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_ORANGE, orangeVName, orangeVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_YELLOW, yellowVName, yellowVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_GREEN, greenVName, greenVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_CYAN, cyanVName, cyanVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_BLUE, blueVName, blueVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_PURPLE, purpleVName, purpleVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_WHITE, whiteVName, whiteVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},
        {ICON_VOUCHER_BLACK, blackVName, blackVDesc, nullptr, nullptr, {252, 77, 255, 100}, {164, 76, 166, 255}},                // kek
        {ICON_SOUL_1, soul1Name, soul1Desc, soul1Get, SoulDropUse, {248, 255, 156, 100}, {146, 153, 50, 255}},                   // Soul Drop, +4% Crit Rate
        {ICON_SOUL_2, soul2Name, soul2Desc, soul2Get, SoulBoonUse, {248, 255, 156, 100}, {146, 153, 50, 255}},                   // Soul Boon, +8% Crit Rate
        {ICON_SOUL_3, soul3Name, soul3Desc, soul3Get, SoulEpiphanyUse, {248, 255, 156, 100}, {146, 153, 50, 255}},               // Soul Epiphany, +16% Crit Rate
        {ICON_SOUL_4, soul4Name, soul4Desc, soul4Get, SoulLegacyUse, {248, 255, 156, 100}, {146, 153, 50, 255}},                 // Soul Legacy, +24% Crit Rate
        {ICON_SPIRIT_1, spirit1Name, spirit1Desc, spirit1Get, SpiritDropUse, {41, 194, 255, 100}, {42, 116, 145, 255}},          // Spirit Drop, +25% Crit Mult
        {ICON_SPIRIT_2, spirit2Name, spirit2Desc, spirit2Get, SpiritBoonUse, {41, 194, 255, 100}, {42, 116, 145, 255}},          // Spirit Boon, +50% Crit Mult
        {ICON_SPIRIT_3, spirit3Name, spirit3Desc, spirit3Get, SpiritEpiphanyUse, {41, 194, 255, 100}, {42, 116, 145, 255}},      // Spirit Epiphany, +100% Crit Mult
        {ICON_SPIRIT_4, spirit4Name, spirit4Desc, spirit4Get, SpiritLegacyUse, {41, 194, 255, 100}, {42, 116, 145, 255}},        // Spirit Legacy, +150% Crit Mult
        {ICON_AEGIS_1, aegis1Name, aegis1Desc, aegis1Get, AegisEndowmentUse, {33, 96, 255, 100}, {34, 64, 140, 255}},            // Aegis Endowment, +15% DR
        {ICON_AEGIS_2, aegis2Name, aegis2Desc, aegis2Get, AegisInvocationUse, {33, 96, 255, 100}, {34, 64, 140, 255}},           // Aegis Invocation, +30% DR
        {ICON_AUSPICE_1, auspice1Name, auspice1Desc, auspice1Get, AuspiceEndowmentUse, {212, 53, 61, 100}, {135, 23, 29, 255}},  // Auspice Endowment, +1 DEF
        {ICON_AUSPICE_2, auspice2Name, auspice2Desc, auspice2Get, AuspiceInvocationUse, {212, 53, 61, 100}, {135, 23, 29, 255}}, // Auspice Invocation, +2 DEF
        {ICON_ARTIFACT_SOUL, artiSoulName, artiSoulDesc, nullptr, SoulArtifactUse, {248, 255, 156, 100}, {146, 153, 50, 255}},
        {ICON_ARTIFACT_SPIRIT, artiSpiritName, artiSpiritDesc, nullptr, SpiritArtifactUse, {41, 194, 255, 100}, {42, 116, 145, 255}},
        {ICON_ARTIFACT_AEGIS, artiAegisName, artiAegisDesc, nullptr, AegisArtifactUse, {33, 96, 255, 100}, {34, 64, 140, 255}},
        {ICON_ARTIFACT_AUSPICE, artiAuspiceName, artiAuspiceDesc, nullptr, AuspiceArtifactUse, {212, 53, 61, 100}, {135, 23, 29, 255}},
        {ICON_ARTIFACT_DELIGHT, artiDelightName, artiDelightDesc, nullptr, DelightArtifactUse, {212, 53, 61, 100}, {135, 23, 29, 255}},
        {ICON_ARTIFACT_DEMISE, artiDemiseName, artiDemiseDesc, nullptr, DemiseArtifactUse, {212, 53, 61, 100}, {135, 23, 29, 255}}};

    RFCColorDef RFC_Colors[] = {
        {{10, 10, 10, 255}, {0, 0, 0, 255}},          // Common
        {{128, 144, 160, 255}, {217, 217, 217, 255}}, // Uncommon -- Iron
        {{173, 150, 75, 255}, {250, 214, 74, 255}},   // Rare -- Gold
        {{63, 202, 179, 255}, {74, 237, 210, 255}}    // Legendary -- Diamond
    };

    const char *RFC_ChestNames[4] = {"MOBJ_gw_ta_big", "MOBJ_dan_u_big", "MOBJ_dan_r_big", "MOBJ_dan_l_big"};

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

    void *RFCSpecialGetPtr()
    {
        return &RFC_SpecialItems[0];
    }

    void *RFCColorsGetPtr()
    {
        return &RFC_Colors[0];
    }

    s32 RFCGetRarity(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], Lunatic->RFC.chestRarity);
        evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)RFCRarityNames[Lunatic->RFC.chestRarity]);
        return 2;
    }

    s32 RFCGetChestKeyParams(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], Lunatic->RFC.chestKeys);      // chest keys required
        evtmgr_cmd::evtSetValue(evtEntry, args[1], Lunatic->RFC.chestKeysOwned); // chest keys owned
        return 2;
    }

    s32 RFCBakudan(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        (void)evtEntry;
        mobjdrv::MobjEntry *chest = mobjdrv::mobjNameToPtr("box");
        mario::marioKeyOn();
        npcdrv::npcDamageMario(0, 0, &chest->pos, 0, Lunatic->RFC.chestKeys, 4);
        return 2;
    }

    s32 RFCSetChestKeys(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        Lunatic->RFC.chestKeysOwned = (u8)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        return 2;
    }

    void RFCUpdateSpecialGetCol(effpatch::EffPatchColorMask *mask)
    {
        mask->frmCtr += 1;
        if (mask->frmCtr > 60)
            return;
        mask->col1.r = (u8)system::intplGetValue(1, 0, (f32)mask->col2.r, mask->frmCtr, 60);
        mask->col1.g = (u8)system::intplGetValue(1, 0, (f32)mask->col2.g, mask->frmCtr, 60);
        mask->col1.b = (u8)system::intplGetValue(1, 0, (f32)mask->col2.b, mask->frmCtr, 60);
        return;
    }

    s32 RFCAnalyzeSpecial(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 idx = evtmgr_cmd::evtGetValue(evtEntry, args[0]); // CWSelect item idx
        if (idx < 0)
            return 2;
        s32 trueIdx = customwin::GlobalCW->Select[customwin::GlobalCW->activeSelect]->Descs[idx].iconId - ICON_VOUCHER_CAKE - TPLPATCH_ICON_REDIRECT; // converts LPIcon to LPCustomItem index
        Lunatic->RFC.rfcSpecialObtained[trueIdx] = true;                                                                                              // Prevents item from reappearing in the shop
        effdrv::EffEntry *eff = eff_pansy_kirakira::effPansyKirakiraEntry(1);
        effpatch::effpatchColorMaskEntry(eff, {0, 0, 0, 255}, RFC_SpecialItems[trueIdx].effCol, RFCUpdateSpecialGetCol);
        if (RFC_SpecialItems[trueIdx].useFunc != nullptr)
            (RFC_SpecialItems[trueIdx].useFunc)();
        if (RFC_SpecialItems[trueIdx].useMsg != nullptr)
            evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)RFC_SpecialItems[trueIdx].useMsg);
        else
            evtmgr_cmd::evtSetValue(evtEntry, args[1], -1);
        evtmgr_cmd::evtSetValue(evtEntry, args[0], trueIdx);
        return 2;
    }

    s32 RFCGenerate(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        spm::evtmgr::EvtScriptCode *interactScript = (spm::evtmgr::EvtScriptCode *)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        spm::evtmgr::EvtScriptCode *openScript = (spm::evtmgr::EvtScriptCode *)evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        u32 loaded = animdrv::animGroupBaseAsync(RFC_ChestNames[Lunatic->RFC.chestRarity], 0, nullptr);
        if (loaded == 0)
            return 0;
        mobjdrv::mobjEntry(rfcChestName, RFC_ChestNames[Lunatic->RFC.chestRarity]);
        mobjdrv::MobjEntry *mobj = mobjdrv::mobjNameToPtr(rfcChestName);
        mobjdrv::mobjSetPosition(rfcChestName, 75.0f, 25.0f, -87.5f);
        mobjdrv::mobjHitEntry(mobj, 6);
        mobjdrv::mobjCalcMtx(mobj);
        mobj->updateFunction = evt_mobj::mobj_thako;
        mobj->interactScript = interactScript;
        mobj->afterInteractScript = openScript;
        mobj->flag0 |= 0x400046;
        return 2;
    }

    s32 RFCReroll(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)evtEntry;
        (void)firstRun;
        DanGen_Items(false);
        return 2;
    }

    s32 RFCPatchDanChestMobjHitEntry(const char *modelName) // seems to be in r3 in both functions
    {
        s32 ret, i;
        for (i = 0; i < 4; i += 1)
        {
            ret = msl::string::strcmp(modelName, RFC_ChestNames[i]);
            if (ret == 0)
                break;
        }
        return ret;
    }

    void RFCDebugForceReroll()
    {
        mario::MarioWork *mario = mario::marioGetPtr();
        if ((mario->buttonsPressed & WPAD_BTN_C) == WPAD_BTN_C)
            DanGen_Items(false);
    }

    void RFCDRVPatches()
    {
        writeBranchLink(mobjdrv::mobjHitEntry, 0xF8, RFCPatchDanChestMobjHitEntry);
        writeBranchLink(mobjdrv::mobjCalcMtx, 0xC8, RFCPatchDanChestMobjHitEntry);
        if (DebugMode)
            globalop::globalopAddEntry((void *)RFCDebugForceReroll, nullptr);
    }
}
