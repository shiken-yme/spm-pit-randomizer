#pragma once

#include <common.h>
#include <evt_cmd.h>

namespace mod
{
    struct ApathyWork
    {
        f32 marioHpMult;
        s32 storedHP;
        s32 enemyDamageIncrease;
        s32 marioDamageDecrease;
        f32 enemyMaxHPMult;
        s32 dispMaxHPDecrease;
        s32 dispEnemyHPIncrease;
        s32 storedCritRate;
        f32 storedCritMult;
    };

    struct DreadWork
    {
        s32 dispBlockChance;
    };

    struct PrejudiceWork
    {
        s32 coinLossChance;
        s32 coinThreshold;
        s32 dispInstantCoinLoss;
    };

    struct IndifferenceWork
    {
        s32 repeat;
    };

    struct RecalcitranceWork
    {
        s32 dispXPMult;
        s32 dispHealingNerf;
    };

    struct DepravityWork
    {
        s32 allLv4FloorThreshold;
    };

    struct IndolenceWork
    {
        s32 attackEffectChance;
        s32 slowDuration;
        s32 dispDmgPctBonus;
    };

    struct MelancholyWork
    {
        bool placeholder;
    };

    struct RuinWork
    {
        bool placeholder;
    };

    void *DisorderDataGetPtr();
    void DecideDisorder(s32 rng, s32 difficulty);
    void SetDisorder(s32 id);
    void ClearDisorder(s32 id);
    void ClearDisorderSub(s32 id);
    void DisorderHandleTremors();
    EVT_DECLARE_USER_FUNC(DisorderDraw, 0)
    EVT_DECLARE_USER_FUNC(DisorderGetId, 1)
    EVT_DECLARE_USER_FUNC(DisorderGetPreId, 1)
    EVT_DECLARE_USER_FUNC(DisorderGetFloorsRem, 1)
    EVT_DECLARE(dan_disorder_indifference)

    void PrejudiceAction();
    EVT_DECLARE_USER_FUNC(IndifferenceAction, 8)
    EVT_DECLARE_USER_FUNC(RecalcitranceCalcHealing, 2)
    EVT_DECLARE_USER_FUNC(DepravityAction, 0)
    s32 DepravityGetAllLv4Threshold(s32 difficulty);
    bool DepravityCheckActive();
}
