#include <berobero.h>

#include "patch.h"

#include <common.h>
#include <evt_cmd.h>
#include <msl/math.h>
#include <msl/stdio.h>
#include <msl/string.h>
#include <spm/acdrv.h>
#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <spm/casedrv.h>
#include <spm/charmwindow.h>
#include <spm/dispdrv.h>
#include <spm/eff/eff_3d_switch.h>
#include <spm/eff/eff_ddtas_block.h>
#include <spm/eff/eff_map_block_del.h>
#include <spm/eff/eff_small_star.h>
#include <spm/eff/eff_spm_hit.h>
#include <spm/eff/eff_spm_levelup.h>
#include <spm/eff/eff_spm_spindash.h>
#include <spm/eff_sub.h>
#include <spm/effdrv.h>
#include <spm/evt_case.h>
#include <spm/evt_door.h>
#include <spm/evt_item.h>
#include <spm/evt_mobj.h>
#include <spm/evt_pouch.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/fadedrv.h>
#include <spm/filemgr.h>
#include <spm/fontmgr.h>
#include <spm/framedrv.h>
#include <spm/hitdrv.h>
#include <spm/levelupwindow.h>
#include <spm/map_data.h>
#include <spm/mario.h>
#include <spm/mario_fairy.h>
#include <spm/mario_hit.h>
#include <spm/mario_motion.h>
#include <spm/mario_pouch.h>
#include <spm/mario_sbr.h>
#include <spm/mobjdrv.h>
#include <spm/mot_fairy_mario.h>
#include <spm/mot_jump.h>
#include <spm/mot_slit.h>
#include <spm/mot_swim.h>
#include <spm/mot_walk.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <spm/seq_mapchange.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <spm/wpadmgr.h>
#include <wii/gx.h>
#include <wii/kpad.h>
#include <wii/os.h>
#include <wii/tpl.h>
#include <wii/wpad.h>

namespace mod::bero {
    using namespace spm;
    using namespace spm::evt_door;

    Dokan * Dokans[BERO_DOKAN_MAX];

    /*
        Persistent in memory; will automatically spawn the pipe and add to dokan descs on specified
       map entry
    */
    Dokan * beroDokanEntry(const char * name, const char * mapName, const char * destMapName, const char * destBeroName, f32 x, f32 y, f32 z,
                           f32 baseObjHeight, DokanScriptNum orientation, const char * headModelName, const char * baseModelName, DokanSpawnChkCb * spawnChk, u16 dokanFlags) {
        s32 i;
        Dokan dokan = {{orientation, dokanFlags, 0, name, mapName, nullptr, nullptr, destMapName, destBeroName}, {x, y, z}, baseObjHeight, headModelName, baseModelName, nullptr, spawnChk, nullptr, nullptr, nullptr, 0};
        for (i = 0; i < BERO_DOKAN_MAX; i += 1) {
            if (Dokans[i] == nullptr)
                break;
        }
        assertf(i < BERO_DOKAN_MAX, "Entry Overflow !! (%d > %d)", i, BERO_DOKAN_MAX);
        Dokans[i] = (Dokan *)memory::__memAlloc(memory::HEAP_MAIN, sizeof(Dokan));
        msl::string::memcpy(Dokans[i], &dokan, sizeof(Dokan));
        return Dokans[i];
    }

