#pragma once
#include <common.h>
#include <evt_cmd.h>
#include <spm/memory.h>
#include <spm/npcdrv.h>
#include <wii/os.h>
#include <wii/tpl.h>

namespace mod {
    using namespace spm;

    #define DAN_ENEMY(tribeId) \
        (tribeId + 1)

    enum NPCDanFlag : u32 {
        DAN_NPC_HOLOGRAPHIC = 0x1,
        DAN_NPC_NEGATIVE = 0x2,
        DAN_NPC_CHILD = 0x4
    };

    s32 round(f32 in);
    s32 clamp(s32 input, s32 min, s32 max);
    s32 split(s32 in, s32 * out);
    f32 abs_value(f32 in);
    wii::tpl::TPLHeader * allocTPL(const char * fileName, const char * folderName, memory::Heap heap, bool bind);
    void lpAddAtk(s32 atk);
    void lpAddHp(s32 maxHp, s32 hp);
    void lpAddCrit(s32 rate, f32 mult);
    EVT_DECLARE_USER_FUNC(evt_lp_add_crit, 2)
    bool npcIsShellEnemy(npcdrv::NPCEntry * npc);
    bool npcCheckHasKey(npcdrv::NPCEntry * npc, bool checkForChestKey, s32 itemIdToChk);
    bool npcCheckDanFlag(npcdrv::NPCEntry * npc, NPCDanFlag flag);
    void npcSetDanFlag(npcdrv::NPCEntry * npc, NPCDanFlag flag);
    void npcClearDanFlag(npcdrv::NPCEntry * npc, NPCDanFlag flag);
    void npcMakeHolo(npcdrv::NPCEntry * npc);
    void npcMakeNegative(npcdrv::NPCEntry * npc);
    const char * npcTribeToName(s32 tribeId);

    EVT_DECLARE_USER_FUNC(ToggleGSWF, 1)
    EVT_DECLARE_USER_FUNC(evt_mario_set_invincibility, 2)
    EVT_DECLARE_USER_FUNC(MsgIconReplaceIdx, 2)

}
