#include <common.h>
#include <lp_common.h>
#include <mod.h>
#include <msl/math.h>
#include <spm/animdrv.h>
#include <spm/filemgr.h>
#include <spm/icondrv.h>
#include <spm/item_data.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/memory.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <spm/rel/mi4.h>
#include <tplpatch.h>
#include <wii/os.h>
#include <wii/tpl.h>

namespace mod {
    /*
        Library for basic utility functions used all across proprietary LP files
    */

    using namespace spm;

    // Input needs to be positive
    s32 round(f32 in) {
        s32 left = (s32)in;
        in -= left;
        if (in >= 0.5f)
            in = 1.0f;
        else
            in = 0.0f;
        s32 out = left + in;
        return out;
    }

    s32 clamp(s32 input, s32 min, s32 max) {
        if (input < min)
            return min;
        else if (input > max)
            return max;
        else
            return input;
    }

    // Splits a positive number into separate digits in order & returns number of digits
    s32 split(s32 in, s32 * out) {
        if (in == 0)
            return 1;
        s32 i = -1;
        for (s32 n = in; n > 0; n /= 10) {
            i += 1;
        }
        s32 num = i + 1;
        // wii::os::OSReport("split(%d): i = %d, num = %d\n", in, i, num);
        for (s32 n = in; i >= 0; i -= 1) {
            out[i] = n % 10;
            n /= 10;
        }
        // wii::os::OSReport("split(%d): num = %d, out = {%d, %d, %d, %d, ...}\n", in, num, out[0], out[1], out[2], out[3]);
        return num;
    }

    f32 abs_value(f32 in) {
        if (in < 0.0f)
            in *= -1.0f;
        return in;
    }

    wii::tpl::TPLHeader * allocTPL(const char * fileName, const char * folderName, memory::Heap heap, bool bind) {
        filemgr::FileEntry * file = nullptr;
        if (folderName != nullptr)
            file = filemgr::fileAllocf(0, "./%s/%s", folderName, fileName);
        else
            file = filemgr::fileAllocf(0, "./%s", fileName);
        u32 heapSize = ((u32)memory::memory_wp->heapEnd[heap] - (u32)memory::memory_wp->heapStart[heap]);
        if (heap == memory::HEAP_MEM1_UNUSED && heapSize < file->length) {
            heap = memory::HEAP_EFFECT;
            heapSize = ((u32)memory::memory_wp->heapEnd[heap] - (u32)memory::memory_wp->heapStart[heap]);
        }
        assertf(file->length < heapSize, "Heap %d Overflow!! [%s] (0x%x > 0x%x)", heap, fileName, file->length, heapSize);
        wii::tpl::TPLHeader * tpl = (wii::tpl::TPLHeader *)memory::__memAlloc(heap, file->length);
        msl::string::memcpy(tpl, file->sp->data, file->length);
        filemgr::fileFree(file);
        if (bind)
            wii::tpl::TPLBind(tpl);
        return tpl;
    }

    void lpAddAtk(s32 atk) {
        mario_pouch::MarioPouchWork * pouch = mario_pouch::pouchGetPtr();
        s32 preAtk = pouch->attack;
        pouch->attack += atk;
        Lunatic->Stats.DemiseATK += (pouch->attack - preAtk);
        return;
    }

    void lpAddHp(s32 maxHp, s32 hp) {
        mario_pouch::MarioPouchWork * pouch = mario_pouch::pouchGetPtr();
        s32 preHp = pouch->maxHp;
        pouch->maxHp += maxHp;
        Lunatic->Stats.DelightHP += (pouch->maxHp - preHp);
        pouch->hp += hp;
        if (pouch->hp > pouch->maxHp)
            pouch->hp = pouch->maxHp;
        return;
    }

    void lpAddCrit(s32 rate, f32 mult) {
        Lunatic->Stats.CritRate += rate;
        Lunatic->Stats.CritMult += mult;
        Lunatic->Stats.CritRate = clamp(Lunatic->Stats.CritRate, -100, 100);
        return;
    }

    s32 evt_lp_add_crit(evtmgr::EvtEntry * evtEntry, bool firstRun) {
        (void)firstRun;
        evtmgr::EvtVar * args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 rate = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        f32 mult = evtmgr_cmd::evtGetFloat(evtEntry, args[1]);
        lpAddCrit(rate, mult);
        return 2;
    }

