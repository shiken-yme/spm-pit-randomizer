#include "mod.h"
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
        Lunatic->Luna.DisorderWork.UserWork.Apathy = wp;
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
        wp->storedCritRate = (s32)msl::math::floor((f32)Lunatic->Crit.Rate / 2.0);
        Lunatic->Crit.Rate -= wp->storedCritRate;
        wp->storedCritMult = msl::math::floor((f32)Lunatic->Crit.Mult / 2.0);
        Lunatic->Crit.Mult -= wp->storedCritMult;
        return;
    }

    void ApathyClear()
    {
        ApathyWork *wp = Lunatic->Luna.DisorderWork.UserWork.Apathy;
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        pouch->maxHp += wp->storedHP;
        pouch->hp += (s32)(wp->storedHP * wp->marioHpMult);
        Lunatic->Crit.Rate += wp->storedCritRate;
        Lunatic->Crit.Mult += wp->storedCritMult;
        return;
    }

    void DreadSet()
    {
        DreadWork *wp = (DreadWork *)memory::__memAlloc(0, sizeof(DreadWork));
        msl::string::memset(wp, 0, sizeof(DreadWork));
        Lunatic->Luna.DisorderWork.UserWork.Dread = wp;
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
        Lunatic->Luna.DisorderWork.UserWork.Prejudice = wp;
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
        f32 mod = (f32)(system::rand() % 11 + (Lunatic->Luna.DisorderWork.UserWork.Prejudice->dispInstantCoinLoss - 5));
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
            if (pouch->coins > 0 && odds < Lunatic->Luna.DisorderWork.UserWork.Prejudice->coinLossChance)
                pouch->coins -= 1;
        }
        return;
    }

    void IndifferenceSet()
    {
        IndifferenceWork *wp = (IndifferenceWork *)memory::__memAlloc(0, sizeof(IndifferenceWork));
        msl::string::memset(wp, 0, sizeof(IndifferenceWork));
        Lunatic->Luna.DisorderWork.UserWork.Indifference = wp;
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
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        u8 difficulty2 = swdrv::swByteGet(1620);
        u8 loops = 1;
        if (difficulty2 == 2)
        {
            loops = 2;
        }
        u8 thresh = 0;
        u8 j = 0;
        s32 indiffItems[] = {83, 95, 98, 160, 174, 175, 176, 178};
        const char *indiffINames[] = {"indiff_i1", "indiff_i2"};
        s32 indiffItemIdx = 0;
        u8 itemsRmd = 0;
        u8 itemsAdded = 0;
        while (loops != j)
        {
            switch (difficulty2)
            {
            case 0:
                thresh = 25;
                break;
            case 1:
                thresh = 50;
                break;
            case 2:
                thresh = 50;
                break;
            }
            s32 odds = system::rand() % 100;
            if (thresh > odds)
            {
                indiffItemIdx = system::rand() % 8;
                mario::MarioWork *mario = mario::marioGetPtr();
                if ((mario_pouch::pouchCountUseItems() + itemsAdded) < 10)
                {
                    itemdrv::ItemEntry *item = itemdrv::itemEntry(indiffINames[itemsAdded], indiffItems[indiffItemIdx], 0, mario->position.x, mario->position.y, mario->position.z, NULL, 0);
                    item->flags = (item->flags | 0x800);
                    itemsAdded = itemsAdded + 1;
                }
                else
                {
                    s32 invIdx = system::rand() % 10;
                    mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
                    itemsRmd = itemsRmd + 1;
                    evtmgr_cmd::evtSetValue(evtEntry, args[itemsRmd], msgdrv::msgSearch(item_data::itemDataTable[pouch->useItem[invIdx]].nameMsg));
                    mario_pouch::pouchRemoveItemIdx(pouch->useItem[invIdx], invIdx);
                    itemdrv::ItemEntry *item = itemdrv::itemEntry(indiffINames[itemsAdded], indiffItems[indiffItemIdx], 0, mario->position.x, mario->position.y, mario->position.z, NULL, 0);
                    item->flags = (item->flags | 0x800);
                    itemsAdded = itemsAdded + 1;
                }
            }
            j = j + 1;
        }
        evtmgr_cmd::evtSetValue(evtEntry, args[0], itemsRmd);
        evtmgr_cmd::evtSetValue(evtEntry, args[3], itemsAdded);
        return 2;
    }

    void RecalcitranceSet()
    {
        RecalcitranceWork *wp = (RecalcitranceWork *)memory::__memAlloc(0, sizeof(RecalcitranceWork));
        msl::string::memset(wp, 0, sizeof(RecalcitranceWork));
        Lunatic->Luna.DisorderWork.UserWork.Recalcitrance = wp;
        s32 difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            wp->dispXpPct = 50;
            wp->dispReturnPostage = 10;
            wp->maxRetPostDmg = 2;
            break;
        case 1:
            wp->dispXpPct = 100;
            wp->dispReturnPostage = 20;
            wp->maxRetPostDmg = 4;
            break;
        case 2:
            wp->dispXpPct = 150;
            wp->dispReturnPostage = 30;
            wp->maxRetPostDmg = 8;
            break;
        default:
            wp->dispXpPct = 200;
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
        Lunatic->Luna.DisorderWork.UserWork.Depravity = wp;
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

    void IndolenceSet()
    {
        IndolenceWork *wp = (IndolenceWork *)memory::__memAlloc(0, sizeof(IndolenceWork));
        msl::string::memset(wp, 0, sizeof(IndolenceWork));
        Lunatic->Luna.DisorderWork.UserWork.Indolence = wp;
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
            wp->slowDuration = 10;
            break;
        case 2:
            wp->attackEffectChance = 75;
            wp->dispDmgPctBonus = 100;
            wp->slowDuration = 15;
            break;
        default:
            wp->attackEffectChance = 100;
            wp->dispDmgPctBonus = 100;
            wp->slowDuration = 20;
            break;
        }
        return;
    }

    // Establish global data tables for Blessings, Curses, and Disorders

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
            {ruinName, ruinDesc, {0, 0, 0, 30}, {0, 0, 0, 50}, {10, 10, 10, 255}, 75, 1.5, 6666, nullptr, nullptr}                                    // RUIN
    };

    void SetDisorderSub(DisorderId id)
    {
        Lunatic->Luna.disorder = id;
        Lunatic->Luna.DisorderWork.preId = 0;
        Lunatic->Luna.DisorderWork.floorsRem = 5;
        Lunatic->Luna.Disorder = &Disorders[(s32)id - 1];
        if (Lunatic->Luna.Disorder->SetFunc != nullptr)
            (Lunatic->Luna.Disorder->SetFunc)();
        return;
    }

    void ClearDisorderSub(s32 id)
    {
        if (Lunatic->Luna.Disorder->ClearFunc != nullptr)
            (Lunatic->Luna.Disorder->ClearFunc)();
        if (Lunatic->Luna.DisorderWork.UserWork.Any != nullptr)
            memory::__memFree(0, Lunatic->Luna.DisorderWork.UserWork.Any);
        Lunatic->Luna.disorder = DISORDER_NULL;
        Lunatic->Luna.Disorder = nullptr;
        return;
    }

    s32 DisorderGetId(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], (s32)Lunatic->Luna.disorder);
        return 2;
    }

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
                msl::string::memset(Lunatic->Luna.DisorderWork.descBuf, 0, sizeof(Lunatic->Luna.DisorderWork.descBuf));
                switch (Lunatic->Luna.disorder)
                {
                case DISORDER_RED:
                    msl::stdio::sprintf(Lunatic->Luna.DisorderWork.descBuf, Disorders[0].desc, Lunatic->Luna.DisorderWork.UserWork.Apathy->dispMaxHPDecrease, Lunatic->Luna.DisorderWork.UserWork.Apathy->dispEnemyHPIncrease, Lunatic->Luna.DisorderWork.UserWork.Apathy->enemyDamageIncrease, Lunatic->Luna.DisorderWork.UserWork.Apathy->marioDamageDecrease);
                    break;
                case DISORDER_ORANGE:
                    msl::stdio::sprintf(Lunatic->Luna.DisorderWork.descBuf, Disorders[1].desc, Lunatic->Luna.DisorderWork.UserWork.Dread->dispBlockChance);
                    break;
                case DISORDER_YELLOW:
                    msl::stdio::sprintf(Lunatic->Luna.DisorderWork.descBuf, Disorders[2].desc, Lunatic->Luna.DisorderWork.UserWork.Prejudice->dispInstantCoinLoss, Lunatic->Luna.DisorderWork.UserWork.Prejudice->coinLossChance, Lunatic->Luna.DisorderWork.UserWork.Prejudice->coinThreshold);
                    break;
                case DISORDER_GREEN:
                    msl::stdio::sprintf(Lunatic->Luna.DisorderWork.descBuf, Disorders[3].desc, Lunatic->Luna.DisorderWork.UserWork.Indifference->repeat);
                    break;
                case DISORDER_CYAN:
                    msl::stdio::sprintf(Lunatic->Luna.DisorderWork.descBuf, Disorders[4].desc, Lunatic->Luna.DisorderWork.UserWork.Recalcitrance->dispXpPct, Lunatic->Luna.DisorderWork.UserWork.Recalcitrance->dispReturnPostage, Lunatic->Luna.DisorderWork.UserWork.Recalcitrance->maxRetPostDmg);
                    break;
                case DISORDER_BLUE:
                    msl::stdio::sprintf(Lunatic->Luna.DisorderWork.descBuf, Disorders[5].desc, Lunatic->Luna.DisorderWork.UserWork.Depravity->allLv4FloorThreshold);
                    break;
                case DISORDER_PURPLE:
                    msl::stdio::sprintf(Lunatic->Luna.DisorderWork.descBuf, Disorders[6].desc, Lunatic->Luna.DisorderWork.UserWork.Indolence->attackEffectChance, Lunatic->Luna.DisorderWork.UserWork.Indolence->dispDmgPctBonus, Lunatic->Luna.DisorderWork.UserWork.Indolence->slowDuration);
                    break;
                default:
                    break;
                }
                evtmgr_cmd::evtSetValue(evtEntry, args[4], (s32)Lunatic->Luna.DisorderWork.descBuf);
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
        switch (Lunatic->Luna.DisorderWork.tremorState)
        {
        case 1:                                                    // Tremor fadein
            if (Lunatic->Luna.DisorderWork.tremorIntplFrmMax == 0) // Init variables
            {
                Lunatic->Luna.DisorderWork.tremorIntplFrmMax = (s32)msl::math::floor((f32)(Lunatic->Luna.DisorderWork.finalShakeTime / 1000 * 60));
                Lunatic->Luna.DisorderWork.tremorIntplFrmTimer = 0;
                Lunatic->Luna.DisorderWork.intplProgressMax = 60;
                Lunatic->Luna.DisorderWork.intplProgress = 0;
            }
            red = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.r, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.r, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            green = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.g, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.g, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            blue = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.b, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.b, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            alpha = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.a, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.a, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            Lunatic->Luna.DisorderWork.tremorIntplFrmTimer += 1;
            Lunatic->Luna.DisorderWork.intplProgress += 1;
            if (Lunatic->Luna.DisorderWork.tremorIntplFrmTimer >= 60)
            {
                Lunatic->Luna.DisorderWork.intplProgress = 0;
                Lunatic->Luna.DisorderWork.tremorState = 2;
            }
            break;
        case 2: // Stay at severecol
            red = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.r, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.r, Lunatic->Luna.DisorderWork.intplProgressMax, Lunatic->Luna.DisorderWork.intplProgressMax);
            green = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.g, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.g, Lunatic->Luna.DisorderWork.intplProgressMax, Lunatic->Luna.DisorderWork.intplProgressMax);
            blue = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.b, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.b, Lunatic->Luna.DisorderWork.intplProgressMax, Lunatic->Luna.DisorderWork.intplProgressMax);
            alpha = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.a, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.a, Lunatic->Luna.DisorderWork.intplProgressMax, Lunatic->Luna.DisorderWork.intplProgressMax);
            Lunatic->Luna.DisorderWork.tremorIntplFrmTimer += 1;
            if ((Lunatic->Luna.DisorderWork.tremorIntplFrmMax - Lunatic->Luna.DisorderWork.tremorIntplFrmTimer) <= 60) // Check if should fade back to normal
                Lunatic->Luna.DisorderWork.tremorState = 3;
            break;
        case 3: // Tremor fadeout
            red = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.r, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.r, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            green = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.g, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.g, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            blue = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.b, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.b, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            alpha = system::intplGetValue(system::INTPL_MODE_LINEAR, (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.a, (f32)Disorders[Lunatic->Luna.disorder - 1].severeCol.a, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            red = Disorders[Lunatic->Luna.disorder - 1].severeCol.r - red + (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.r;
            green = Disorders[Lunatic->Luna.disorder - 1].severeCol.g - green + (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.g;
            blue = Disorders[Lunatic->Luna.disorder - 1].severeCol.b - blue + (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.b;
            alpha = Disorders[Lunatic->Luna.disorder - 1].severeCol.a - alpha + (f32)Disorders[Lunatic->Luna.disorder - 1].mainCol.a;
            Lunatic->Luna.DisorderWork.intplProgress += 1;
            Lunatic->Luna.DisorderWork.tremorIntplFrmTimer += 1;
            if (Lunatic->Luna.DisorderWork.tremorIntplFrmTimer == Lunatic->Luna.DisorderWork.tremorIntplFrmMax)
            {
                Lunatic->Luna.DisorderWork.tremorIntplFrmMax = 0;
                Lunatic->Luna.DisorderWork.tremorIntplFrmTimer = 0;
                Lunatic->Luna.DisorderWork.tremorState = 0;
                Lunatic->Luna.DisorderWork.intplProgress = 0;
                Lunatic->Luna.DisorderWork.intplProgressMax = 0;
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
        if (Lunatic->Luna.DisorderWork.intplProgressMax == 0)
        {
            Lunatic->Luna.DisorderWork.intplProgressMax = 100;
        }
        if (Lunatic->Luna.DisorderWork.intplProgress < Lunatic->Luna.DisorderWork.intplProgressMax)
        {
            red = system::intplGetValue(system::INTPL_MODE_LINEAR, 0.0f, (f32)Disorders[id - 1].mainCol.r, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            green = system::intplGetValue(system::INTPL_MODE_LINEAR, 0.0f, (f32)Disorders[id - 1].mainCol.g, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            blue = system::intplGetValue(system::INTPL_MODE_LINEAR, 0.0f, (f32)Disorders[id - 1].mainCol.b, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            alpha = system::intplGetValue(system::INTPL_MODE_LINEAR, 0.0f, (f32)Disorders[id - 1].mainCol.a, Lunatic->Luna.DisorderWork.intplProgress, Lunatic->Luna.DisorderWork.intplProgressMax);
            if (clear == 1)
            {
                red = -(red - Disorders[id - 1].mainCol.r);
                green = -(green - Disorders[id - 1].mainCol.g);
                blue = -(blue - Disorders[id - 1].mainCol.b);
                alpha = -(alpha - Disorders[id - 1].mainCol.a);
            }
            wii::gx::GXColor color = {(u8)red, (u8)green, (u8)blue, (u8)alpha};
            mapdrv::mapSetColor(&color);
            Lunatic->Luna.DisorderWork.intplProgress += 1;
            // DisorderDraw will attempt to draw after this interpolation ends & before ClearDisorderSub, so we set disorder to null directly beforehand
            if (Lunatic->Luna.DisorderWork.intplProgress == 99)
                Lunatic->Luna.disorder = DISORDER_NULL;
            return 0;
        }
        else
        {
            Lunatic->Luna.DisorderWork.intplProgressMax = 0;
            Lunatic->Luna.DisorderWork.intplProgress = 0;
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
    USER_FUNC(evt_cam::evt_cam_shake, camdrv::CAM_ID_3D, LW(12), LW(12), FLOAT(0.0), 3000, 0)
    USER_FUNC(evt_snd::evt_snd_sfxoff, LW(0))
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_7"), 0)
    WAIT_MSEC(700)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    WAIT_MSEC(300)
    USER_FUNC(evt_npc::evt_npc_unfreeze_all)
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
    EVT_END()

    EVT_BEGIN(EvtDisorderTremor)
    USER_FUNC(evt_snd::evt_snd_sfxon, PTR("SFX_EVT_QUAKE1L"))
    USER_FUNC(evt_snd::evt_snd_get_last_sfx_id, LW(0))
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
        // Roll through each difficulty to decide whether or not to set a disorder
        s32 disorderRNG = system::rand() % DISORDER_PURPLE + 1;
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
            compare = 990;
            break;
        default:
            compare = 333;
            break;
        }
        if (num < compare)
            Lunatic->Luna.DisorderWork.preId = disorderRNG;
        return;
    }

    void DisorderHandleTremors()
    {
        s32 odds = system::rand() % 1000;
        if (odds < Lunatic->Luna.Disorder->shakeOdds && Lunatic->Luna.DisorderWork.tremorState == 0)
        {
            Lunatic->Luna.DisorderWork.tremorState = 1;
            s32 time = (Lunatic->Luna.Disorder->shakeTime - ((s32)msl::math::floor(system::rand() % (Lunatic->Luna.Disorder->shakeTime / 2))));
            if (time < 2000)
                time = 2000;
            Lunatic->Luna.DisorderWork.finalShakeTime = time;
            evtmgr::EvtEntry *evt = evtmgr::evtEntryType(EvtDisorderTremor, 0, 0, 0);
            evt->lw[10] = time;
            evt->lw[12] = FLOAT(Lunatic->Luna.Disorder->shakeStrength);
        }
        return;
    }
}