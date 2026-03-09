#include "mod.h"
#include <common.h>
#include <lp_common.h>
#include <evtpatch.h>
#include <tplpatch.h>
#include <evt_cmd.h>
#include "patch.h"
#include <util.h>
#include <msgpatch.h>
#include <customwin.h>
#include <globalop.h>
#include <rfcdrv.h>
#include <lunadrv.h>
#include <interface.h>

#include "lunatic/localize.h"
#include <gen.h>
#include <rewrite.h>

#include <cutscene_helpers.h>
#include <spm/rel/aa1_01.h>
#include <spm/rel/mi4.h>
#include <spm/rel/relocatable_module.h>
#include <spm/temp_unk.h>
#include <spm/animdrv.h>
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
#include <spm/fadedrv.h>
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
#include <spm/wpadmgr.h>
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
    using namespace customwin;

    void LPGUIDrawText(f32 x, f32 y, f32 scale, u8 alpha, wii::gx::GXColor color, bool rainbow, const char *msg)
    {
        if (alpha > 0)
            fontmgr::FontDrawStart_alpha(alpha);
        else
            fontmgr::FontDrawStart();
        fontmgr::FontDrawEdge();
        fontmgr::FontDrawColor(&color);
        fontmgr::FontDrawScale(scale);
        fontmgr::FontDrawNoiseOff();
        if (rainbow)
            fontmgr::FontDrawRainbowColor();
        else
            fontmgr::FontDrawRainbowColorOff();
        fontmgr::FontDrawString(x, y, msg);
        return;
    }

    void youSuckDisplay(f32 offset)
    {
        if (!Lunatic->Interface.youSuck)
            return;
        const char *youSuckText = "YOU SUCK";
        f32 scale = 3.69f;
        LPGUIDrawText(-((fontmgr::FontGetMessageWidth(youSuckText) * scale) / 2), (0.0f + (offset * 2)), scale, 0, {255, 255, 255, 255}, true, youSuckText);
    }

    void new_dan_gameover()
    {
        Lunatic->Interface.youSuck = true;
    }

    void disorderDisplay(f32 offset)
    {
        f32 x = -385.0f;
        s32 disorderNum = (s32)Lunatic->Luna.disorder;
        if (disorderNum > 0)
        {
            wii::mtx::Vec3 position = {x, (-230.0f - (offset / 1.5f)), 0.0f};
            s32 mainIconId = (disorderNum - 1 + TPLPATCH_ICON(ICON_DISORDER_APATHY));
            icondrv::iconDispGxAlpha(0.64f, &position, 0x10, mainIconId, 200);
            icondrv::iconDispGxAlpha(0.64f, &position, 0x10, TPLPATCH_ICON(ICON_BORDER_DISORDER), 225);
        }
        s32 disorderRooms = Lunatic->Luna.DW.floorsRem;
        if (disorderRooms > 0)
        {
            wii::gx::GXColor funnyColor = {Lunatic->Luna.Disorder->mainCol.r, Lunatic->Luna.Disorder->mainCol.g, Lunatic->Luna.Disorder->mainCol.b, 255};
            char buffer[4];
            msl::stdio::sprintf(buffer, "%d", disorderRooms);
            const char *msg = buffer;
            x -= 4.0f;
            if (disorderRooms == 1)
                x += 1.0f;
            LPGUIDrawText(x, (-185.0f - (offset / 1.5f)), 0.64f, 200, funnyColor, true, msg);
        }
    }

    void voucherDisplay(f32 offset)
    {
        f32 y = -230.0f;
        f32 x = 385.0f;
        wii::mtx::Mtx34 mtxPos, mtxRot, mtxScale;
        for (s32 i = 0; i < VOUCHER_MAX; i += 1)
        {
            if (Lunatic->Voucher.Work[i] != nullptr)
            {
                if (Lunatic->Voucher.Work[i]->iconId != 0)
                {
                    wii::mtx::PSMTXTrans(mtxPos, x, y, 0.0f);
                    wii::mtx::PSMTXScale(mtxScale, 0.73f, 0.73f, 0.73f);
                    wii::mtx::PSMTXRotRad((Lunatic->Voucher.Work[i]->iconRotation * PI / 180.0f), mtxRot, 0x79); // 'y'
                    wii::mtx::PSMTXConcat(mtxPos, mtxScale, mtxPos);
                    wii::mtx::PSMTXConcat(mtxPos, mtxRot, mtxPos);
                    icondrv::iconDispGxCol(mtxPos, 0x10, TPLPATCH_ICON(Lunatic->Voucher.Work[i]->iconId), {255, 255, 255, Lunatic->Voucher.Work[i]->iconAlpha});
                    y += 32.0f;
                }
            }
        }
    }

    void critDisplay()
    {
        f32 slide = -100.0f;
        if (Lunatic->Interface.critDispStartDisp)
        {
            if (!Lunatic->Interface.critDisp)
            {
                Lunatic->Interface.critDisp = true;
                Lunatic->Interface.critDispProgress = 0;
            }
            slide += system::intplGetValue(system::INTPL_MODE_QUADRATIC_OUT, 0.0f, 140.0f, Lunatic->Interface.critDispProgress, 30);
            if (Lunatic->Interface.critDispProgress < 30)
                Lunatic->Interface.critDispProgress += 1;
        }
        else
        {
            if (Lunatic->Interface.critDisp)
            {
                Lunatic->Interface.critDisp = false;
                Lunatic->Interface.critDispProgress = 0;
                Lunatic->Interface.critDispSlideOutAdj = 140.0f;
            }
            slide -= system::intplGetValue(system::INTPL_MODE_QUADRATIC_IN, 0.0f, 140.0f, Lunatic->Interface.critDispProgress, 30);
            slide += Lunatic->Interface.critDispSlideOutAdj;
            if (Lunatic->Interface.critDispProgress < 30)
                Lunatic->Interface.critDispProgress += 1;
            else
                Lunatic->Interface.critDispSlideOutAdj = 0.0f;
        }

        f32 x = -420.0f + slide;
        f32 y = 105.0f;
        const char *fmt = "%s: %d%%";

        const char *interfaceCM[2] = {interfaceCM1, interfaceCM2};
        const char *interfaceCR[2] = {interfaceCR1, interfaceCR2};
        const char *interfaceDEF[2] = {interfaceDEF1, interfaceDEF2};
        const char *interfaceDR[2] = {interfaceDR1, interfaceDR2};

        // Mult
        wii::mtx::Vec3 position = {x, y, 0.0f};
        icondrv::iconDispGxAlpha(0.6f, &position, 0x10, TPLPATCH_ICON(ICON_SPIRIT_2), 200);
        char multBuf[24];
        msl::stdio::sprintf(multBuf, fmt, interfaceCM[(s32)swdrv::swGet(1632)], (s32)Lunatic->Stats.CritMult);
        const char *multMsg = multBuf;
        LPGUIDrawText((x + 15.0f), (position.y + 20.0f), 0.7f, 0, {58, 158, 255, 255}, false, multMsg);

        // Rate
        position.y -= 25.0f;
        icondrv::iconDispGxAlpha(0.6f, &position, 0x10, TPLPATCH_ICON(ICON_SOUL_2), 225);
        char rateBuf[24];
        msl::stdio::sprintf(rateBuf, fmt, interfaceCR[(s32)swdrv::swGet(1632)], Lunatic->Stats.CritRate);
        const char *rateMsg = rateBuf;
        LPGUIDrawText((x + 15.0f), (position.y + 20.0f), 0.7f, 0, {251, 211, 0, 255}, false, rateMsg);

        // Defense
        position.y -= 25.0f;
        icondrv::iconDispGxAlpha(0.6f, &position, 0x10, TPLPATCH_ICON(ICON_AEGIS_2), 225);
        char defBuf[24];
        msl::stdio::sprintf(defBuf, "%s: %d", interfaceDEF[(s32)swdrv::swGet(1632)], Lunatic->Stats.AegisDef);
        const char *defMsg = defBuf;
        LPGUIDrawText((x + 15.0f), (position.y + 20.0f), 0.7f, 0, {96, 100, 196, 255}, false, defMsg);

        // Damage Reduction
        position.y -= 25.0f;
        icondrv::iconDispGxAlpha(0.6f, &position, 0x10, TPLPATCH_ICON(ICON_AUSPICE_2), 225);
        char drBuf[24];
        msl::stdio::sprintf(drBuf, fmt, interfaceDR[(s32)swdrv::swGet(1632)], round(Lunatic->Stats.AuspiceDR));
        const char *drMsg = drBuf;
        LPGUIDrawText((x + 15.0f), (position.y + 20.0f), 0.7f, 0, {232, 67, 122, 255}, false, drMsg);
    }

    mario::MarioWork *danDisplay()
    {
        if (msl::string::strstr(spmario::gp->mapName, "dan") != nullptr)
        {
            f32 offset = hud::hud_wp->basePos.y;
            disorderDisplay(offset);
            voucherDisplay(offset);
            youSuckDisplay(offset);
            critDisplay();
        }
        return mario::marioGetPtr();
    }

    static seqdef::SeqFunc *seq_titleMainReal;
    void seq_titleMainOverride(seqdrv::SeqWork *wp)
    {
        const char *msg = MOD_VERSION;
        const char *debugMsg = "LP Debug Mode activated! ^w^";
        wii::gx::GXColor notgreen = {230, 116, 216, 255};
        f32 scale = 0.8f;
        f32 y = 200.0f;
        f32 x;
        if (!DebugMode)
        {
            x = -((fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            LPGUIDrawText(x, y, scale, 0, notgreen, false, msg);
        }
        else
        {
            x = -((fontmgr::FontGetMessageWidth(debugMsg) * scale) / 2);
            LPGUIDrawText(x, y, scale, 0, notgreen, true, debugMsg);
        }
        // Hotkey to enable debug mode
        if (!DebugMode && (wpadmgr::wpadGetButtonsHeld(0) & WPAD_BTN_Z) != 0 && (wpadmgr::wpadGetButtonsPressed(0) & WPAD_BTN_B) != 0)
        {
            DebugMode = true;
            spmario_snd::spsndSFXOn("SFX_I_BRUNK_APPEAR1");
        }
        seq_titleMainReal(wp);
    }

    void danYouSuck()
    {
        patch::hookFunction(dan::danCountdownDone, new_dan_gameover);
    }

    s32 LPGUIShowHideStats(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 showHide = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        showHide == 1 ? Lunatic->Interface.critDispStartDisp = true : Lunatic->Interface.critDispStartDisp = false;
        return 2;
    }

    /*EVT_BEGIN(LPGUIShowStats)
    USER_FUNC(LPGUIShowHideStats, 1)
    RETURN()
    EVT_END()

    EVT_BEGIN(LPGUIHideStats)
    USER_FUNC(LPGUIShowHideStats, 0)
    RETURN()
    EVT_END()*/

    bool DoNothing(winmgr::WinmgrSelect *sel)
    {
        (void)sel;
        return false;
    }

    s32 LPGUIUnpause(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        (void)evtEntry;
        pausewin::pausewinUnpauseGame();
        hud::hudUnhide();
        return 2;
    }
    EVT_DECLARE_USER_FUNC(LPGUIUnpause, 0)

    s32 GetEffectInfo(evtmgr::EvtEntry *evtEntry, bool firstRun)
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
                evtmgr_cmd::evtSetValue(evtEntry, args[2], (s32)Lunatic->Luna.disorder - 1 + TPLPATCH_ICON(ICON_DISORDER_APATHY));
                evtmgr_cmd::evtSetValue(evtEntry, args[3], (s32)Lunatic->Luna.Disorder->name);
                msl::string::memset(Lunatic->Luna.DW.descBuf, 0, sizeof(Lunatic->Luna.DW.descBuf));
                LeyLineDisorder *Disorders = (LeyLineDisorder *)DisorderDataGetPtr();
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
        case 3: // Voucher
            RFCItemData *RFC_SpecialItems = (RFCItemData *)RFCSpecialGetPtr();
            s32 i = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
            if (Lunatic->Voucher.Work[i] != nullptr)
            {
                evtmgr_cmd::evtSetValue(evtEntry, args[2], TPLPATCH_ICON((s32)Lunatic->Voucher.Work[i]->iconId));
                evtmgr_cmd::evtSetValue(evtEntry, args[3], (s32)RFC_SpecialItems[Lunatic->Voucher.Work[i]->itemId].name);
                evtmgr_cmd::evtSetValue(evtEntry, args[4], (s32)RFC_SpecialItems[Lunatic->Voucher.Work[i]->itemId].description);
                evtmgr_cmd::evtSetValue(evtEntry, args[5], (s32)&RFC_SpecialItems[Lunatic->Voucher.Work[i]->itemId].textDrawCol);
                break;
            }
            else
                evtmgr_cmd::evtSetValue(evtEntry, args[2], 0);
            break;
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(GetEffectInfo, 6)

    EVT_BEGIN(LPGUIActiveEffects)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 255, 128, 1000)
    INLINE_EVT()
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    USER_FUNC(evt_map::evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    END_INLINE()
    WAIT_MSEC(500)
    USER_FUNC(LPGUIShowHideStats, 1)
    USER_FUNC(EvtCWSelectEntry, PTR("Active"), CWSELECT_DEFAULT, PTR("Active Effects"), PTR(""), 0, 0)
    USER_FUNC(EvtCWSelectHideDescWindow, PTR("Active"))
    USER_FUNC(EvtCWSelectOverrideSelectionBehavior, PTR("Active"), PTR(DoNothing))
    // Disorder: effect type, disorder id, icon id, name, description, text col
    USER_FUNC(GetEffectInfo, 2, LW(2), LW(5), LW(6), LW(7), LW(9))
    IF_LARGE(LW(2), 0)
    SET(LW(8), 1)
    USER_FUNC(EvtCWSelectAddListing, PTR("Active"), LW(6), LW(7), LW(5), 0, 0, LW(9))
    END_IF()
    // Voucher: effect type, voucher idx, icon id, name, description, text col
    SET(LW(2), 0)
    DO(VOUCHER_MAX)
    USER_FUNC(GetEffectInfo, 3, LW(2), LW(5), LW(6), LW(7), LW(9))
    IF_LARGE(LW(5), 0)
    SET(LW(8), 1)
    USER_FUNC(EvtCWSelectAddListing, PTR("Active"), LW(6), LW(7), LW(5), 0, 0, LW(9))
    END_IF()
    ADD(LW(2), 1)
    WHILE()
    IF_EQUAL(LW(8), 0) // If 1, indicates that a listing was created
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(activeConditionsNone), 0, 0)
    ELSE()
    USER_FUNC(EvtCWSelectMenuStart, PTR("Active"), 0, 0)
    USER_FUNC(EvtCWSelectReset)
    END_IF()
    USER_FUNC(EvtCWSelectDelete, PTR("Active"))
    USER_FUNC(LPGUIShowHideStats, 0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 128, 255, 500)
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    USER_FUNC(evt_map::evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    USER_FUNC(LPGUIUnpause)
    RETURN()
    EVT_END()

    void danInterfaceMain()
    {
        mario::MarioWork *mario = mario::marioGetPtr();
        // Handle opening the config menu
        {
            if (((mario->buttonsPressed & WPAD_BTN_B) == WPAD_BTN_B) && ((mario->buttonsHeld & WPAD_BTN_Z) != WPAD_BTN_Z)) // B pressed, Z not held
            {
                bool canPause = mario_motion::func_80146f0c();
                bool keyOff = mario::marioKeyOffChk();
                bool ctrlOff = mario::marioCtrlOffChk();
                bool noFade = fadedrv::fadeIsFinish();
                if (noFade && !keyOff && !ctrlOff && canPause && msl::string::strstr(spmario::gp->mapName, "dan") != nullptr)
                {
                    pausewin::pausewinPauseGame();
                    hud::hudHide();
                    evtmgr::evtEntryType(LPGUIActiveEffects, 0, 0, 0);
                }
            }
        }
    }

    void guiOverrides()
    {
        seq_titleMainReal = seqdef::seq_data[seqdrv::SEQ_TITLE].main;
        seqdef::seq_data[seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
        writeBranchLink(hud::hudDisp, 0x54, danDisplay);
        globalop::globalopAddEntry((void *)danInterfaceMain, nullptr);
    }
}