    void beroCreateDokan(Dokan * entry) {
        // Create the MOBJ entries for the head and the tail
        mobjdrv::mobjEntry(entry->Desc.name, entry->headModelName);
        mobjdrv::MobjEntry * dokan = mobjdrv::mobjNameToPtr(entry->Desc.name);
        entry->headMobj = dokan;
        dokan->pos = entry->pos;
        dokan->flag0 |= 0x40; // disables culling
        // Assign rotation
        switch (entry->Desc.scriptNumber) {
        case IN_DOWN:
            dokan->rotation.z = 0.0;
            break;
        case IN_UP:
            dokan->rotation.z = 180.0;
            break;
        case IN_LEFT:
            dokan->rotation.z = 270.0;
            dokan->pos.y += dokan->objSize.x / 2.0f;
            entry->Desc.flags |= 0x2000; // Forcibly face right when exiting
            break;
        default:
            dokan->rotation.z = 90.0;
            dokan->pos.y += dokan->objSize.x / 2.0f;
            entry->Desc.flags |= 0x1000; // Forcibly face left when exiting
            break;
        }
        if (entry->baseModelName != nullptr && entry->baseObjHeight > 0.0f) {
            dokan->flag0 |= 0x2; // disables shadow
            char buf[16];
            msl::stdio::sprintf(buf, "%st", entry->Desc.name);
            const char * baseName = buf;
            mobjdrv::mobjEntry(baseName, entry->baseModelName);
            mobjdrv::MobjEntry * base = mobjdrv::mobjNameToPtr(baseName);
            entry->baseMobj = base;
            base->scale.y = (entry->baseObjHeight / base->objSize.y);
            base->pos = dokan->pos;
            base->flag0 |= 0x40; // disables culling
            switch (entry->Desc.scriptNumber) {
            case IN_DOWN:
                dokan->pos.y += entry->baseObjHeight;
                break;
            case IN_UP:
                base->rotation.z = 180.0;
                dokan->pos.y -= entry->baseObjHeight;
                base->flag0 |= 0x2; // disables shadow
                break;
            case IN_LEFT:
                base->rotation.z = 270.0;
                dokan->pos.x += entry->baseObjHeight;
                break;
            default:
                base->rotation.z = 90.0;
                dokan->pos.x -= entry->baseObjHeight;
                break;
            }
            animdrv::animPoseSetAnim(base->animPoseId, "S_1", 1);
            mobjdrv::mobjCalcMtx(base);
        }
        if (entry->cornerCount > 0) {
            for (s32 i = 0; i < entry->cornerCount; i += 1) {
                char cBuf[16];
                msl::stdio::sprintf(cBuf, "%sc%d", entry->Desc.name, i);
                const char * cornerName = cBuf;
                mobjdrv::mobjEntry(cornerName, entry->cornerModelName);
                mobjdrv::MobjEntry * corner = mobjdrv::mobjNameToPtr(cornerName);
                entry->Corners[i].cornerMobj = corner;
                corner->pos = dokan->pos;
                corner->flag0 |= 0x42;
                corner->rotation.z = (f32)entry->Corners[i].orientation * 90.0f;
                if (i == 0) {
                    if (entry->baseMobj != nullptr)
                        corner->pos = entry->baseMobj->pos;
                } else {
                    if (entry->Corners[i - 1].extraBaseMobj != nullptr)
                        corner->pos = entry->Corners[i - 1].extraBaseMobj->pos;
                }
                CornerShift shift = entry->Corners[i].shift;
                if (shift != SHIFT_NONE) {
                    f32 shiftAmt = (dokan->objSize.y / 2.0f);
                    if (shift <= SHIFT_UP_RIGHT) {
                        corner->pos.y += shiftAmt;
                    } else if (shift <= SHIFT_DOWN_RIGHT) {
                        corner->pos.y -= shiftAmt;
                    }
                    if (shift == SHIFT_LEFT || shift == SHIFT_UP_LEFT || shift == SHIFT_DOWN_LEFT) {
                        corner->pos.x -= shiftAmt;
                    } else if (shift == SHIFT_RIGHT || shift == SHIFT_UP_RIGHT || shift == SHIFT_DOWN_RIGHT) {
                        corner->pos.x += shiftAmt;
                    }
                }
                animdrv::animPoseSetAnim(corner->animPoseId, "S_1", 1);
                mobjdrv::mobjCalcMtx(corner);
            }
        }
        entry->Desc.hitName2d = dokan->joints[1].name;
        entry->Desc.hitName3d = dokan->joints[0].name;
        animdrv::animPoseSetAnim(dokan->animPoseId, "S_1", 1);
        mobjdrv::mobjCalcMtx(dokan);
        return;
    }

