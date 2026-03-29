#include <common.h>
#include <spm/item_data.h>
#include <spm/mario_pouch.h>
#include <spm/msgdrv.h>
#include <spm/icondrv.h>
#include <spm/npcdrv.h>
#include <tplpatch.h>
#include <wii/os.h>

#include <mod.h>

namespace mod
{
    /*
        Library for basic utility functions used all across proprietary LP files
    */

    using namespace spm;

    // Input needs to be positive
    s32 round(f32 in)
    {
        s32 left = (s32)in;
        in -= left;
        if (in >= 0.5f)
            in = 1.0f;
        else
            in = 0.0f;
        s32 out = left + in;
        return out;
    }

    s32 clamp(s32 input, s32 min, s32 max)
    {
        if (input < min)
            return min;
        else if (input > max)
            return max;
        else
            return input;
    }

    void lpAddAtk(s32 atk)
    {
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();    
        s32 preAtk = pouch->attack;
        pouch->attack += atk;
        Lunatic->Stats.DemiseATK += (pouch->attack - preAtk);
        return;
    }

    void lpAddHp(s32 maxHp, s32 hp)
    {
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();    
        s32 preHp = pouch->maxHp;
        pouch->maxHp += maxHp;
        Lunatic->Stats.DelightHP += (pouch->maxHp - preHp);
        pouch->hp += hp;
        if (pouch->hp > pouch->maxHp)
            pouch->hp = pouch->maxHp;
        return;
    }

    void lpAddCrit(s32 rate, f32 mult)
    {
        Lunatic->Stats.CritRate += rate;
        Lunatic->Stats.CritMult += mult;
        Lunatic->Stats.CritRate = clamp(Lunatic->Stats.CritRate, -100, 100);
        return;
    }

    const char *npcTribeToName(s32 tribeId)
    {
        return msgdrv::msgSearch(item_data::itemDataTable[npcdrv::npcTribes[tribeId].catchCardItemId].nameMsg);
    }

    s32 ActiveEffectsToggleIconB(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 onOff = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        if (onOff > 0)
            msgdrv::msgdrv_msgIcon[3].iconId = TPLPATCH_ICON(ICON_B);
        else
            msgdrv::msgdrv_msgIcon[3].iconId = icondrv::ICON_BTN_1;
        return 2;
    }

}