    bool npcIsShellEnemy(npcdrv::NPCEntry * npc) {
        if (npc->templateKouraKickScript != nullptr)
            return true;
        return false;
    }

    bool npcCheckHasKey(npcdrv::NPCEntry * npc, bool checkForChestKey, s32 itemIdToChk) {
        s32 item = itemIdToChk;
        if (item == 0)
            item = npc->dropItemId;
        if (checkForChestKey && item == ITEM_ID_KEY_MAC_KEY_00)
            return true;
        if (item == ITEM_ID_KEY_DAN_KEY)
            return true;
        return false;
    }

    bool npcCheckDanFlag(npcdrv::NPCEntry * npc, NPCDanFlag flag) {
        if (((u32)npc->unkShellSfx & flag) != 0)
            return true;
        return false;
    }

    void npcSetDanFlag(npcdrv::NPCEntry * npc, NPCDanFlag flag) {
        u32 f = (u32)npc->unkShellSfx;
        f |= flag;
        npc->unkShellSfx = (const char *)f;
        return;
    }

    void npcClearDanFlag(npcdrv::NPCEntry * npc, NPCDanFlag flag) {
        u32 f = (u32)npc->unkShellSfx;
        f &= ~flag;
        npc->unkShellSfx = (const char *)f;
        return;
    }

    void npcMakeHolo(npcdrv::NPCEntry * npc) {
        npcSetDanFlag(npc, DAN_NPC_HOLOGRAPHIC);
        npc->maxHp *= 4;
        npc->hp *= 4;
        if ((npc->m_Anim).m_nPoseId != -1)
            animdrv::animPoseSetDispCallback2((npc->m_Anim).m_nPoseId, (void *)mi4::mi4MimiHolographicEffect, nullptr);
        npcdrv::NPCPart * part = npc->parts;
        while (part != nullptr) {
            if (part->m_Anim.m_nPoseId != -1)
                animdrv::animPoseSetDispCallback2((part->m_Anim).m_nPoseId, (void *)mi4::mi4MimiHolographicEffect, nullptr);
            part = part->nextPart;
        }
        return;
    }

    void npcMakeNegative(npcdrv::NPCEntry * npc) {
        npcSetDanFlag(npc, DAN_NPC_NEGATIVE);
        if (npc->maxHp >= 2) {
            npc->maxHp = round((f32)npc->maxHp / 3.0f);
            if (npc->maxHp == 0)
                npc->maxHp = 1;
            npc->hp = npc->maxHp;
        }
        if ((npc->m_Anim).m_nPoseId != -1)
            animdrv::animPoseSetDispCallback2((npc->m_Anim).m_nPoseId, (void *)DanEnemyNegativeDispCb, nullptr);
        npcdrv::NPCPart * part = npc->parts;
        while (part != nullptr) {
            if (part->m_Anim.m_nPoseId != -1)
                animdrv::animPoseSetDispCallback2((part->m_Anim).m_nPoseId, (void *)DanEnemyNegativeDispCb, nullptr);
            part = part->nextPart;
        }
        return;
    }

    const char * npcTribeToName(s32 tribeId) {
        return msgdrv::msgSearch(item_data::itemDataTable[npcdrv::npcTribes[tribeId].catchCardItemId].nameMsg);
    }

    s32 evt_mario_set_invincibility(evtmgr::EvtEntry * evtEntry, bool firstRun) {
        (void)firstRun;
        evtmgr::EvtVar * args = (evtmgr::EvtVar *)evtEntry->pCurData;
        f32 frames = evtmgr_cmd::evtGetFloat(evtEntry, args[0]);
        s32 type = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        mario::marioSetInvincibility(frames, type);
        return 2;
    }

    s32 ToggleGSWF(evtmgr::EvtEntry * evtEntry, bool firstRun) {
        (void)firstRun;
        evtmgr::EvtVar * args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 gswf = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        if (swdrv::swGet(gswf) == false)
            swdrv::swSet(gswf);
        else
            swdrv::swClear(gswf);
        return 2;
    }

    s32 MsgIconReplaceIdx(evtmgr::EvtEntry * evtEntry, bool firstRun) {
        (void)firstRun;
        evtmgr::EvtVar * args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 idx = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 iconId = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        msgdrv::msgdrv_msgIcon[idx].iconId = iconId;
        return 2;
    }

}