    // Hooks into evt_door_handle_spawn to emplace custom dokans and hijack dokan descs to include custom ones
    const char * beroEmplaceCustomDokans(evtmgr::EvtEntry * evtEntry) {
        evtmgr::EvtVar * args = (evtmgr::EvtVar *)evtEntry->pCurData;
        const char * beroName = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        EvtDoorWork * wp = evt_door_wp;
        s32 dokanIdx[15];
        s32 numCustomDokans = 0;
        s32 i;
        for (i = 0; i < BERO_DOKAN_MAX; i += 1) {
            if (Dokans[i] == nullptr)
                continue;
            if (msl::string::strcmp(Dokans[i]->Desc.mapName, spmario::gp->mapName) == 0) {
                // Failsafe to make sure this hook isn't running a duplicate time
                if (mobjdrv::mobjNameToPtrNoAssert(Dokans[i]->Desc.name) != nullptr)
                    return beroName;
                // Run spawncheck callback
                if (Dokans[i]->spawnCheck != nullptr) {
                    bool spawn = (Dokans[i]->spawnCheck)(Dokans[i]);
                    if (!spawn)
                        continue;
                }
                beroCreateDokan(Dokans[i]);
                dokanIdx[numCustomDokans] = i;
                numCustomDokans += 1;
                wii::os::OSReport("Dokan \"%s\" created in map \"%s\" with dokanIdx %d.\n", Dokans[i]->Desc.name, Dokans[i]->Desc.mapName, i);
            }
        }
        if (numCustomDokans == 0)
            return beroName;
        u32 size_total = (sizeof(DokanDesc) * (wp->dokan_desc_num + numCustomDokans));
        DokanDesc * descs = (DokanDesc *)memory::__memAlloc(memory::HEAP_MAP, size_total);
        msl::string::memset(descs, 0, size_total);
        wii::os::OSReport("Memory allocated for %d total dokans @ %p\n", (wp->dokan_desc_num + numCustomDokans), descs);
        if (wp->dokanDescs != nullptr && wp->dokan_desc_num != 0) {
            msl::string::memcpy(descs, wp->dokanDescs, (sizeof(DokanDesc) * (wp->dokan_desc_num)));
            wii::os::OSReport("%d vanilla dokans copied to the beginning of the new DokanDescs array.\n", wp->dokan_desc_num);
        }
        wp->dokanDescUnkBitmask = 0x0;
        wp->dokanDescs = descs;
        wp->dokan_desc_num += numCustomDokans; // important that we define this before calling enable/disable dokan desc
        for (i = 0; i < numCustomDokans; i += 1) {
            msl::string::memcpy(&descs[wp->dokan_desc_num - numCustomDokans + i], &Dokans[dokanIdx[i]]->Desc, sizeof(DokanDesc));
            wii::os::OSReport("Dokan \"%s\" copied to DokanDescs with local idx %d, true Dokan idx %d.\n", Dokans[dokanIdx[i]]->Desc.name, i, dokanIdx[i]);
            // Has a destination map and loading zone and not DOKAN_INACTIVE
            if (Dokans[dokanIdx[i]]->Desc.destMapName != nullptr && Dokans[dokanIdx[i]]->Desc.destBeroName != nullptr && (Dokans[dokanIdx[i]]->Desc.flags & 0x1) == 0) {
                evtmgr::EvtEntry enableDokanEvt;
                s32 lw[2] = {1, (s32)Dokans[dokanIdx[i]]->Desc.name};
                enableDokanEvt.pCurData = lw;
                evt_door_enable_disable_dokan_desc(&enableDokanEvt, true);
                wp->dokan2dCaseIds[wp->dokan_desc_num - numCustomDokans + i] = wp->dokan2dCaseIds[0];
                wp->dokan3dCaseIds[wp->dokan_desc_num - numCustomDokans + i] = wp->dokan3dCaseIds[0];
                wii::os::OSReport("Dokan \"%s\" successfully enabled.\n", Dokans[dokanIdx[i]]->Desc.name);
            }
        }
        return beroName;
    }

    void beroDokanCornerEntry(Dokan * dokan, const char * cornerModelName, CornerOrient orientation, CornerShift shift, f32 extraBaseObjHeight) {
        DokanCorner * corner;
        if (cornerModelName != nullptr)
            dokan->cornerModelName = cornerModelName;
        if (dokan->cornerCount == 0) {
            corner = (DokanCorner *)memory::__memAlloc(0, sizeof(DokanCorner));
            dokan->Corners = corner;
        } else {
            DokanCorner * realloc = (DokanCorner *)memory::__memAlloc(0, (sizeof(DokanCorner) * (dokan->cornerCount + 1)));
            msl::string::memcpy(realloc, dokan->Corners, (sizeof(DokanCorner) * dokan->cornerCount));
            memory::__memFree(0, dokan->Corners);
            dokan->Corners = realloc;
            corner = &dokan->Corners[dokan->cornerCount];
        }
        corner->orientation = orientation;
        corner->extraBaseObjHeight = extraBaseObjHeight;
        corner->shift = shift;
        dokan->cornerCount += 1;
        return;
    }

