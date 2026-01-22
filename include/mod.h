#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <customwin.h>
#include <lunadrv.h>
#include <gen.h>

namespace mod
{
    enum BlessId : s32
    {
        /* 0x0 */ MERLUNA_NULL_BLESS,
        /* 0x1 */ MERLUNA_SPECTRE,
        /* 0x2 */ MERLUNA_HOURAI,
        /* 0x3 */ MERLUNA_PARAMITA
    };

    enum CurseId : s32
    {
        /* 0x0 */ MERLUNA_NULL_CURSE,
        /* 0x1 */ MERLUNA_SHION,
        /* 0x2 */ MERLUNA_HEX,
        /* 0x3 */ MERLUNA_MIGRAINE
    };

    enum DisorderId : s32
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

    enum LPIcon : s32
    {
        ICON_SKULL_KEY,
        ICON_BUMP_GRAY,
        ICON_BUMP_BLUE,
        ICON_BORDER_BLESSING,
        ICON_BORDER_CURSE,
        ICON_BORDER_DISORDER,
        ICON_DISORDER_APATHY,
        ICON_DISORDER_DREAD,
        ICON_DISORDER_PREJUDICE,
        ICON_DISORDER_INDIFFERENCE,
        ICON_DISORDER_RECALCITRANCE,
        ICON_DISORDER_DEPRAVITY,
        ICON_DISORDER_INDOLENCE,
        ICON_DISORDER_MELANCHOLY,
        ICON_DISORDER_RUIN,
        ICON_B,
        ICON_VOUCHER_CAKE,
        ICON_VOUCHER_THUNDER,
        ICON_VOUCHER_STELLAR,
        ICON_VOUCHER_JUDGEMENT,
        ICON_VOUCHER_RED,
        ICON_VOUCHER_ORANGE,
        ICON_VOUCHER_YELLOW,
        ICON_VOUCHER_GREEN,
        ICON_VOUCHER_CYAN,
        ICON_VOUCHER_BLUE,
        ICON_VOUCHER_PURPLE,
        ICON_VOUCHER_WHITE,
        ICON_VOUCHER_BLACK, // kek
        ICON_SOUL_1,
        ICON_SOUL_2,
        ICON_SOUL_3,
        ICON_SOUL_4,
        ICON_SPIRIT_1,
        ICON_SPIRIT_2,
        ICON_SPIRIT_3,
        ICON_SPIRIT_4,
        ICON_AEGIS_1,
        ICON_AEGIS_2,
        ICON_AUSPICE_1,
        ICON_AUSPICE_2
    };

    enum LPCustomItem : s32
    {
        VOUCHER_CAKE,
        VOUCHER_THUNDER,
        VOUCHER_STELLAR,
        VOUCHER_JUDGEMENT,
        VOUCHER_RED,
        VOUCHER_ORANGE,
        VOUCHER_YELLOW,
        VOUCHER_GREEN,
        VOUCHER_CYAN,
        VOUCHER_BLUE,
        VOUCHER_PURPLE,
        VOUCHER_WHITE,
        VOUCHER_BLACK, // kek
        SOUL_1,
        SOUL_2,
        SOUL_3,
        SOUL_4,
        SPIRIT_1,
        SPIRIT_2,
        SPIRIT_3,
        SPIRIT_4,
        AEGIS_1,
        AEGIS_2,
        AUSPICE_1,
        AUSPICE_2
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

    struct DivineJudgement
    {
        const char *name;
        CooldownTimer CD;
        s32 activateThreshold;
        Callback *SetFunc;
        Callback *ClearFunc;
    };

    struct JudgementMiscs
    {
        bool judgementRendered;
        BlessId preBlessId;
        CurseId preCurseId;
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
        s32 intplProgress; // for fading color between 0 and mainCol
        s32 intplProgressMax;
        s32 tremorState;
        s32 finalShakeTime;
        s32 tremorIntplFrmTimer; // for fading color between mainCol and severeCol
        s32 tremorIntplFrmMax;
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
        } UW;
    };

    struct InvisibleFullMoon
    {
        BlessId blessing;
        CurseId curse;
        DisorderId disorder;
        DivineJudgement *Blessing;
        DivineJudgement *Curse;
        LeyLineDisorder *Disorder;
        JudgementMiscs JW;
        DisorderMiscs DW;
    };

    struct RFCItemData
    {
        LPIcon iconId;
        const char *name;
        const char *description;
        const char *useMsg;
        Callback *useFunc;
    };

    struct RFCColorDef
    {
        wii::gx::GXColor textCol;
        wii::gx::GXColor chestCol;
    };

    struct RestFloorChest
    {
        u32 rerolls;
        s32 chestKeys;
        s32 chestRarity;
        RFCItemData *rfcItemData[3];
        customwin::CWSelectItemDesc rfcItems[3];
    };

    struct MoverWork
    {
        s32 moverRNG;
    };

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
