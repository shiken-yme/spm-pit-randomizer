#include <berobero.h>

#include "patch.h"

#include <common.h>
#include <evt_cmd.h>
#include <spm/acdrv.h>
#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <spm/mario.h>
#include <spm/effdrv.h>
#include <spm/eff_sub.h>
#include <spm/eff/eff_spm_levelup.h>
#include <spm/eff/eff_spm_spindash.h>
#include <spm/eff/eff_spm_hit.h>
#include <spm/mobjdrv.h>
#include <spm/npcdrv.h>
#include <spm/casedrv.h>
#include <spm/dispdrv.h>
#include <spm/evt_case.h>
#include <spm/evt_door.h>
#include <spm/evt_item.h>
#include <spm/evt_mobj.h>
#include <spm/evt_pouch.h>
#include <spm/eff/eff_ddtas_block.h>
#include <spm/eff/eff_map_block_del.h>
#include <spm/eff/eff_small_star.h>
#include <spm/eff/eff_3d_switch.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/fontmgr.h>
#include <spm/hitdrv.h>
#include <spm/filemgr.h>
#include <spm/fadedrv.h>
#include <spm/framedrv.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/charmwindow.h>
#include <spm/levelupwindow.h>
#include <spm/mario_motion.h>
#include <spm/mario_pouch.h>
#include <spm/mot_fairy_mario.h>
#include <spm/mot_jump.h>
#include <spm/mot_slit.h>
#include <spm/mot_swim.h>
#include <spm/map_data.h>
#include <spm/mario_hit.h>
#include <spm/mario_fairy.h>
#include <spm/mot_walk.h>
#include <spm/msgdrv.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <spm/mario_sbr.h>
#include <spm/seq_mapchange.h>
#include <spm/wpadmgr.h>
#include <wii/kpad.h>
#include <wii/wpad.h>
#include <wii/tpl.h>
#include <wii/os.h>
#include <wii/gx.h>
#include <msl/string.h>
#include <msl/stdio.h>
#include <msl/math.h>
#include <cstdio>

namespace mod::bero
{
    using namespace spm;

    Dokan *Dokans[BERO_DOKAN_MAX];

    /*
        Persistent in memory; will automatically spawn the pipe and add to dokan descs on specified map entry
    */
    Dokan *beroDokanEntry(const char *name, const char *mapName, const char *destMapName, const char *destBeroName, f32 x, f32 y, f32 z, f32 scale, evt_door::DokanScriptNum orientation, const char *modelName, DokanSpawnChkCb *spawnChk, u16 dokanFlags)
    {
        s32 i;
        Dokan dokan = {{orientation, dokanFlags, 0, name, mapName, nullptr, nullptr, destMapName, destBeroName}, {x, y, z}, scale, modelName, spawnChk};
        for (i = 0; i < BERO_DOKAN_MAX; i += 1)
        {
            if (Dokans[i] == nullptr)
                break;
        }
        assertf(i < BERO_DOKAN_MAX, "Max dokan entry count of %d reached", BERO_DOKAN_MAX);
        Dokans[i] = (Dokan *)memory::__memAlloc(memory::HEAP_MAIN, sizeof(Dokan));
        msl::string::memcpy(Dokans[i], &dokan, sizeof(Dokan));
        return Dokans[i];
    }

    void beroCreateDokan(Dokan *entry)
    {
        // Create the MOBJ entries for the head and the tail
        mobjdrv::mobjEntry(entry->Desc.name, entry->modelName);
        //    mobjdrv::mobjEntry(tailName, mobjDokanTails[color]);
        mobjdrv::MobjEntry *dokan = mobjdrv::mobjNameToPtr(entry->Desc.name);
        //    mobjdrv::MobjEntry *tail = mobjdrv::mobjNameToPtr(tailName);
        // Init tail position @ given coords, calc position for head, and normalize mtx for all collision
        //    mobjdrv::mobjSetPosition(tailName, x, y, z);
        //    tail->scale.y = scale;
        //    mobjdrv::mobjCalcMtx(tail);
        //    dokan->pos.y = (y + tail->objSize.y);
        // DEBUG
        dokan->scale.x = 1.6;
        dokan->scale.y = 1.2;
        dokan->scale.z = 1.6;
        dokan->pos.x = entry->pos.x;
        dokan->pos.y = entry->pos.y;
        dokan->pos.z = entry->pos.z;
        switch (entry->Desc.scriptNumber)
        {
        case evt_door::IN_DOWN:
            dokan->pos.y = entry->pos.y - 13.0f;
            break;
        case evt_door::IN_UP:
            dokan->pos.y = entry->pos.y + 15.0f;
            break;
        case evt_door::IN_LEFT:
            dokan->pos.x = entry->pos.x - 19.3f;
            break;
        default:
            dokan->pos.x = entry->pos.x - 15.0f;
            break;
        }
        // END DEBUG
        // Assign rotation
        switch (entry->Desc.scriptNumber)
        {
        case evt_door::IN_DOWN:
            dokan->rotation.z = 0.0;
            break;
        case evt_door::IN_UP:
            dokan->rotation.z = 180.0;
            break;
        case evt_door::IN_LEFT:
            dokan->rotation.z = 270.0;
            break;
        default:
            dokan->rotation.z = 90.0;
            break;
        }
        // Finalize: create dokan desc & copy to allocated evtBeroDokans memory, then create case entries
        entry->Desc.hitName2d = dokan->joints[1].name;
        entry->Desc.hitName3d = dokan->joints[0].name;
        animdrv::animPoseSetAnim(dokan->animPoseId, "W_1", 1);
        mobjdrv::mobjCalcMtx(dokan);
        // DEBUG
        switch (entry->Desc.scriptNumber)
        {
        case evt_door::IN_DOWN:
            dokan->pos.y -= 15.0f;
            break;
        case evt_door::IN_UP:
            dokan->pos.y += 15.0f;
            break;
        case evt_door::IN_LEFT:
            dokan->pos.x -= 15.0f;
            break;
        default:
            dokan->pos.x += 15.0f;
            break;
        }
        // END DEBUG
        return;
    }

