#include "mod.h"
#include <gen.h>
#include <common.h>
#include <evtpatch.h>
#include <tplpatch.h>
#include <evt_cmd.h>
#include "patch.h"
#include <util.h>
#include <msgpatch.h>
#include <customwin.h>
#include "lunatic/localize.h"

#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/evt_cam.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
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
#include <spm/gxsub.h>
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
#include <spm/npcdrv.h>
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

    void ApathySet()
    {
        ApathyWork *wp = (ApathyWork *)memory::__memAlloc(0, sizeof(ApathyWork));
        msl::string::memset(wp, 0, sizeof(ApathyWork));
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        s32 difficulty = swdrv::swByteGet(1620);
        Lunatic->Luna.DW.UW.Apathy = wp;
        switch (difficulty)
        {
        case 0:
            wp->marioHpMult = 0.1;
            wp->enemyDamageIncrease = 1;
            wp->marioDamageDecrease = 0;
            wp->enemyMaxHPMult = 1.0;
            wp->dispMaxHPDecrease = 10;
            wp->dispEnemyHPIncrease = 0;
            break;
        case 1:
            wp->marioHpMult = 0.2;
            wp->enemyDamageIncrease = 2;
            wp->marioDamageDecrease = 1;
            wp->enemyMaxHPMult = 1.2;
            wp->dispMaxHPDecrease = 20;
            wp->dispEnemyHPIncrease = 20;
            break;
        case 2:
            wp->marioHpMult = 0.33;
            wp->enemyDamageIncrease = 3;
            wp->marioDamageDecrease = 1;
            wp->enemyMaxHPMult = 1.5;
            wp->dispMaxHPDecrease = 33;
            wp->dispEnemyHPIncrease = 50;
            break;
        default:
            wp->marioHpMult = 0.5;
            wp->enemyDamageIncrease = 4;
            wp->marioDamageDecrease = 2;
            wp->enemyMaxHPMult = 2.0;
            wp->dispMaxHPDecrease = 50;
            wp->dispEnemyHPIncrease = 100;
            break;
        }
        wp->storedHP = msl::math::floor((f32)pouch->maxHp * wp->marioHpMult);
        pouch->hp = msl::math::floor((f32)pouch->hp * wp->marioHpMult) + 1;
        pouch->maxHp -= wp->storedHP;
        wp->storedCritRate = (s32)msl::math::floor((f32)Lunatic->Stats.CritRate / 2.0);
        Lunatic->Stats.CritRate -= wp->storedCritRate;
        wp->storedCritMult = msl::math::floor((f32)Lunatic->Stats.CritMult / 2.0);
        Lunatic->Stats.CritMult -= wp->storedCritMult;
        return;
    }

    void ApathyClear()
    {
        ApathyWork *wp = Lunatic->Luna.DW.UW.Apathy;
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        pouch->maxHp += wp->storedHP;
        pouch->hp += (s32)(wp->storedHP * wp->marioHpMult);
        Lunatic->Stats.CritRate += wp->storedCritRate;
        Lunatic->Stats.CritMult += wp->storedCritMult;
        return;
    }

    void DreadSet()
    {
        DreadWork *wp = (DreadWork *)memory::__memAlloc(0, sizeof(DreadWork));
        msl::string::memset(wp, 0, sizeof(DreadWork));
        Lunatic->Luna.DW.UW.Dread = wp;
        s32 difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            wp->dispBlockChance = 10;
            break;
        case 1:
            wp->dispBlockChance = 20;
            break;
        case 2:
            wp->dispBlockChance = 30;
            break;
        default:
            wp->dispBlockChance = 50;
            break;
        }
        return;
    }

    void PrejudiceSet()
    {
        PrejudiceWork *wp = (PrejudiceWork *)memory::__memAlloc(0, sizeof(PrejudiceWork));
        msl::string::memset(wp, 0, sizeof(PrejudiceWork));
        Lunatic->Luna.DW.UW.Prejudice = wp;
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        s32 difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            wp->dispInstantCoinLoss = 15;
            wp->coinLossChance = 30;
            break;
        case 1:
            wp->dispInstantCoinLoss = 20;
            wp->coinLossChance = 50;
            break;
        case 2:
            wp->dispInstantCoinLoss = 25;
            wp->coinLossChance = 70;
            break;
        default:
            wp->dispInstantCoinLoss = 30;
            wp->coinLossChance = 100;
            break;
        }
        wp->coinThreshold = (s32)(msl::math::floor((f32)pouch->coins * 0.15) + 1);
        return;
    }

    void PrejudiceAction()
    {
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        s32 coinsLost;
        f32 mod = (f32)(system::rand() % 11 + (Lunatic->Luna.DW.UW.Prejudice->dispInstantCoinLoss - 5));
        mod /= 100;
        coinsLost = (s32)msl::math::floor(pouch->coins * mod);
        if (hud::hud_wp->countdownTimer == 299)
        {
            if (pouch->coins > 0 && coinsLost != 0)
                pouch->coins -= coinsLost;
        }
        else if (hud::hud_wp->countdownTimer < 295)
        {
            s32 odds = system::rand() % 100;
            if (pouch->coins > 0 && odds < Lunatic->Luna.DW.UW.Prejudice->coinLossChance)
                pouch->coins -= 1;
        }
        return;
    }

    void IndifferenceSet()
    {
        IndifferenceWork *wp = (IndifferenceWork *)memory::__memAlloc(0, sizeof(IndifferenceWork));
        msl::string::memset(wp, 0, sizeof(IndifferenceWork));
        Lunatic->Luna.DW.UW.Indifference = wp;
        s32 difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            wp->repeat = 1;
            break;
        case 1:
            wp->repeat = 2;
            break;
        case 2:
            wp->repeat = 3;
            break;
        default:
            wp->repeat = 4;
            break;
        }
        return;
    }

    s32 IndifferenceAction(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 indiffItems[] = {83, 95, 98, 160, 174, 175, 176, 178};
        s32 i, idx;
        s32 itemsSpawned = 0;
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        for (i = 0; i < 8; i += 1)
            evtmgr_cmd::evtSetValue(evtEntry, args[i], 0);
        for (i = 0; i < Lunatic->Luna.DW.UW.Indifference->repeat; i += 1)
        {
            s32 odds = system::rand() % 100;
            if (odds < 50)
            {
                idx = system::rand() % (sizeof(indiffItems) / 4);
                if ((mario_pouch::pouchCountUseItems() + itemsSpawned) < 10)
                {
                    itemsSpawned += 1;
                    evtmgr_cmd::evtSetValue(evtEntry, args[(i * 2)], indiffItems[idx]);
                }
                else
                {
                    s32 invIdx = system::rand() % 10;
                    evtmgr_cmd::evtSetValue(evtEntry, args[(i * 2)], (s32)msgdrv::msgSearch(item_data::itemDataTable[pouch->useItem[invIdx]].nameMsg));
                    pouch->useItem[invIdx] = indiffItems[idx];
                    evtmgr_cmd::evtSetValue(evtEntry, args[((i * 2) + 1)], (s32)msgdrv::msgSearch(item_data::itemDataTable[pouch->useItem[invIdx]].nameMsg));
                }
            }
        }
        return 2;
    }

    EVT_BEGIN(dan_disorder_indifference_dialogue)
    // LW(9) is the original item, LW(10) is the replacement
    IF_NOT_EQUAL(LW(10), 0) // If the replacement is set, then continue/assert LW(9) is the original item
    ADD(LW(11), 1)
    IF_EQUAL(LW(11), 1)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    USER_FUNC(evt_msg::evt_msg_print_insert, 1, PTR(disorderIndifferenceItemNotif), 0, 0, LW(9), LW(10))
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print_add_insert, 1, PTR(disorderIndifferenceItemNotif2), LW(9), LW(10))
    END_IF()
    WAIT_MSEC(1000)
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(dan_disorder_indifference)
    // Floors rem can be 0 only if preId is non-zero.
    USER_FUNC(DisorderGetFloorsRem, LW(5))
    IF_EQUAL(LW(5), 0)
    IF_EQUAL(LW(6), 0)
    RETURN()
    END_IF()
    END_IF()
    USER_FUNC(IndifferenceAction, LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8))
    // Spawn items that don't have replacements
    IF_EQUAL(LW(2), 0)
    IF_NOT_EQUAL(LW(1), 0)
    USER_FUNC(evt_item::evt_item_entry, PTR("IN1"), LW(1), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("IN1"), 0x8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("IN1"))
    END_IF()
    END_IF()
    IF_EQUAL(LW(4), 0)
    IF_NOT_EQUAL(LW(3), 0)
    USER_FUNC(evt_item::evt_item_entry, PTR("IN2"), LW(3), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("IN2"), 0x8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("IN2"))
    END_IF()
    END_IF()
    IF_EQUAL(LW(6), 0)
    IF_NOT_EQUAL(LW(5), 0)
    USER_FUNC(evt_item::evt_item_entry, PTR("IN3"), LW(5), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("IN3"), 0x8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("IN3"))
    END_IF()
    END_IF()
    IF_EQUAL(LW(8), 0)
    IF_NOT_EQUAL(LW(7), 0)
    USER_FUNC(evt_item::evt_item_entry, PTR("IN4"), LW(7), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("IN4"), 0x8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("IN4"))
    END_IF()
    END_IF()
    // Iterate through items to see if they were forced into the inventory
    SET(LW(11), 0)
    SET(LW(9), LW(1))
    SET(LW(10), LW(2))
    RUN_CHILD_EVT(dan_disorder_indifference_dialogue)
    SET(LW(9), LW(3))
    SET(LW(10), LW(4))
    RUN_CHILD_EVT(dan_disorder_indifference_dialogue)
    SET(LW(9), LW(5))
    SET(LW(10), LW(6))
    RUN_CHILD_EVT(dan_disorder_indifference_dialogue)
    SET(LW(9), LW(7))
    SET(LW(10), LW(8))
    RUN_CHILD_EVT(dan_disorder_indifference_dialogue)
    IF_LARGE(LW(11), 0)
    USER_FUNC(evt_msg::evt_msg_print_add, 1, PTR("\n<k>"))
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_IF()
    RETURN()
    EVT_END()

    void RecalcitranceSet()
    {
        RecalcitranceWork *wp = (RecalcitranceWork *)memory::__memAlloc(0, sizeof(RecalcitranceWork));
        msl::string::memset(wp, 0, sizeof(RecalcitranceWork));
        Lunatic->Luna.DW.UW.Recalcitrance = wp;
        s32 difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            wp->dispXpPct = 100;
            wp->dispReturnPostage = 30;
            wp->maxRetPostDmg = 2;
            break;
        case 1:
            wp->dispXpPct = 150;
            wp->dispReturnPostage = 50;
            wp->maxRetPostDmg = 4;
            break;
        case 2:
            wp->dispXpPct = 200;
            wp->dispReturnPostage = 70;
            wp->maxRetPostDmg = 8;
            break;
        default:
            wp->dispXpPct = 300;
            wp->dispReturnPostage = 100;
            wp->maxRetPostDmg = 10;
            break;
        }
        return;
    }

    void DepravitySet()
    {
        DepravityWork *wp = (DepravityWork *)memory::__memAlloc(0, sizeof(DepravityWork));
        msl::string::memset(wp, 0, sizeof(DepravityWork));
        Lunatic->Luna.DW.UW.Depravity = wp;
        s32 difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            wp->allLv4FloorThreshold = 80;
            break;
        case 1:
            wp->allLv4FloorThreshold = 60;
            break;
        case 2:
            wp->allLv4FloorThreshold = 40;
            break;
        default:
            wp->allLv4FloorThreshold = 20;
            break;
        }
        return;
    }

    s32 DepravityGetAllLv4Threshold(s32 difficulty)
    {
        // This can't be part of the DepravityWork struct because work is only initialized after the color takes effect
        // This variable is needed before the first room displays at all
        // Consider reworking disorder init to use preId when setting, then alloc work before display?
        s32 ret;
        switch (difficulty)
        {
        case 0:
            ret = 180;
            break;
        case 1:
            ret = 160;
            break;
        case 2:
            ret = 40;
            break;
        default:
            ret = 20;
            break;
        }
        return ret;
    }

    bool DepravityCheckActive()
    {
        if ((Lunatic->Luna.disorder == DISORDER_BLUE && Lunatic->Luna.DW.floorsRem != 0) || Lunatic->Luna.DW.preId == 6)
            return true;
        return false;
    }

    s32 DepravityAction(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        (void)evtEntry;
        s32 currentFloor = swdrv::swByteGet(1);
        bool active = DepravityCheckActive();
        if (!active)
            return 2;
        DanGen_Enemies(currentFloor, true);
        DanGen_Enemies_Apply();
        return 2;
    }

    void IndolenceSet()
    {
        IndolenceWork *wp = (IndolenceWork *)memory::__memAlloc(0, sizeof(IndolenceWork));
        msl::string::memset(wp, 0, sizeof(IndolenceWork));
        Lunatic->Luna.DW.UW.Indolence = wp;
        s32 difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            wp->attackEffectChance = 25;
            wp->dispDmgPctBonus = 50;
            wp->slowDuration = 5;
            break;
        case 1:
            wp->attackEffectChance = 50;
            wp->dispDmgPctBonus = 50;
            wp->slowDuration = 5;
            break;
        case 2:
            wp->attackEffectChance = 75;
            wp->dispDmgPctBonus = 100;
            wp->slowDuration = 10;
            break;
        default:
            wp->attackEffectChance = 100;
            wp->dispDmgPctBonus = 100;
            wp->slowDuration = 10;
            break;
        }
        return;
    }

    // Establish global data tables for Blessings, Curses, and Disorders

    DivineJudgement Blessings[MERLUNA_PARAMITA] =
        {
            {spectreName, {120, 0}, 0, nullptr, nullptr},
            {houraiName, {0, 0}, 0, nullptr, nullptr},
            {paramitaName, {0, 0}, 0, nullptr, nullptr}
        };

    DivineJudgement Curses[MERLUNA_MIGRAINE] =
        {
            {shionName, {0, 0}, 0, nullptr, nullptr},
            {hexName, {0, 0}, 0, nullptr, nullptr},
            {migraineName, {0, 0}, 0, nullptr, nullptr}
        };

    LeyLineDisorder Disorders[DISORDER_BLACK] =
        {
            {apathyName, apathyDesc, {255, 0, 0, 30}, {255, 0, 0, 50}, {60, 0, 0, 255}, 20, 0.7, 3000, ApathySet, ApathyClear},                       // APATHY
            {dreadName, dreadDesc, {255, 128, 0, 30}, {255, 128, 0, 65}, {60, 30, 0, 255}, 40, 1.2, 5000, DreadSet, nullptr},                         // DREAD
            {prejudiceName, prejudiceDesc, {255, 255, 0, 30}, {255, 255, 0, 50}, {60, 60, 0, 255}, 15, 0.7, 4000, PrejudiceSet, nullptr},             // PREJUDICE
            {indifferenceName, indifferenceDesc, {0, 255, 0, 20}, {0, 255, 0, 40}, {0, 60, 0, 255}, 20, 1.0, 4500, IndifferenceSet, nullptr},         // INDIFFERENCE
            {recalcitranceName, recalcitranceDesc, {0, 255, 255, 20}, {0, 255, 225, 40}, {0, 60, 60, 255}, 30, 1.2, 4200, RecalcitranceSet, nullptr}, // RECALCITRANCE
            {depravityName, depravityDesc, {0, 0, 255, 30}, {0, 0, 255, 50}, {0, 0, 60, 255}, 50, 1.3, 6900, DepravitySet, nullptr},                  // DEPRAVITY
            {indolenceName, indolenceDesc, {128, 0, 255, 30}, {128, 0, 225, 50}, {30, 0, 60, 255}, 10, 0.5, 2000, IndolenceSet, nullptr},             // INDOLENCE
            {melancholyName, melancholyDesc, {255, 255, 255, 30}, {255, 255, 255, 50}, {60, 60, 60, 255}, 20, 0.8, 3200, nullptr, nullptr},           // MELANCHOLY
            {ruinName, ruinDesc, {0, 0, 0, 60}, {0, 0, 0, 255}, {10, 10, 10, 255}, 75, 1.5, 6666, nullptr, nullptr}                                    // RUIN
    };

    void SetDisorderSub(DisorderId id)
    {
        Lunatic->Luna.disorder = id;
        Lunatic->Luna.DW.preId = 0;
        Lunatic->Luna.DW.floorsRem = 5;
        Lunatic->Luna.Disorder = &Disorders[(s32)id - 1];
        if (Lunatic->Luna.Disorder->SetFunc != nullptr)
            (Lunatic->Luna.Disorder->SetFunc)();
        return;
    }

    void ClearDisorderSub(s32 id)
    {
        (void)id;
        if (Lunatic->Luna.Disorder->ClearFunc != nullptr)
            (Lunatic->Luna.Disorder->ClearFunc)();
        if (Lunatic->Luna.DW.UW.Any != nullptr)
            memory::__memFree(0, Lunatic->Luna.DW.UW.Any);
        Lunatic->Luna.disorder = DISORDER_NULL;
        Lunatic->Luna.Disorder = nullptr;
        return;
    }

    s32 DisorderGetId(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], (s32)Lunatic->Luna.disorder);
        //    evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)Lunatic->Luna.DW.preId);
        return 2;
    }

    s32 DisorderGetPreId(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], Lunatic->Luna.DW.preId);
        return 2;
    }

    s32 DisorderGetFloorsRem(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], (s32)Lunatic->Luna.DW.floorsRem);
        return 2;
    }

    s32 DisorderIntroIconOnOff(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 onOff = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        if (onOff > 0)
        {
            msgdrv::msgdrv_msgIcon[3].iconId = TPLPATCH_ICON_REDIRECT + ICON_B;
        }
        else
            msgdrv::msgdrv_msgIcon[3].iconId = 0xF;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(DisorderIntroIconOnOff, 1)

    s32 LunaGetConditionInfo(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 type = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        switch (type)
        {
        case 0: // Blessing
            break;
        case 1: // Curse
            break;
        case 2: // Disorder
            if (Lunatic->Luna.disorder > 0)
            {
                evtmgr_cmd::evtSetValue(evtEntry, args[2], (s32)Lunatic->Luna.disorder - 1 + ICON_DISORDER_APATHY + TPLPATCH_ICON_REDIRECT);
                evtmgr_cmd::evtSetValue(evtEntry, args[3], (s32)Lunatic->Luna.Disorder->name);
                msl::string::memset(Lunatic->Luna.DW.descBuf, 0, sizeof(Lunatic->Luna.DW.descBuf));
                switch (Lunatic->Luna.disorder)
                {
                case DISORDER_RED:
                    msl::stdio::sprintf(Lunatic->Luna.DW.descBuf, Disorders[0].desc, Lunatic->Luna.DW.UW.Apathy->dispMaxHPDecrease, Lunatic->Luna.DW.UW.Apathy->dispEnemyHPIncrease, Lunatic->Luna.DW.UW.Apathy->enemyDamageIncrease, Lunatic->Luna.DW.UW.Apathy->marioDamageDecrease);
                    break;
                case DISORDER_ORANGE:
                    msl::stdio::sprintf(Lunatic->Luna.DW.descBuf, Disorders[1].desc, Lunatic->Luna.DW.UW.Dread->dispBlockChance);
                    break;
                case DISORDER_YELLOW:
                    msl::stdio::sprintf(Lunatic->Luna.DW.descBuf, Disorders[2].desc, Lunatic->Luna.DW.UW.Prejudice->dispInstantCoinLoss, Lunatic->Luna.DW.UW.Prejudice->coinLossChance, Lunatic->Luna.DW.UW.Prejudice->coinThreshold);
                    break;
                case DISORDER_GREEN:
                    msl::stdio::sprintf(Lunatic->Luna.DW.descBuf, Disorders[3].desc, Lunatic->Luna.DW.UW.Indifference->repeat);
                    break;
                case DISORDER_CYAN:
                    msl::stdio::sprintf(Lunatic->Luna.DW.descBuf, Disorders[4].desc, Lunatic->Luna.DW.UW.Recalcitrance->dispXpPct, Lunatic->Luna.DW.UW.Recalcitrance->dispReturnPostage, Lunatic->Luna.DW.UW.Recalcitrance->maxRetPostDmg);
                    break;
                case DISORDER_BLUE:
                    msl::stdio::sprintf(Lunatic->Luna.DW.descBuf, Disorders[5].desc, Lunatic->Luna.DW.UW.Depravity->allLv4FloorThreshold);
                    break;
                case DISORDER_PURPLE:
                    msl::stdio::sprintf(Lunatic->Luna.DW.descBuf, Disorders[6].desc, Lunatic->Luna.DW.UW.Indolence->attackEffectChance, Lunatic->Luna.DW.UW.Indolence->dispDmgPctBonus, Lunatic->Luna.DW.UW.Indolence->slowDuration);
                    break;
                default:
                    break;
                }
                evtmgr_cmd::evtSetValue(evtEntry, args[4], (s32)Lunatic->Luna.DW.descBuf);
                evtmgr_cmd::evtSetValue(evtEntry, args[5], (s32)&Lunatic->Luna.Disorder->textDrawCol);
            }
            evtmgr_cmd::evtSetValue(evtEntry, args[1], (s32)Lunatic->Luna.disorder);
            break;
        }
        return 2;
    }

    s32 DisorderDraw(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        (void)evtEntry;
        if (Lunatic->Luna.disorder == DISORDER_NULL)
            return 0;
        wii::gx::GXColor color;
        u8 red = Disorders[Lunatic->Luna.disorder - 1].mainCol.r;
        u8 green = Disorders[Lunatic->Luna.disorder - 1].mainCol.g;
        u8 blue = Disorders[Lunatic->Luna.disorder - 1].mainCol.b;
        u8 alpha = Disorders[Lunatic->Luna.disorder - 1].mainCol.a;
        switch (Lunatic->Luna.DW.tremorState)
        {
        case 1:                                                    // Tremor fadein
            if (Lunatic->Luna.DW.tremorIntplFrmMax == 0) // Init variables
            {
                Lunatic->Luna.DW.tremorIntplFrmMax = (s32)msl::math::floor((f32)(Lunatic->Luna.DW.finalShakeTime / 1000 * 60));
                Lunatic->Luna.DW.tremorIntplFrmTimer = 0;
                Lunatic->Luna.DW.intplProgressMax = 60;
                Lunatic->Luna.DW.intplProgress = 0;
            }
            red = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.r, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.r, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            green = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.g, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.g, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            blue = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.b, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.b, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            alpha = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.a, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.a, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            Lunatic->Luna.DW.tremorIntplFrmTimer += 1;
            Lunatic->Luna.DW.intplProgress += 1;
            if (Lunatic->Luna.DW.tremorIntplFrmTimer >= 60)
            {
                Lunatic->Luna.DW.intplProgress = 0;
                Lunatic->Luna.DW.tremorState = 2;
            }
            break;
        case 2: // Stay at severecol
            red = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.r, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.r, Lunatic->Luna.DW.intplProgressMax, Lunatic->Luna.DW.intplProgressMax);
            green = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.g, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.g, Lunatic->Luna.DW.intplProgressMax, Lunatic->Luna.DW.intplProgressMax);
            blue = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.b, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.b, Lunatic->Luna.DW.intplProgressMax, Lunatic->Luna.DW.intplProgressMax);
            alpha = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.a, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.a, Lunatic->Luna.DW.intplProgressMax, Lunatic->Luna.DW.intplProgressMax);
            Lunatic->Luna.DW.tremorIntplFrmTimer += 1;
            if ((Lunatic->Luna.DW.tremorIntplFrmMax - Lunatic->Luna.DW.tremorIntplFrmTimer) <= 60) // Check if should fade back to normal
                Lunatic->Luna.DW.tremorState = 3;
            break;
        case 3: // Tremor fadeout
            red = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.r, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.r, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            green = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.g, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.g, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            blue = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.b, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.b, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            alpha = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.a, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.a, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            red = Disorders[Lunatic->Luna.disorder - 1].severeCol.r - red + (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.r;
            green = Disorders[Lunatic->Luna.disorder - 1].severeCol.g - green + (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.g;
            blue = Disorders[Lunatic->Luna.disorder - 1].severeCol.b - blue + (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.b;
            alpha = Disorders[Lunatic->Luna.disorder - 1].severeCol.a - alpha + (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.a;
            Lunatic->Luna.DW.intplProgress += 1;
            Lunatic->Luna.DW.tremorIntplFrmTimer += 1;
            if (Lunatic->Luna.DW.tremorIntplFrmTimer == Lunatic->Luna.DW.tremorIntplFrmMax)
            {
                Lunatic->Luna.DW.tremorIntplFrmMax = 0;
                Lunatic->Luna.DW.tremorIntplFrmTimer = 0;
                Lunatic->Luna.DW.tremorState = 0;
                Lunatic->Luna.DW.intplProgress = 0;
                Lunatic->Luna.DW.intplProgressMax = 0;
            }
        }
        color = {red, green, blue, alpha};
        mapdrv::mapSetColor(&color);
        return 0;
    }

    s32 DisorderSetColor(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 id = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 clear = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        f32 red, green, blue, alpha;
        if (Lunatic->Luna.DW.intplProgressMax == 0)
        {
            Lunatic->Luna.DW.intplProgressMax = 100;
        }
        if (Lunatic->Luna.DW.intplProgress < Lunatic->Luna.DW.intplProgressMax)
        {
            red = system::intplGetValue(system::INTPL_MODE_LINEAR, 0.0f, (f32)Disorders[id - 1].mainCol.r, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            green = system::intplGetValue(system::INTPL_MODE_LINEAR, 0.0f, (f32)Disorders[id - 1].mainCol.g, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            blue = system::intplGetValue(system::INTPL_MODE_LINEAR, 0.0f, (f32)Disorders[id - 1].mainCol.b, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            alpha = system::intplGetValue(system::INTPL_MODE_LINEAR, 0.0f, (f32)Disorders[id - 1].mainCol.a, Lunatic->Luna.DW.intplProgress, Lunatic->Luna.DW.intplProgressMax);
            if (clear == 1)
            {
                red = -(red - Disorders[id - 1].mainCol.r);
                green = -(green - Disorders[id - 1].mainCol.g);
                blue = -(blue - Disorders[id - 1].mainCol.b);
                alpha = -(alpha - Disorders[id - 1].mainCol.a);
            }
            wii::gx::GXColor color = {(u8)red, (u8)green, (u8)blue, (u8)alpha};
            mapdrv::mapSetColor(&color);
            Lunatic->Luna.DW.intplProgress += 1;
            // DisorderDraw will attempt to draw after this interpolation ends & before ClearDisorderSub, so we set disorder to null directly beforehand
            if (Lunatic->Luna.DW.intplProgress == 99)
                Lunatic->Luna.disorder = DISORDER_NULL;
            return 0;
        }
        else
        {
            Lunatic->Luna.DW.intplProgressMax = 0;
            Lunatic->Luna.DW.intplProgress = 0;
            if (clear == 0)
            {
                SetDisorderSub((DisorderId)id);
            }
            else
                ClearDisorderSub(id);
            return 2;
        }
    }
    EVT_DECLARE_USER_FUNC(DisorderSetColor, 2)

    EVT_BEGIN(EvtDisorderInitOrClear)
    USER_FUNC(evt_door::evt_door_wait_flag, 0x100)
    DO(0)
    USER_FUNC(evt_mario::evt_mario_get_flags, 0, LW(0))
    IF_NOT_FLAG(LW(0), 0x8)
    DO_BREAK()
    END_IF()
    WAIT_FRM(1)
    WHILE()
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    USER_FUNC(evt_npc::evt_npc_freeze_all)
    WAIT_MSEC(300)
    INLINE_EVT()
    WAIT_MSEC(1000)
    USER_FUNC(DisorderSetColor, LW(10), LW(11))
    END_INLINE()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("E_3"), 0)
    USER_FUNC(evt_snd::evt_snd_sfxon, PTR("SFX_EVT_QUAKE1L"))
    USER_FUNC(evt_snd::evt_snd_get_last_sfx_id, LW(0))
    IF_EQUAL(GSWF(1630), 0) // Lighter camera tremors ACTIVE
    DIVF(LW(12), 4)
    END_IF()
    USER_FUNC(evt_cam::evt_cam_shake, camdrv::CAM_ID_3D, LW(12), LW(12), FLOAT(0.0), 3000, 0)
    USER_FUNC(evt_snd::evt_snd_sfxoff, LW(0))
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_7"), 0)
    WAIT_MSEC(700)
    IF_EQUAL(GSWF(1661), 0)
    SET(GSWF(1661), 1)
    USER_FUNC(DisorderIntroIconOnOff, 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(disorderIntro), 0, 0)
    USER_FUNC(DisorderIntroIconOnOff, 0)
    WAIT_MSEC(300)
    END_IF()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    WAIT_MSEC(300)
    USER_FUNC(evt_npc::evt_npc_unfreeze_all)
    USER_FUNC(evt_mario::evt_mario_key_on)
    USER_FUNC(DisorderGetId, LW(5))
    IF_EQUAL(LW(5), 4)
    RUN_CHILD_EVT(dan_disorder_indifference)
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(EvtDisorderTremor)
    USER_FUNC(evt_snd::evt_snd_sfxon, PTR("SFX_EVT_QUAKE1L"))
    USER_FUNC(evt_snd::evt_snd_get_last_sfx_id, LW(0))
    IF_EQUAL(GSWF(1630), 0) // Lighter camera tremors ACTIVE
    DIVF(LW(12), 4)
    END_IF()
    USER_FUNC(evt_cam::evt_cam_shake, camdrv::CAM_ID_3D, LW(12), LW(12), FLOAT(0.0), LW(10), 1)
    USER_FUNC(evt_snd::evt_snd_sfxoff, LW(0))
    RETURN()
    EVT_END()

    void SetDisorder(s32 id)
    {
        evtmgr::EvtEntry *evt = evtmgr::evtEntryType(EvtDisorderInitOrClear, 0, 0, 0);
        evt->lw[10] = (s32)id;
        evt->lw[11] = 0;
        evt->lw[12] = FLOAT(1.2);
        return;
    }

    void ClearDisorder(s32 id)
    {
        evtmgr::EvtEntry *evt = evtmgr::evtEntryType(EvtDisorderInitOrClear, 0, 0, 0);
        evt->lw[10] = (s32)id;
        evt->lw[11] = 1;
        evt->lw[12] = FLOAT(0.6);
        return;
    }

    void DecideDisorder(s32 num, s32 difficulty)
    {
        // Check for debug mode first
        s32 debugDisorderId = swdrv::swByteGet(1660);
        if (DebugMode && debugDisorderId > 0)
        {
            Lunatic->Luna.DW.preId = debugDisorderId;
            swdrv::swByteSet(1660, 0);
            return;
        }
        // Roll through each difficulty to decide whether or not to set a disorder
        s32 compare;
        switch (difficulty)
        {
        case 0:
            compare = 30;
            break;
        case 1:
            compare = 50;
            break;
        case 2:
            compare = 100;
            break;
        default:
            compare = 333;
            break;
        }
        if (num == 999)
            num = system::irand(999);
        if (num < compare)
        {
            s32 disorderRNG = system::rand() % DISORDER_PURPLE + 1;
            Lunatic->Luna.DW.preId = disorderRNG;
        }
        return;
    }

    void DisorderHandleTremors()
    {
        s32 odds = system::rand() % 1000;
        if (odds < Lunatic->Luna.Disorder->shakeOdds && Lunatic->Luna.DW.tremorState == 0)
        {
            Lunatic->Luna.DW.tremorState = 1;
            s32 time = (Lunatic->Luna.Disorder->shakeTime - ((s32)msl::math::floor(system::rand() % (Lunatic->Luna.Disorder->shakeTime / 2))));
            if (time < 2000)
                time = 2000;
            Lunatic->Luna.DW.finalShakeTime = time;
            evtmgr::EvtEntry *evt = evtmgr::evtEntryType(EvtDisorderTremor, 0, 0, 0);
            evt->lw[10] = time;
            evt->lw[12] = FLOAT(Lunatic->Luna.Disorder->shakeStrength);
        }
        return;
    }
}