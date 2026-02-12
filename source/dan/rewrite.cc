#include <common.h>
#include <evt_cmd.h>
#include <gen.h>
#include <util.h>
#include <cutscene_helpers.h>
#include <evtpatch.h>
#include <tplpatch.h>
#include <lunatic/localize.h>
#include <rewrite.h>
#include <rfcdrv.h>
#include <lunadrv.h>
#include <mod.h>

#include <spm/rel/aa1_01.h>
#include <spm/rel/mi4.h>
#include <spm/rel/relocatable_module.h>
#include <spm/temp_unk.h>
#include <spm/animdrv.h>
#include <spm/npcdrv.h>
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

    const char *restFloorNpcNames[] = {"Null", "Flimm", "Merluna", "Boodin", "undetermined"};

    npcdrv::NPCTribeAnimDef _moverAnims[] = {
        {0, "stg2_syuuzin_b_S_1"}, // Idle
        {1, "stg2_syuuzin_b_W_1"}, // Walking
        {2, "stg2_syuuzin_b_R_1"}, // Running
        {3, "stg2_syuuzin_b_T_1"}, // Talking (Idle)
        {-1, "stg2_syuuzin_b_Z_1"}};

    using namespace spm::npcdrv;

    s32 evt_dan_try_disorder(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        (void)evtEntry;
        // Clear disorder tremor effects on mapchange
        Lunatic->Luna.DW.tremorIntplFrmMax = 0;
        Lunatic->Luna.DW.tremorIntplFrmTimer = 0;
        Lunatic->Luna.DW.tremorState = 0;
        Lunatic->Luna.DW.intplProgress = 0;
        Lunatic->Luna.DW.intplProgressMax = 0;
        // Roll for Disorders, else decrement floorsRem
        s32 currentFloor = swdrv::swByteGet(1);
        s32 currentFloorLastDigit = currentFloor % 10;
        if (Lunatic->Luna.disorder == DISORDER_NULL && Lunatic->Luna.DW.floorsRem == 0 && currentFloorLastDigit < 4)
        {
            s32 difficulty = swdrv::swByteGet(1620);
            DecideDisorder(Lunatic->Mover.moverRNG, difficulty);
        }
        else if (Lunatic->Luna.DW.floorsRem > 0)
            Lunatic->Luna.DW.floorsRem -= 1;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_dan_try_disorder, 0)

    s32 evt_dan_disorder_set_or_clear(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        (void)evtEntry;
        if (Lunatic->Luna.DW.floorsRem == 0)
        {
            if (Lunatic->Luna.disorder > DISORDER_NULL)
            {
                ClearDisorder((s32)Lunatic->Luna.disorder);
            }
            else if (Lunatic->Luna.DW.preId > 0)
                SetDisorder(Lunatic->Luna.DW.preId);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_dan_disorder_set_or_clear, 0)

    s32 LunaticForceBGMChange(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        (void)firstRun;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 variant = evtmgr_cmd::evtGetValue(evtEntry, args[0]); // 1-4
        spmario_snd::BgmEntry copy = *spmario_snd::spsndBgmPlayers[0].bgmEntry;
        copy.brsarIdx = variant + 1379;
        msl::string::memcpy(spmario_snd::spsndBgmPlayers[0].bgmEntry, &copy, sizeof(copy));
        spmario_snd::spsndBgmPlayers[0].flags |= 1;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(LunaticForceBGMChange, 1)

    // Like so many other functions used in this mod, this was adapted heavily from decomp dan.c
    // Thank you Seeky! This mod and many others would not exist without your work.
    // You are greatly appreciated by all of us in the SPM modding and reverse engineering community.
    s32 evt_dan_handle_key_failsafe_new(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)isFirstCall;
        // Check whether the key exists anywhere
        if (!dan::danCheckEnemyInMapBbox() && !dan::danCheckKeyInMapBbox() &&
            !mario_pouch::pouchCheckHaveItem(48) &&
            !itemdrv::itemCheckForId(48))
        {
            // Spawn the key at Mario's position if not
            mario::MarioWork *mario = mario::marioGetPtr();
            itemdrv::itemEntry(NULL, 48, 1, mario->position.x, mario->position.y, mario->position.z, NULL, 0);
            return 2;
        }
        else
            return 0;
    }
    EVT_DECLARE_USER_FUNC(evt_dan_handle_key_failsafe_new, 0)

    s32 evt_dan_modify_enemy(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[1], 0);
        npcdrv::NPCEntry *npc = npcdrv::npcNameToPtr_NoAssert((const char *)evtmgr_cmd::evtGetValue(evtEntry, args[0]));
        s32 sup = system::rand() % 100;
        s32 currentFloor = swdrv::swByteGet(1);
        s32 tribe = npc->tribeId;
        npcdrv::NPCDropItem *dropItems = npcdrv::npcTribes[tribe].dropItemList;
        // Nerf all random item drops
        s32 difficulty = swdrv::swByteGet(1620);
        if (npc->dropItemId != ITEM_ID_KEY_DAN_KEY || npc->dropItemId != ITEM_ID_KEY_MAC_KEY_00)
        {
            VoucherState vState = VoucherGetStateById(VOUCHER_STELLAR, nullptr);
            if (vState == V_ACTIVE)
            {
                s32 vOdds = system::rand() % 100;
                if (vOdds > 50)
                    npcSetDanFlag(npc, DAN_NPC_STELLARIZED);
            }
            if (npcCheckDanFlag(npc, DAN_NPC_STELLARIZED) == false)
            {
                switch (difficulty)
                {
                case 0:
                    if (sup > 80)
                        npc->dropItemId = 0;
                    break;
                case 1:
                    if (sup > 60)
                        npc->dropItemId = 0;
                    break;
                case 2:
                    if (sup > 10)
                        npc->dropItemId = 0;
                    break;
                }
            }
        }
        if (Lunatic->Luna.disorder == DISORDER_RED) // APATHY
        {
            npc->maxHp = (u32)msl::math::floor((f32)npc->maxHp * Lunatic->Luna.DW.UW.Apathy->enemyMaxHPMult);
            npc->hp = npc->maxHp;
        }
        sup = system::rand() % 100;
        // Create holographic enemy
        // DEBUG: sup > -1 && currentFloor > -1 && (npc->maxHp >= 1 || npcdrv::npcTribes[npc->tribeId].attackStrength >= 0)
        // NORMAL: sup > 95 && currentFloor > 149 && (npc->maxHp >= 10 || npcdrv::npcTribes[npc->tribeId].attackStrength >= 3)
        if (sup > 95 && currentFloor > 149 && (npc->maxHp >= 10 || npcdrv::npcTribes[npc->tribeId].attackStrength >= 3))
        {
            if ((s32)npc != 0 && npc->templateUnkScript9 == 0 && npc->tribeId != 200 && npc->tribeId != 201 && npc->tribeId != 32 && npc->tribeId != 142 && npc->tribeId != 144 && npc->tribeId != 146 && npc->tribeId != 504 && npc->tribeId != 156 && npc->tribeId != 157 && npc->tribeId != 188 && npc->tribeId != 189 && npc->tribeId != 184 && npc->tribeId != 185)
            {
                npc->maxHp *= 2;
                npc->hp *= 2;
                npcSetDanFlag(npc, DAN_NPC_HOLOGRAPHIC);
                sup = system::rand() % 100;
                if (sup > 25)
                {
                    if (npc->dropItemId == 0 && dropItems[0].itemId != 0) // If it doesn't already have an item, continue
                    {
                        sup = 1;
                        s32 i = 0;
                        for (i = 0; sup != 0; i += 1)
                            sup = dropItems[i].itemId;
                        do
                        {
                            sup = system::rand() % i;
                            npc->dropItemId = dropItems[sup].itemId;
                            if (npc->dropItemId == 0x57)
                            {
                                s32 sup2 = system::rand() % 100;
                                if (sup2 > 25)
                                    npc->dropItemId = 0x53; // 75% chance to replace Catch Card drops with Dried Shrooms
                            }
                        } while (dropItems[sup].itemId == 0);
                    }
                }
                evtmgr_cmd::evtSetValue(evtEntry, args[1], 1);
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_dan_modify_enemy, 2)

    // heavily adapted from dan.c decomp, thanks again Seeky!
    s32 evt_dan_distribute_keys(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)isFirstCall;
        (void)entry;
        s32 i = 0, n = 0, j = 0, k = 0, currentFloor = swdrv::swByteGet(1), phase = 0, enemiesInCycle = 0;
        bool assign = false, randomKeyAssigned = false;
        npcdrv::NPCWork *npcWp = npcdrv::npcGetWorkPtr();
        NPCEntry *curNpc = npcWp->entries;
        s32 enemyCount = 0;
        NPCEntry *enemies[80];
        if ((currentFloor % 10) == 0)
        {
            msl::string::memset(&Lunatic->RFC.chestKeysToSpawn[0], 0xff, 8);
            s32 difficulty = swdrv::swByteGet(1620);
            s32 guaranteedFloors = __builtin_abs(difficulty - 4);
            for (i = 0; i < guaranteedFloors; i += 1)
            {
            rerollFloor:
                u8 rand = (u8)(system::rand() % 9);
                assign = true;
                for (j = 0; j < 8; j += 1)
                {
                    if (rand == Lunatic->RFC.chestKeysToSpawn[j])
                        assign = false;
                }
                if (assign)
                    Lunatic->RFC.chestKeysToSpawn[i] = rand;
                else
                    goto rerollFloor;
            }
            wii::os::OSReport("%d guaranteed chest keys for this cycle @ rooms ending in %d, %d, %d, %d\n", guaranteedFloors, Lunatic->RFC.chestKeysToSpawn[0] + 1, Lunatic->RFC.chestKeysToSpawn[1] + 1, Lunatic->RFC.chestKeysToSpawn[2] + 1, Lunatic->RFC.chestKeysToSpawn[3] + 1);
        }
        // Create list of enemies to give keys in the current room
        for (i = 0; i < npcWp->num; curNpc++, i++)
        {
            if (CHECK_ANY_MASK(curNpc->flag8, 0x1) && !CHECK_ANY_MASK(curNpc->flag8, 0x40000))
                enemies[enemyCount++] = curNpc;
        }
        // Give floor key on 1st run, maybe chest key on 2nd
        for (i = 0; i < 2; ++i)
        {
            n = 0;
        buh:
            n += 1;
            if (n > 50) // Failsafe
                return 2;
            s32 random = system::rand() % enemyCount;
            if (npcCheckDanFlag(enemies[random], DAN_NPC_HOLOGRAPHIC) == true) // Block holographic enemies
                goto buh;
            if (i == 0) // Distribute main floor key
                enemies[random]->dropItemId = item_data::ITEM_ID_KEY_DAN_KEY;
            else // Distribute chest key
            {
                if (enemies[random]->dropItemId == item_data::ITEM_ID_KEY_DAN_KEY)
                    goto buh;
                assign = false;
                for (j = 0; j < 8; j += 1)
                {
                    if ((u8)(currentFloor % 10) == Lunatic->RFC.chestKeysToSpawn[j])
                        assign = true;
                }
                if (assign) // Assign guaranteed chest key if this floor is queued to have one
                    enemies[random]->dropItemId = item_data::ITEM_ID_KEY_MAC_KEY_00;
                if (assign || Lunatic->Luna.disorder == DISORDER_BLUE)
                    return 2;
                // Calculate enemiesInCycle
                for (j = 0; j < 10; j += 1)
                {
                    phase = (currentFloor / 10) % 10;
                    currentFloor = (phase * 10) + j;
                    for (k = 0; k < Lunatic->Floor[currentFloor].enemyTypes; k += 1)
                        enemiesInCycle += Lunatic->Floor[currentFloor].Enemies[k].num;
                }
                // wii::os::OSReport("Enemies in current cycle: %d\n", enemiesInCycle);
                // If a floor does not have a guaranteed chest key, small chance for any enemy to be assigned one
                for (j = 0; j < enemyCount; j += 1)
                {
                    s32 odds = system::rand() % enemiesInCycle;
                    if (odds == 0 && !randomKeyAssigned) // Spawn chance = (1 / # enemies in this cycle) repeated enemyCount times
                    {
                        enemies[random]->dropItemId = item_data::ITEM_ID_KEY_MAC_KEY_00;
                        randomKeyAssigned = true;
                    }
                }
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_dan_distribute_keys, 0)

    EVT_BEGIN(homogenize_lock_interact)
    USER_FUNC(evt_mario::evt_mario_key_off, 0)
    IF_EQUAL(GSWF(1620), 1)
    USER_FUNC(evt_pouch::evt_pouch_check_have_item, 48, LW(0))
    IF_NOT_EQUAL(LW(0), 0)
    USER_FUNC(evt_sub::evt_sub_item_select_menu, 0, PTR(&dan::dan_flipsideLockItems), LW(0), 0)
    IF_NOT_EQUAL(LW(0), 48)
    USER_FUNC(evt_mobj::evt_mobj_exec_cancel, PTR("me"))
    END_IF()
    END_IF()
    END_IF()
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
    EVT_END()

    EVT_BEGIN(dan_enemy_room_init_evt_new)
    SET(GSW(1623), 4) // Set rest floor NPC to "undetermined" for crash report purposes
    SET(LW(0), GSW(1))
    IF_EQUAL(LW(0), 0)
    USER_FUNC(evt_dan_init_lunatic)
    END_IF()
    USER_FUNC(set_mover_rng)
    USER_FUNC(get_mover_rng, LW(1)) // Movers
    IF_SMALL_EQUAL(LW(1), 14)
    USER_FUNC(evt_npc::evt_npc_entry, PTR("mover"), PTR("n_stg2_syuuzin_b"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("mover"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(_moverAnims))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("mover"), 0, true)
    USER_FUNC(evt_npc::evt_npc_add_flip_part, PTR("mover"))
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("mover"), -40, 0, 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("mover"), 9, PTR(fwd_mover_speech))
    ELSE()
    USER_FUNC(evt_dan_try_disorder)
    INLINE_EVT()
    USER_FUNC(DisorderDraw)
    END_INLINE()
    END_IF()
    USER_FUNC(dan::evt_dan_read_data)
    USER_FUNC(dan::evt_dan_handle_map_parts, LW(0))
    USER_FUNC(dan::evt_dan_handle_dokans, LW(0))
    USER_FUNC(evt_door::evt_door_set_dokan_descs, PTR(&dan::dan_dokanDescs), 8)
    USER_FUNC(DisorderGetId, LW(1))
    USER_FUNC(DisorderGetPreId, LW(2))
    IF_EQUAL(LW(1), 6)
    USER_FUNC(DepravityAction)
    ELSE()
    IF_EQUAL(LW(2), 6)
    USER_FUNC(DepravityAction)
    END_IF()
    END_IF()
    SET(LW(1), 0)
    USER_FUNC(dan::evt_dan_handle_doors, LW(0), LW(1), LW(10), LW(11), LW(2), LW(3), LW(4))
    USER_FUNC(evt_door::evt_door_set_map_door_descs, PTR(&dan::dan_mapDoorDescs), 2)
    USER_FUNC(evt_door::evt_door_enable_disable_map_door_desc, 0, LW(10))
    USER_FUNC(evt_door::evt_door_enable_disable_map_door_desc, 0, LW(11))
    USER_FUNC(evt_mobj::evt_mobj_zyo, PTR("lock_00"), 48, LW(2), LW(3), LW(4), 0, PTR(homogenize_lock_interact), PTR(dan::dan_lock_open_evt), 0) // Only one lock type
    USER_FUNC(dan::evt_dan_make_spawn_table, LW(0))
    SET(LW(10), 0)
    SET(LW(9), 0)
    DO(16)
    USER_FUNC(dan::evt_dan_get_enemy_info, LW(0), LW(10), LW(11), LW(12))
    IF_LARGE(LW(12), 0)
    DO(LW(12))
    USER_FUNC(dan::evt_dan_get_enemy_spawn_pos, LW(9), LW(0), LW(10), LW(13), LW(14), LW(15))
    ADD(LW(9), 1)
    USER_FUNC(evt_npc::evt_npc_entry_from_template, 0, LW(11), LW(13), LW(14), LW(15), LW(5), EVT_NULLPTR)
    USER_FUNC(evt_dan_modify_enemy, LW(5), LW(6))
    IF_EQUAL(LW(6), 1)
    USER_FUNC(evt_npc::evt_npc_set_animpose_disp_callback, LW(5), PTR(mi4::mi4MimiHolographicEffect), 0)
    END_IF()
    WHILE()
    END_IF()
    ADD(LW(10), 1)
    WHILE()
    USER_FUNC(evt_dan_distribute_keys)
    /*IF_EQUAL(GSW(1601), 1) // Tatarian Aster
    USER_FUNC(rand100, LW(8))
    IF_SMALL(LW(8), 70)
    USER_FUNC(dan::evt_dan_decide_key_enemy, 48)
    END_IF()
    ELSE()
    USER_FUNC(dan::evt_dan_decide_key_enemy, 48)
    END_IF()*/
    USER_FUNC(evt_npc::evt_npc_freeze_all)
    USER_FUNC(evt_hit::evt_hitobj_attr_onoff, 1, 1, PTR("A2"), 1073741824)
    USER_FUNC(evt_hit::evt_hitobj_attr_onoff, 1, 1, PTR("A3"), 536870912)
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S"), 2)
    USER_FUNC(evt_map::evt_mapobj_flag4_onoff, 1, 1, PTR("S"), 16)
    RUN_CHILD_EVT(evt_door::door_init_evt)
    ADD(GSW(1), 1)
    RUN_CHILD_EVT(custom_pit_music)
    USER_FUNC(evt_snd::evt_snd_set_sfx_reverb_mode, 0)
    IF_SMALL(GSW(1602), 1)
    USER_FUNC(handleBlessingWearOff)
    END_IF()
    INLINE_EVT()
    USER_FUNC(evt_door::evt_door_wait_flag, 256)
    USER_FUNC(evt_sub::evt_sub_display_room_name, 1, 6)
    END_INLINE()
    USER_FUNC(dan::evt_dan_start_countdown)
    INLINE_EVT()
    USER_FUNC(evt_door::evt_door_wait_flag, 256)
    USER_FUNC(evt_dan_disorder_set_or_clear)
    IF_EQUAL(GSW(1620), 2)
    IF_EQUAL(GSWF(1603), 0)
    SET(GSWF(1603), 1)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(hardDifficultyFirst), 0, 0)
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_IF()
    END_IF()
    USER_FUNC(DisorderGetId, LW(5))
    IF_EQUAL(LW(5), 4)
    RUN_CHILD_EVT(dan_disorder_indifference)
    END_IF()
    USER_FUNC(evt_npc::evt_npc_unfreeze_all)
    USER_FUNC(EvtVoucherCallAction, VOUCHER_CAKE)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 255, 0, 1000)
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    USER_FUNC(dan::evt_dan_get_door_names, LW(2), LW(3))
    USER_FUNC(evt_map::evt_mapobj_color, 1, LW(2), 255, 255, 255, LW(0))
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 1, LW(2), 1)
    END_INLINE()
    INLINE_EVT()
    USER_FUNC(evt_dan_handle_key_failsafe_new) // Completely overhauls key despawn behavior
    END_INLINE()
    USER_FUNC(evt_sub::func_800d4de4, 1, 0)
    RETURN()
    EVT_END()

    // Patches Dimentio to have a dynamic movement zone rather than being hardcoded for one room.
    s32 dimen_determine_move_pos_new(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        mario::MarioWork *marioWork = mario::marioGetPtr();
        npcdrv::NPCEntry *npc = (npcdrv::NPCEntry *)entry->ownerNPC;
        double destYPos = 0;
        f32 marioZ = ((marioWork->position).z);
        f32 destXPos = 0;
        u32 dimenMoveRand = 0;
        wii::mtx::Vec3 min;
        wii::mtx::Vec3 max;
        hitdrv::hitGetMapEntryBbox(0, &min, &max);
        s32 i = 0;
        do
        {
            while (true)
            {
                do
                {
                    i = i + 1;
                    dimenMoveRand = system::irand(400);
                    destXPos = ((marioWork->position).x + (f32)dimenMoveRand - 200);
                    if (i > 50)
                    {
                        destXPos = npc->position.x;
                        goto outOfBounds;
                    }
                } while ((destXPos <= (min.x + 25)) || ((max.x - 25) <= destXPos));
            outOfBounds:
                u32 yMoveBehavior = system::irand(100);
                if (yMoveBehavior < 67)
                {
                    dimenMoveRand = system::irand(4);
                    destYPos = (10.0 * (f32)dimenMoveRand + 20.0);
                }
                else
                {
                    dimenMoveRand = system::irand(3);
                    destYPos = (32.0 * (f32)dimenMoveRand + 40.0);
                }
                if (npc->flippedTo3d != 0)
                    break;
                if ((100.0 < __builtin_abs((destXPos - (marioWork->position).x))) || (80.0 < destYPos))
                    goto setFloats;
            }
            destYPos = system::distABf(destXPos, marioZ, ((marioWork->position).x), marioZ);
        } while ((destYPos <= 120.0) && (destYPos <= 80.0));
    setFloats:
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)entry->pCurData;
        evtmgr_cmd::evtSetFloat(entry, args[0], destXPos);
        evtmgr_cmd::evtSetFloat(entry, args[1], destYPos);
        evtmgr_cmd::evtSetFloat(entry, args[2], marioZ);
        return 2;
    }

    void rewrite_main()
    {
        // Enemy room init evt complete rewrite
        evtpatch::hookEvtReplace(dan::dan_enemy_room_init_evt, 1, dan_enemy_room_init_evt_new);
        patch::hookFunction(npc_dimeen_l::npc_dimen_determine_move_pos, dimen_determine_move_pos_new);
    }

}