    /*DokanDesc *beroPatchSameMapDokanExitAnims(evtmgr_cmd::EvtEntry *evtEntry)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        DokanDesc *entrance = (DokanDesc *)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s16 i = 0;
        for (; i < evt_door_wp->dokan_desc_num; i += 1)
        {
            if (msl::string::strcmp(entrance->destBeroName, evt_door_wp->dokanDescs[i].name) == 0)
    // Verify src and dest are in the same map return &evt_door_wp->dokanDescs[i];
        }
        return entrance;
    }*/

    // evtmgr::EvtScriptCode *beroRetDokanUpSameMapEvt()

    /*
        TODO:
        account for GRAV_UP/LEFT/RIGHT
        force Y position
    */
    void beroPatchDokanExitPositions(evtmgr::EvtEntry * evtEntry, f32 z) {
        evtmgr::EvtVar * args = (evtmgr::EvtVar *)evtEntry->pCurData;
        const char * beroName = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 i;
        for (i = 0; i < BERO_DOKAN_MAX; i += 1) {
            if (Dokans[i] == nullptr)
                continue;
            if (msl::string::strcmp(Dokans[i]->Desc.mapName, spmario::gp->mapName) == 0) {
                if (msl::string::strcmp(Dokans[i]->Desc.name, beroName) == 0) {
                    switch (Dokans[i]->Desc.scriptNumber) {
                    case IN_DOWN:
                        evtmgr_cmd::evtSetFloat(evtEntry, args[4], (30.0f + Dokans[i]->baseObjHeight + Dokans[i]->pos.y));
                        break;
                    case IN_UP:
                        evtmgr_cmd::evtSetFloat(evtEntry, args[4], (Dokans[i]->pos.y - (30.0f + Dokans[i]->baseObjHeight + mario_hit::marioGetHeight())));
                        break;
                    case IN_LEFT:
                        evtmgr_cmd::evtSetFloat(evtEntry, args[3], (Dokans[i]->pos.x + Dokans[i]->baseObjHeight + 40.0f));
                        // evtmgr_cmd::evtSetFloat(evtEntry, args[4], (Dokans[i]->pos.y));
                        mario_sbr::marioFaceRight();
                        break;
                    default:
                        evtmgr_cmd::evtSetFloat(evtEntry, args[3], (Dokans[i]->pos.x - Dokans[i]->baseObjHeight - 40.0f));
                        // evtmgr_cmd::evtSetFloat(evtEntry, args[4], (Dokans[i]->pos.y));
                        mario_sbr::marioFaceLeft();
                        break;
                    }
                }
            }
        }
        evtmgr_cmd::evtSetFloat(evtEntry, args[5], z);
        return;
    }

