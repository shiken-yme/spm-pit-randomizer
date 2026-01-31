#include "mod.h"
#include <common.h>
#include <evtpatch.h>
#include <tplpatch.h>
#include <evt_cmd.h>
#include "patch.h"
#include <util.h>
#include <msgpatch.h>
#include <customwin.h>
#include <globalop.h>
#include <lunadrv.h>

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

    static seqdef::SeqFunc *seq_gameMainReal;

    void youSuckDisplay()
    {
        if (Lunatic->Misc.youSuck)
        {
            wii::gx::GXColor funnyColor = {255, 255, 255, 255};
            f32 scale = 2.8f;
            char buffer[50];
            const char *youSuckText = "YOU SUCK";
            msl::stdio::sprintf(buffer, "%s", youSuckText);
            const char *msg = buffer;
            fontmgr::FontDrawStart();
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColor();
            f32 x = -((fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            fontmgr::FontDrawString(x, 200.0f, msg);
        }
    }

    void new_dan_gameover()
    {
        Lunatic->Misc.youSuck = true;
    }

    void merlunaBlessingDisplay()
    {
        s32 blessingRoomCounter = swdrv::swByteGet(1602);
        if (blessingRoomCounter > 0)
        {
            bool roomOnHud = swdrv::swGet(422);
            s32 alpha = 225;
            if (roomOnHud)
            {
                alpha = 80;
            }
            wii::gx::GXColor funnyColor = {18, 227, 178, 255};
            f32 scale = 0.64f;
            char buffer[100];
            s32 blessingNum = swdrv::swByteGet(1600);
            bool houraiActivation = swdrv::swGet(1671);
            // Iterate through exceptional cases that modify message text
            if (houraiActivation)
            {
                funnyColor = {227, 178, 18, 255};
                msl::stdio::sprintf(buffer, "Affliction: Phoenix's Tail");
                if (roomOnHud)
                {
                    alpha = 80;
                }
            }
            else if (blessingNum == BlessId::MERLUNA_PARAMITA)
            {
                s32 paramitaTimer = swdrv::swByteGet(1610);
                if (paramitaTimer > 20)
                {
                    paramitaTimer = paramitaTimer - 20;
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Active, %d)", paramitaTimer);
                }
                else if (paramitaTimer > 0)
                {
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Cooldown, %d)", paramitaTimer);
                }
                else
                {
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Press B to use)");
                }
            }
            else
            {
                const char *blessingNames[] = {"DUMMY", "Spectre", "Hourai Doll", "Paramita"};
                const char *blessingDisp = blessingNames[blessingNum];
                msl::stdio::sprintf(buffer, "Blessing: %s", blessingDisp);
            }
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            fontmgr::FontDrawString(-360, -170.0f, msg);
        }
    }

    void merlunaBlessingNumDisplay()
    {
        s32 blessingRoomCounter = swdrv::swByteGet(1602);
        if (blessingRoomCounter > 0)
        {
            bool roomOnHud = swdrv::swGet(422);
            s32 alpha = 225;
            if (roomOnHud)
            {
                alpha = 80;
            }
            wii::gx::GXColor funnyColor = {8, 110, 102, 255};
            f32 scale = 0.64f;
            char buffer[50];
            bool houraiActivation = swdrv::swGet(1671);
            if (houraiActivation)
            {
                funnyColor = {140, 110, 18, 225};
                if (roomOnHud)
                {
                    funnyColor = {140, 110, 18, 80};
                }
            }
            msl::stdio::sprintf(buffer, "Neutralizes in %d rooms", blessingRoomCounter);
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            fontmgr::FontDrawString(-360, -190.0f, msg);
        }
    }

    void merlunaCurseDisplay()
    {
        s32 curseNum = swdrv::swByteGet(1601);
        if (curseNum > 0)
        {
            bool roomOnHud = swdrv::swGet(422);
            s32 alpha = 225;
            if (roomOnHud)
            {
                alpha = 80;
            }
            wii::gx::GXColor funnyColor = {138, 0, 207, 255};
            f32 scale = 0.64f;
            char buffer[100];
            const char *curseNames[] = {"DUMMY", "Tatarian Aster", "Vulnerability Hex", "Migraine"};
            const char *curseDisp = curseNames[curseNum];
            msl::stdio::sprintf(buffer, "Curse: %s", curseDisp);
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            fontmgr::FontDrawString(-360, -170.0f, msg);
        }
    }

    void merlunaCurseNumDisplay()
    {
        s32 curseNum = swdrv::swByteGet(1601);
        if (curseNum > 0)
        {
            bool roomOnHud = swdrv::swGet(422);
            s32 alpha = 225;
            if (roomOnHud)
            {
                alpha = 80;
            }
            wii::gx::GXColor funnyColor = {71, 4, 99, 255};
            f32 scale = 0.64f;
            char buffer[50];
            msl::stdio::sprintf(buffer, "Pay Merluna to clear");
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            fontmgr::FontDrawString(-360, -190.0f, msg);
        }
    }

    void disorderDisplay()
    {
        s32 disorderNum = (s32)Lunatic->Luna.disorder;
        if (disorderNum > 0)
        {
            wii::mtx::Vec3 position = {-355.0, -215.0, 0.0};
            s32 mainIconId = (disorderNum - 1 + ICON_DISORDER_APATHY + TPLPATCH_ICON_REDIRECT);
            icondrv::iconDispGxAlpha(0.64f, &position, 0x18, mainIconId, 200);
            icondrv::iconDispGxAlpha(0.64f, &position, 0x18, ICON_BORDER_DISORDER + TPLPATCH_ICON_REDIRECT, 225);
        }
        s32 disorderRooms = Lunatic->Luna.DW.floorsRem;
        if (disorderRooms > 0)
        {
            wii::gx::GXColor funnyColor = {Lunatic->Luna.Disorder->mainCol.r, Lunatic->Luna.Disorder->mainCol.g, Lunatic->Luna.Disorder->mainCol.b, 255};
            char buffer[4];
            msl::stdio::sprintf(buffer, "%d", disorderRooms);
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(200);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(0.64f);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColor();
            f32 x = -359.0;
            if (disorderRooms == 1)
                x += 1.0f;
            fontmgr::FontDrawString(x, -170.0f, msg);
        }
    }

    void textDisplay(seqdrv::SeqWork *wp)
    {
        merlunaBlessingDisplay();
        merlunaBlessingNumDisplay();
        merlunaCurseDisplay();
        merlunaCurseNumDisplay();
        disorderDisplay();
        youSuckDisplay();
        seq_gameMainReal(wp);
    }

    static seqdef::SeqFunc *seq_titleMainReal;
    void seq_titleMainOverride(seqdrv::SeqWork *wp)
    {
        wii::gx::GXColor notgreen = {230, 116, 216, 255};
        f32 scale = 0.8f;
        const char *msg = MOD_VERSION;
        fontmgr::FontDrawStart();
        fontmgr::FontDrawEdge();
        fontmgr::FontDrawColor(&notgreen);
        fontmgr::FontDrawScale(scale);
        fontmgr::FontDrawNoiseOff();
        fontmgr::FontDrawRainbowColorOff();
        f32 x = -((fontmgr::FontGetMessageWidth(msg) * scale) / 2);
        fontmgr::FontDrawString(x, 200.0f, msg);
        seq_titleMainReal(wp);
    }

    void danYouSuck()
    {
        patch::hookFunction(dan::danCountdownDone, new_dan_gameover);
    }

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

    EVT_BEGIN(LPGUI_ActiveConditions)
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
    USER_FUNC(EvtCWSelectEntry, PTR("Active"), CWSELECT_DEFAULT, PTR("Active Conditions"), PTR(""), 0, 0)
    USER_FUNC(EvtCWSelectHideDescWindow, PTR("Active"))
    USER_FUNC(EvtCWSelectOverrideSelectionBehavior, PTR("Active"), PTR(DoNothing))
    USER_FUNC(LunaGetConditionInfo, 2, LW(2), LW(5), LW(6), LW(7), LW(9))
    IF_LARGE(LW(2), 0)
    SET(LW(8), 1)
    USER_FUNC(EvtCWSelectAddListing, PTR("Active"), LW(6), LW(7), LW(5), 0, 0, LW(9))
    END_IF()
    IF_EQUAL(LW(8), 0)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(activeConditionsNone), 0, 0)
    ELSE()
    USER_FUNC(EvtCWSelectMenuStart, PTR("Active"), 0, 0)
    USER_FUNC(EvtCWSelectReset)
    END_IF()
    USER_FUNC(EvtCWSelectDelete, PTR("Active"))
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
                    evtmgr::evtEntryType(LPGUI_ActiveConditions, 0, 0, 0);
                }
            }
        }
    }

    void guiOverrides()
    {
        seq_titleMainReal = seqdef::seq_data[seqdrv::SEQ_TITLE].main;
        seqdef::seq_data[seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
        seq_gameMainReal = seqdef::seq_data[seqdrv::SEQ_GAME].main;
        seqdef::seq_data[seqdrv::SEQ_GAME].main = &textDisplay;
        globalop::globalopAddEntry((void *)danInterfaceMain, nullptr);
    }
}