    // Hooks into evt_door_set_handle_spawn to emplace custom dokans and hijack dokan descs to include custom ones
    const char *beroEmplaceCustomDokans(evtmgr::EvtEntry *evtEntry)
    {
        evt_door::EvtDoorWork *wp = evt_door::evt_door_wp;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        const char *beroName = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 dokanIdx[15];
        s32 numCustomDokans = 0;
        s32 i;
        for (i = 0; i < BERO_DOKAN_MAX; i += 1)
        {
            if (Dokans[i] == nullptr)
                continue;
            if (msl::string::strcmp(Dokans[i]->Desc.mapName, spmario::gp->mapName) == 0)
            {
                if (Dokans[i]->spawnCheck != nullptr)
                {
                    bool spawn = (Dokans[i]->spawnCheck)(Dokans[i]);
                    if (!spawn)
                        continue;
                }
                beroCreateDokan(Dokans[i]);
                dokanIdx[numCustomDokans] = i;
                numCustomDokans += 1;
            }
        }
        if (numCustomDokans == 0)
            return beroName;
        u32 size_total = (sizeof(evt_door::DokanDesc) * (wp->dokan_desc_num + numCustomDokans));
        evt_door::DokanDesc *descs = (evt_door::DokanDesc *)memory::__memAlloc(memory::HEAP_MAP, size_total);
        msl::string::memset(descs, 0, size_total);
        if (wp->dokanDescs != nullptr && wp->dokan_desc_num != 0)
            msl::string::memcpy(descs, wp->dokanDescs, (sizeof(evt_door::DokanDesc) * (wp->dokan_desc_num)));
        wp->dokanDescs = descs;
        wp->dokan_desc_num += numCustomDokans; // important that we define this before calling enable/disable dokan desc
        for (i = 0; i < numCustomDokans; i += 1)
        {
            msl::string::memcpy(&descs[wp->dokan_desc_num - numCustomDokans + i], &Dokans[dokanIdx[i]]->Desc, sizeof(evt_door::DokanDesc));
            evtmgr::EvtEntry enableDokanEvt;
            s32 lw[2] = {1, (s32)Dokans[dokanIdx[i]]->Desc.name};
            enableDokanEvt.pCurData = lw;
            evt_door::evt_door_enable_disable_dokan_desc(&enableDokanEvt, true);
        }
        return beroName;
    }

    /*s32 beroHookDescNum()
    {
        return evt_door::evt_door_wp->dokan_desc_num;
    }*/

    /*
    // Currently deprecated, will eventually reprogram
    s32 evt_bero_dokan(evtmgr::EvtEntry *evtEntry, bool isFirstCall)
    {
        (void)isFirstCall;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        const char *name = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        const char *destMapName = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        const char *destBeroName = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        f32 x = evtmgr_cmd::evtGetFloat(evtEntry, args[3]);
        f32 y = evtmgr_cmd::evtGetFloat(evtEntry, args[4]);
        f32 z = evtmgr_cmd::evtGetFloat(evtEntry, args[5]);
        f32 scale = evtmgr_cmd::evtGetFloat(evtEntry, args[6]);
        s32 orientation = evtmgr_cmd::evtGetValue(evtEntry, args[7]);
        const char *modelName = evtmgr_cmd::evtGetValue(evtEntry, args[8]);
        u16 flags = (u16)evtmgr_cmd::evtGetValue(evtEntry, args[9]);
        // agb async the pipe model/tex, then create the dokan
        s32 agbAsync = animdrv::animGroupBaseAsync(modelName, 0, 0);
        //    s32 agbAsync2 = animdrv::animGroupBaseAsync(mobjDokanTails[color], 0, 0);
        //    if (agbAsync == 0 || agbAsync2 == 0)
        if (agbAsync <= 0)
            return 0;
        // beroCreateDokan(name, destMapName, destBeroName, x, y, z, scale, orientation, modelName, flags);
        return 2;
    }*/

    void beroberoInit()
    {
        writeBranchLink(evt_door::evt_door_handle_spawn, 0x28, beroEmplaceCustomDokans);
        // writeBranchLink(evt_door::evt_door_set_dokan_descs, 0x34, beroHookDescNum);
        return;
    }
}