    void beroberoInit() {
        writeBranchLink(evt_door_handle_spawn, 0x28, beroEmplaceCustomDokans);
        // writeBranchLink(evt_door_set_dokan_exit_anim, 0x28, beroPatchSameMapDokanExitAnims);
        // Add support for entering an IN_UP pipe with a same-map destination
        /*writeWord(evt_door_set_dokan_descs, 0x190, ADDI(24, 29, 0x3A60)); // offset to
        evt_door_enter_dokan_down_same_map_evt writeWord(evt_door_set_dokan_descs, 0x194, LI(25,
        0x6)); // IN_UP writeWord(evt_door_set_dokan_descs, 0x198, B(0x10));
        writeWord(evt_door_enable_disable_dokan_desc, 0x198, ADDI(28, 27, 0x3A60)); // offset to
        evt_door_enter_dokan_down_same_map_evt writeWord(evt_door_enable_disable_dokan_desc, 0x19C,
        LI(30, 0x6)); // IN_UP writeWord(evt_door_enable_disable_dokan_desc, 0x1A0, B(0x14));*/
        // Patch custom dokans to spawn you at the correct position (MOBJ collision doesn't get hitchkfiltered in handle_spawn)
        writeBranchLink(evt_door_handle_spawn, 0xC1C, beroPatchDokanExitPositions);
        writeBranchLink(evt_door_handle_spawn, 0xBC0, beroPatchDokanExitPositions);
        // Flipside Tower orientation tests
        bero::beroDokanEntry("dokan_u", "mac_02", "ta3_02", "dokan_skip", 0.0f, 230.0f, 0.0f, 100.0f, IN_UP, "MOBJ_dokan_y", "MOBJ_dokan_yt", nullptr, 0x0);
        bero::beroDokanEntry("dokan_d", "mac_02", "mac_02", "dokan_d", 100.0f, 0.0f, -30.0f, 12.0f, IN_DOWN, "MOBJ_dokan_o", "MOBJ_dokan_ot", nullptr, 0x0);
        bero::beroDokanEntry("dokan_d2", "mac_02", "mac_02", "dokan_d", 0.0f, 0.0f, 30.0f, 0.0f, IN_DOWN, "MOBJ_dokan_p", "MOBJ_dokan_pt", nullptr, 0x0);
        bero::Dokan * jigen = bero::beroDokanEntry("dokan_l", "mac_02", "mac_02", "dokan_d", -350.0f, 0.0f, -60.0f, 30.0f, IN_LEFT, "MOBJ_dokan_j", "MOBJ_dokan_jt", nullptr, 0x0);
        bero::beroDokanCornerEntry(jigen, "MOBJ_dokan_jc", ORIENT_UP_RIGHT, SHIFT_NONE, 20.0f);
        bero::beroDokanEntry("dokan_r", "mac_02", "mac_05", "dokan_c", 500.0f, 0.0f, -90.0f, 60.0f, IN_RIGHT, "MOBJ_dokan_r", "MOBJ_dokan_rt", nullptr, 0x0);
        // 1-1 dokan_m
        bero::beroDokanEntry("dokan_m", "he1_01", "mac_02", "dokan_3", 1150.0f, 100.0f, 50.0f, 0.0f, IN_DOWN, "MOBJ_dokan_m", "MOBJ_dokan_mt", nullptr, 0x0);
        // 3-3 improvement
        bero::beroDokanEntry("dokan_skip", "ta3_02", "ta3_04", "dokan_skim", 0.0f, 2900.0f, 170.0f, 430.0f, IN_UP, "MOBJ_dokan_y", "MOBJ_dokan_yt", nullptr, 0x0);
        bero::beroDokanEntry("dokan_skim", "ta3_04", "ta3_02", "dokan_skip", 120.0f, 0.0f, 0.0f, 0.0f, IN_DOWN, "MOBJ_dokan_y", nullptr, nullptr, 0x0);
        // Bowser's Castle test pipes
        bero::beroDokanEntry("dokan_p", "mac_05", "aa2_02", "dokan_d", 75.0f, 199.92f, -1250.0f, 20.0f, IN_DOWN, "MOBJ_dokan_p", "MOBJ_dokan_pt", nullptr, 0x0);
        bero::beroDokanEntry("dokan_c", "mac_05", "mac_02", "dokan_r", 15.0f, 199.92f, -1250.0f, 20.0f, IN_DOWN, "MOBJ_dokan_c", "MOBJ_dokan_ct", nullptr, 0x0);
        bero::beroDokanEntry("dokan_d", "aa2_02", "mac_05", "dokan_p", 80.0f, 200.0f, 0.0f, 32.0f, IN_UP, "MOBJ_dokan_d", "MOBJ_dokan_dt", nullptr, 0x0);
        bero::beroDokanEntry("dokan_d2", "aa2_02", "mac_05", "dokan_p", -80.0f, 0.0f, 0.0f, 32.0f, IN_DOWN, "MOBJ_dokan_d", "MOBJ_dokan_dt", nullptr, 0x0);
        bero::beroDokanEntry("dokan_d3", "aa2_02", "mac_05", "dokan_p", -240.0f, 0.0f, 0.0f, 32.0f, IN_LEFT, "MOBJ_dokan_d", "MOBJ_dokan_dt", nullptr, 0x0);
        bero::beroDokanEntry("dokan_d4", "aa2_02", "mac_05", "dokan_p", -400.0f, 0.0f, 0.0f, 32.0f, IN_RIGHT, "MOBJ_dokan_d", "MOBJ_dokan_dt", nullptr, 0x0);
        return;
    }
}
