#pragma once
#include <common.h>
#include <evt_cmd.h>
#include <spm/npcdrv.h>
#include <wii/os.h>

namespace mod
{
    using namespace spm;

    enum NPCDanFlag : u32
    {
        DAN_NPC_HOLOGRAPHIC = 0x1,
        DAN_NPC_NEGATIVE = 0x2
    };

    s32 round(f32 in);
    s32 clamp(s32 input, s32 min, s32 max);
    void lpAddAtk(s32 atk);
    void lpAddHp(s32 maxHp, s32 hp);
    void lpAddCrit(s32 rate, f32 mult);
    bool npcIsShellEnemy(npcdrv::NPCEntry *npc);
    bool npcCheckDanFlag(npcdrv::NPCEntry *npc, NPCDanFlag flag);
    void npcSetDanFlag(npcdrv::NPCEntry *npc, NPCDanFlag flag);
    void npcClearDanFlag(npcdrv::NPCEntry *npc, NPCDanFlag flag);
    void npcMakeHolo(npcdrv::NPCEntry *npc);
    void npcMakeNegative(npcdrv::NPCEntry *npc);
    const char *npcTribeToName(s32 tribeId);

    EVT_DECLARE_USER_FUNC(MsgIconReplaceIdx, 2)

}