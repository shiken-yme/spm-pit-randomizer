#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <customwin.h>
#include <lunadrv.h>
#include <gen.h>

namespace mod
{
    enum BlessId
    {
        /* 0x0 */ MERLUNA_NULL_BLESS,
        /* 0x1 */ MERLUNA_SPECTRE,
        /* 0x2 */ MERLUNA_HOURAI,
        /* 0x3 */ MERLUNA_PARAMITA
    };

    enum CurseId
    {
        /* 0x0 */ MERLUNA_NULL_CURSE,
        /* 0x1 */ MERLUNA_SHION,
        /* 0x2 */ MERLUNA_HEX,
        /* 0x3 */ MERLUNA_MIGRAINE
    };

    enum DisorderId
    {
        /* 0x0 */ DISORDER_NULL,
        /* 0x1 */ DISORDER_RED,
        /* 0x2 */ DISORDER_ORANGE,
        /* 0x3 */ DISORDER_YELLOW,
        /* 0x4 */ DISORDER_GREEN,
        /* 0x5 */ DISORDER_CYAN,
        /* 0x6 */ DISORDER_BLUE,
        /* 0x7 */ DISORDER_PURPLE,
        /* 0x8 */ DISORDER_WHITE,
        /* 0x9 */ DISORDER_BLACK
    };

    enum LPIcon
    {
        /* 0x0 */ ICON_SKULL_KEY,
        /* 0x1 */ ICON_BUMP_GRAY,
        /* 0x2 */ ICON_BUMP_BLUE,
        /* 0x3 */ ICON_BORDER_BLESSING,
        /* 0x4 */ ICON_BORDER_CURSE,
        /* 0x5 */ ICON_BORDER_DISORDER,
        /* 0x6 */ ICON_DISORDER_APATHY,
        /* 0x7 */ ICON_DISORDER_DREAD,
        /* 0x8 */ ICON_DISORDER_PREJUDICE,
        /* 0x9 */ ICON_DISORDER_INDIFFERENCE,
        /* 0xA */ ICON_DISORDER_RECALCITRANCE,
        /* 0xB */ ICON_DISORDER_DEPRAVITY,
        /* 0xC */ ICON_DISORDER_INDOLENCE,
        /* 0xD */ ICON_DISORDER_MELANCHOLY,
        /* 0xE */ ICON_DISORDER_RUIN,
        /* 0xF */ ICON_B
    };

    typedef void(Callback)(void);

    struct LPSessionMiscs
    {
        bool youSuck;
        f64 boobies;
    };

    struct BoodinBalls
    {
        customwin::CWSelectItemDesc Cards[15];
        s32 cardNum; // includes perma SP
    };

    struct CooldownTimer
    {
        s32 frames;
        s32 floors;
    };

    struct CriticalStrike
    {
        s32 Rate; // base value of 4 when initialized
        f32 Mult; // base value of 50.0 when initialized
    };

    struct JudgementWork
    {
        const char *name;
        CooldownTimer CD;
        s32 activateThreshold;
        Callback *SetFunc;
        Callback *ClearFunc;
    };

    struct LeyLineDisorder
    {
        const char *name;
        const char *desc;
        wii::gx::GXColor mainCol; // Default color
        wii::gx::GXColor severeCol; // Fades to this when the screen randomly shakes, fades back when it's done
        wii::gx::GXColor textDrawCol; // Text displays as this color in menus
        s32 shakeOdds; // Checks if a random int from 0-1000 is below this value every second to determine if the screen should shake
        f32 shakeStrength; // Shakes the screen on the x/y axes at this intensity maximum, half this much minimum
        s32 shakeTime; // Shakes the screen for this many ms maximum, half this many minimum
        Callback *SetFunc;
        Callback *ClearFunc;
    };

    struct DisorderMiscs
    {
        s32 floorsRem;
        s32 preId;
        s32 intplProgressMax;
        s32 intplProgress;
        s32 tremorState;
        s32 finalShakeTime;
        s32 tremorIntplFrmMax;
        s32 tremorIntplFrmTimer;
        char descBuf[300];
        union
        {
            ApathyWork *Apathy;
            DreadWork *Dread;
            PrejudiceWork *Prejudice;
            IndifferenceWork *Indifference;
            RecalcitranceWork *Recalcitrance;
            DepravityWork *Depravity;
            IndolenceWork *Indolence;
            MelancholyWork *Melancholy;
            RuinWork *Ruin;
            void *Any;
        } UserWork;
    };

    struct InvisibleFullMoon
    {
        BlessId blessing;
        CurseId curse;
        DisorderId disorder;
        JudgementWork *Blessing;
        JudgementWork *Curse;
        LeyLineDisorder *Disorder;
        DisorderMiscs DisorderWork;
    };

    struct RestFloorChest
    {
        bool closeChest;
        s32 rfcItems[4];
    };

    struct MoverWork
    {
        s32 moverRNG;
    };

    // todo: merge DanNPCData/similar structs and data into this
    struct LunaticPitWork
    {
        FloorData Floor[200];
        LPSessionMiscs Misc;
        BoodinBalls Boodin;
        InvisibleFullMoon Luna;
        RestFloorChest RFC;
        MoverWork Mover;
        CriticalStrike Crit;
    };

    extern LunaticPitWork *Lunatic;

    extern bool DebugMode;

    EVT_DECLARE(fwd_mover_speech)
    EVT_DECLARE(custom_pit_music)

    EVT_DECLARE_USER_FUNC(rand100, 1)
    EVT_DECLARE_USER_FUNC(handleBlessingWearOff, 0)
    EVT_DECLARE_USER_FUNC(get_mover_rng, 1)
    EVT_DECLARE_USER_FUNC(set_mover_rng, 0)
    EVT_DECLARE_USER_FUNC(evt_dan_init_lunatic, 0)
    EVT_DECLARE_USER_FUNC(DebugModeGetStatus, 1)

    s32 danClearMapParts();
    void main();
}
