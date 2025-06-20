#include "mod.h"
#include "patch.h"

/*
    NOTE TO ALL FUTURE MODDERS:
    I do not advise forking this project, because the code is bad and I have some very outdated libraries for evtpatch.
    If you're looking to make your own mods, please consider forking L5050's Hard Mode: Rubies and Magic or her mod template instead.
*/
#include "lunatic/npcdata.h"
#include "lunatic/localize.h"

#include <common.h>
#include <util.h>
#include <cutscene_helpers.h>
#include <evtpatch.h>
#include <tplpatch.h>
#include <sndpatch.h>
#include <evt_cmd.h>
#include <spm/rel/aa1_01.h>
#include <spm/rel/mi4.h>
#include <spm/rel/relocatable_module.h>
#include <spm/temp_unk.h>
#include <spm/animdrv.h>
#include <spm/bgdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/npc_ninja.h>
#include <spm/eff_fire.h>
#include <spm/eff_small_star.h>
#include <spm/eff_spm_confetti.h>
#include <spm/eff_zunbaba.h>
#include <spm/eff_spm_recovery.h>
#include <spm/eff_spm_spindash.h>
#include <spm/eff_spm_hit.h>
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
#include <spm/npcdrv.h>
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

    bool youSuck = false;

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
        /* 0x9 */ DISORDER_BLACK,
    };

    // Patches Dimentio to have a dynamic movement zone rather than being hardcoded for one room.
    s32 dimen_determine_move_pos_new(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        mario::MarioWork *marioWork = mario::marioGetPtr();
        npcdrv::NPCEntry *npc = entry->ownerNPC;
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

    static double boobies = 0;

    const char *destMap;
    const char *loadMap;

    const char *models[] = {"e_heiho", "e_buross_h", "e_buross_b", "e_burosu_i",
                            "e_cheririn_a", "e_chorobon_g", "e_gabow", "e_jugemu_d", "e_jyama_b", "e_card_jyama_b", "e_k_kuribo", "e_k_shoote4",
                            "e_kamek_g", "e_kamek_r", "e_kamek_w", "e_kames", "e_karon_d", "e_kmond", "e_kuribo_h",
                            "e_mer", "e_nin_d", "e_nokoteki_d", "e_ntl_p", "e_sinemoh", "e_tesita_bt", "e_tesita_sb",
                            "e_togenokd", "e_togezb", "e_touginoko", "e_wanwan_g", "e_teresa_b", "terminator"};

    s32 patchEnemyList[] = {2, 5, 6, 8, 10, 16, 19, 24, 29, 38, 43, 44, 49, 50, 55, 56, 61, 62, 66, 67, 68, 75, 76, 86, 95, 98, 101, 111,
                            112, 124, 167, 171, 220, 221, 222, 225, 228, 330, 331, 332, 333, 446, 465, 466, 470, 471, 473, 474, 475, 476, 477, 478, 479, 480,
                            481, 495, 496, 504, 505, 506, 529, 530, 531, 532, 534, -1};

    s32 enemyConfigArray[2001] = {169};

    // Pit Flimm item pool to iterate from
    s32 rotenShopItemPool[] = {81, 82, 84, 89, 90, 95, 99, 106, 106, 106, 106, 118, 123, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
                               141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160, 161, 162, 163, 164, 166, 168, 169, 170,
                               171, 173, 174, 177, 178, 180, 181, 182, 183, 184, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 198, 199, 200,
                               201, 202, 203, 204, 205, 207, 208, 209, 211, 212, 214, 215};

    s32 rotenShopLowerClassItemPool[] = {65, 66, 67, 68, 69, 70, 73, 74, 75, 76, 77, 78, 79, 83, 86, 98, 104, 109, 113};

    s32 boodinShopItemPool[] = {
        // Custom Pit Rando enemies
        283, 284, 286, 289, 290, 293, 294, 297, 300, 304, 306, 309, 314, 315, 316, 318, 322, 324, 330, 333, 336, 342,
        344, 350, 351, 352, 353, 356, 359, 364, 381, 384, 388, 402, 427, 434, 438, 439, 535, 512,
        // Vanilla enemies
        283, 285, 287, 288, 291, 292, 296, 298, 299, 301, 302, 303, 305, 307, 308, 310, 311, 312, 313, 317, 319, 323,
        328, 329, 331, 332, 334, 335, 338, 341, 343, 345, 346, 347, 348, 349, 354, 355, 358, 360, 362, 363, 365, 366,
        372, 373, 374, 375, 377, 378, 379, 380, 382, 383, 385, 386, 387, 389, 392, 393, 394, 395, 396, 398, 399, 400,
        401, 403, 408, 409, 412, 414, 415, 420, 421, 423, 424, 426, 428, 429, 431, 432, 433, 436, 437, 440, 441, 442,
        444, 446, 447, 448, 528, 529, 530, 531};

    struct
    {
        s32 cardCount = 13;
        s32 cardArray[15] = {0x58, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    } boodinBalls;

    s32 finalCardArray[15] = {0x58, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    s32 finalCardSelectedIdx = 0;

    // Set the Pit Flimm inventory
    s32 newRotenShopItems[] = {
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        -1};

    s32 rfcCommon[] = {
        item_data::ItemType::ITEM_ID_COOK_HUNNY_KINOKO, 1,
        item_data::ItemType::ITEM_ID_USE_HONOO_SAKURETU, 1,
        item_data::ItemType::ITEM_ID_USE_STAR_MEDAL, 2,
        item_data::ItemType::ITEM_ID_USE_BIG_EGG, 1,
        item_data::ItemType::ITEM_ID_USE_KOURA_DE_PON, 2,
        item_data::ItemType::ITEM_ID_USE_KOORI_NO_IBUKI, 2,
        item_data::ItemType::ITEM_ID_USE_BARIA_FRAME, 2,
        item_data::ItemType::ITEM_ID_COOK_BOMB_EGG, 3,
        item_data::ItemType::ITEM_ID_USE_POW_BLOCK, 2,
        item_data::ItemType::ITEM_ID_USE_TOROPICO_MANGO, 1,
        item_data::ItemType::ITEM_ID_USE_MILD_CACAO, 1,
        item_data::ItemType::ITEM_ID_USE_ROW_PASTA, 1,
        item_data::ItemType::ITEM_ID_USE_INGREDIENTS_OF_SWEET, 1,
        item_data::ItemType::ITEM_ID_USE_TUKUSHINBO, 1,
        item_data::ItemType::ITEM_ID_USE_PRIMITIVENUT, 1,
        item_data::ItemType::ITEM_ID_USE_HERB, 1,
        item_data::ItemType::ITEM_ID_USE_SHINABITA_KINOKO, 1,
        item_data::ItemType::ITEM_ID_COOK_FAIL_COOKING1, 1,
        item_data::ItemType::ITEM_ID_COOK_FAIL_COOKING2, 1,
        item_data::ItemType::ITEM_ID_COOK_HOT_COCOA, 2,
        item_data::ItemType::ITEM_ID_COOK_KAME_TEA, 3,
        item_data::ItemType::ITEM_ID_COOK_HERB_TEA, 3};

    s32 rfcUncommon[] = {
        item_data::ItemType::ITEM_ID_COOK_KINOKO_FRY, 1,
        item_data::ItemType::ITEM_ID_COOK_HUNNY_KINOKO_S, 2,
        item_data::ItemType::ITEM_ID_USE_NANIGA_OKORUKANA, 2,
        item_data::ItemType::ITEM_ID_USE_KAMINARI_DOKKAN, 1,
        item_data::ItemType::ITEM_ID_USE_TUYOTUYO_DRINK, 1,
        item_data::ItemType::ITEM_ID_COOK_HOTDOG, 2,
        item_data::ItemType::ITEM_ID_COOK_HANAJIRU_SYRUP, 2,
        item_data::ItemType::ITEM_ID_COOK_MOUSSE_CAKE, 2,
        item_data::ItemType::ITEM_ID_COOK_CHOCOLA_CAKE, 2,
        item_data::ItemType::ITEM_ID_USE_KINKYU_KINOKO, 2,
        item_data::ItemType::ITEM_ID_COOK_PEACH_TART, 1,
        item_data::ItemType::ITEM_ID_COOK_MIX_SHAKE, 2,
        item_data::ItemType::ITEM_ID_USE_POWERFUL_MEET, 2,
        item_data::ItemType::ITEM_ID_USE_SHINABITA_KINOKO, 1};

    s32 rfcRare[] = {
        item_data::ItemType::ITEM_ID_COOK_KINOKO_HOILE_FRY, 1,
        item_data::ItemType::ITEM_ID_USE_KIRAKIRA_OTOSHI, 1,
        item_data::ItemType::ITEM_ID_COOK_DINNER, 2,
        item_data::ItemType::ITEM_ID_COOK_HANA_DANGO, 1,
        item_data::ItemType::ITEM_ID_COOK_MANGO_PUDDING, 1,
        item_data::ItemType::ITEM_ID_COOK_GOLD_CHOKO, 2,
        item_data::ItemType::ITEM_ID_USE_GOLD_MEDAL, 1,
        item_data::ItemType::ITEM_ID_COOK_ICHIGO_RABBIT, 1,
        item_data::ItemType::ITEM_ID_COOK_SNOW_RABBIT, 1,
        item_data::ItemType::ITEM_ID_COOK_LOVE_NOODLE, 2,
        item_data::ItemType::ITEM_ID_USE_ULTRA_DRINK, 2,
        item_data::ItemType::ITEM_ID_COOK_EMERGENCY_MEAL, 2,
        item_data::ItemType::ITEM_ID_COOK_FRUITS_HUMBURG, 1,
        item_data::ItemType::ITEM_ID_USE_KINKYU_KINOKO, 2,
        item_data::ItemType::ITEM_ID_USE_SHINABITA_KINOKO, 1,
        item_data::ItemType::ITEM_ID_COOK_TRIAL_PAN, 3};

    s32 rfcItems[] = {0, 0, 0, -1};

    s32 whackaItems[] = {106, -1};

    s32 musicItems[] = {0x41, 0x42, 0x43, 0x45, 0x44, -1};

    s32 featuresItems[] = {0x41, 0x42, 0x43, -1};

    s32 patchesItems[] = {0x41, 0x42, -1};

    s32 accessibilityItems[] = {0x41, -1};

    s32 customSelectType = 0;

    enum CustomSelects
    {
        /* 0x0 */ SELECT_DEFAULT,
        /* 0x1 */ SELECT_MUSIC,
        /* 0x2 */ SELECT_FEATURES,
        /* 0x3 */ SELECT_PATCHES,
        /* 0x4 */ SELECT_ACCESSIBILITY
    };

    s32 no = 0;
    s32 i = 0;
    s32 currentFloor = 0;
    s32 nextFloor = 0;
    s32 enemyArrayOffset = 0;
    s32 nextEnemyCount = 0;
    s32 nextName1 = 0;
    s32 nextNum1 = 0;
    s32 nextPos1 = 0;
    s32 nextName2 = 0;
    s32 nextNum2 = 0;
    s32 nextPos2 = 0;
    s32 nextName3 = 0;
    s32 nextNum3 = 0;
    s32 nextPos3 = 0;

    s32 marioMaxHp = 0;
    s32 marioAtk = 0;
    s32 bowserAtk = 0;

    s32 moverRNG = 0;
    s32 houraiStoredAtk = 0;
    s32 apathyStoredHp = 0;
    bool hpMaxed = false;
    bool roomOnHud = false;
    bool houraiActivation = 0;
    s32 blessingRoomCounter = 0;
    s32 blessingNum = 0;
    s32 curseNum = 0;
    s32 disorderNum = 0;
    s32 disorderRooms = 0;
    s32 rand100Num = 0;
    s32 blessStorage = 0;
    s32 curseStorage = 0;
    s32 moverDown2Price = 0;
    s32 moverDown5Price = 0;
    s32 difficulty = 0;
    s32 motId = 0;

    s16 frameR = 255;
    s16 frameG = 0;
    s16 frameB = 0;
    s16 frameColPhase = 0;

    npcdrv::NPCTribeAnimDef luigiAnims[] = {
        {0, "luigi_S_1"},
        {1, "luigi_W_1"},
        {2, "luigi_R_2"},
        {3, "luigi_T_1"},
        {4, "luigi_D_7"},
        {6, "luigi_D_7"},
        {7, "luigi_D_7"},
        {8, "luigi_D_7"},
        {9, "luigi_K_1"},
        {10, "luigi_Y_1"},
        {11, "luigi_Y_1"},
        {12, "luigi_K_1"},
        {13, "luigi_K_1"},
        {14, "luigi_E_2"},
        {15, "luigi_Y_1"},
        {25, "luigi_J_1B"},
        {26, "luigi_T_3A"},
        {27, "luigi_J_1C"},
        {28, "luigi_I_1"},
        {29, "luigi_S_3"},
        {30, "luigi_D_4"},
        {-1, "luigi_Z_1"}};

    npcdrv::NPCPartDef sbParts[2] = {npcdrv::npcTribes[38].partsList[0], npcdrv::npcTribes[38].partsList[1]};

    npcdrv::NPCPartDef gcParts[7] = {npcdrv::npcTribes[124].partsList[0], npcdrv::npcTribes[124].partsList[1], npcdrv::npcTribes[124].partsList[2],
                                     npcdrv::npcTribes[124].partsList[3], npcdrv::npcTribes[124].partsList[4], npcdrv::npcTribes[124].partsList[5], npcdrv::npcTribes[124].partsList[6]};

    npcdrv::NPCPartDef bbParts[10] = {npcdrv::npcTribes[171].partsList[0], npcdrv::npcTribes[171].partsList[1], npcdrv::npcTribes[171].partsList[2],
                                      npcdrv::npcTribes[171].partsList[3], npcdrv::npcTribes[171].partsList[4], npcdrv::npcTribes[171].partsList[5], npcdrv::npcTribes[171].partsList[6],
                                      npcdrv::npcTribes[171].partsList[7], npcdrv::npcTribes[171].partsList[8], npcdrv::npcTribes[171].partsList[9]};

    // Shy Guy anim defs
    npcdrv::NPCTribeAnimDef heihoAnims[] = { // Copied from Goombas, then R_2 thrown in lol
        {0, "heiho_S_1"},                    // Idle
        {1, "heiho_W_1"},                    // Walking
        {2, "heiho_R_2"},                    // Running (Replace with R_1 for normal running)
        {3, "heiho_T_1"},                    // Talking
        {4, "heiho_D_1"},                    // Damaged
        {6, "heiho_D_1"},                    // Damaged
        {7, "heiho_D_1"},                    // Damaged
        {8, "heiho_D_1"},                    // Damaged
        {9, "heiho_K_1"},                    // Dizzy
        {10, "heiho_Z_1"},                   // Idle
        {11, "heiho_Z_1"},                   // Idle
        {12, "heiho_K_1"},                   // Dizzy
        {13, "heiho_K_1"},                   // Dizzy
        {14, "heiho_N_1"},                   // Eepy
        {15, "heiho_Z_1"},                   // Idle
        {26, "heiho_R_2"},                   // Ludicrous speed!!! (Use only for non-enemy stuff)
        {-1, "heiho_Z_1"}};

    EVT_BEGIN(heiho_g_panic_real)
    DO(0)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(4), LW(5), LW(6))
    USER_FUNC(evt_npc::evt_npc_get_axis_movement_unit, PTR("me"), LW(7))
    USER_FUNC(evt_npc::evt_npc_get_unitwork, PTR("me"), 13, LW(13))
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(14), 0, LW(15))
    USER_FUNC(evt_mario::evt_mario_check_3d, LW(10))
    IF_EQUAL(LW(10), 0) // Check if in 3D
                        // Check if Shy Guy is facing Mario and is within 200 units on X (Z only if in 3D)
    IF_EQUAL(LW(13), 0) // Not already in Panicked state
    SUB(LW(14), LW(4))
    MUL(LW(14), LW(7))
    IF_SMALL(LW(14), 0) // If NPC is facing Mario & under 200 units away. Trust me, this works
    IF_LARGE(LW(14), -100)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(1), LW(2), LW(3))
    SUB(LW(5), LW(2))
    IF_EQUAL(LW(5), 0) // Same Y val. Execute code now
    USER_FUNC(evt_npc::evt_npc_set_unitwork, PTR("me"), 8, 1)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(14), LW(2), LW(15))
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(14), LW(15), 0, 0, 532, 0, 0)
    MUL(LW(1), -1)
    MUL(LW(7), -1)
    WAIT_MSEC(450)
    //        USER_FUNC(evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), LW(7))
    USER_FUNC(evt_npc::evt_npc_set_unitwork, PTR("me"), 13, 1)
    WAIT_MSEC(350)
    MUL(LW(7), 8192)
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(7), LW(3), 0, 127, 532, 0, 0)
    USER_FUNC(evt_npc::evt_npc_set_unitwork, PTR("me"), 8, 0)
    DO_BREAK()
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    WAIT_FRM(5)
    WHILE()
    RETURN()
    EVT_END()

    EVT_BEGIN(heiho_g_panic)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(12))
    USER_FUNC(evt_npc::evt_npc_get_unitwork, PTR("me"), 13, LW(13))
    IF_EQUAL(LW(13), 0)
    IF_EQUAL(LW(12), 530)
    RUN_EVT(heiho_g_panic_real)
    END_IF()
    END_IF()
    IF_EQUAL(LW(11), 0)
    IF_EQUAL(LW(12), 530) // Is Green Shy Guy
    USER_FUNC(evt_npc::evt_npc_get_unitwork, PTR("me"), 13, LW(13))
    IF_EQUAL(LW(13), 1) // In Panicked state
    USER_FUNC(evt_npc::evt_npc_get_unitwork, PTR("me"), 8, LW(8))
    IF_EQUAL(LW(8), 0)
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 255, 532, 0, 0)
    END_IF()
    ELSE() // Not in Panicked state
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 40, 532, 0, 0)
    END_IF()
    ELSE() // Not Green Shy Guy
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 30, 532, 0, 0)
    END_IF()
    ELSE() // Not Green Shy Guy OR Goomba
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 30, 540, 0, 0)
    END_IF()
    USER_FUNC(evt_npc::evt_npc_set_unitwork, PTR("me"), 8, 0)
    RETURN_FROM_CALL()

    // Define enemy defenses
    npcdrv::NPCDefense createDef(int type, s32 defense, s32 flags)
    {
        npcdrv::NPCDefense def;
        def.type = type;
        def.defense = defense;
        def.flags = flags;
        return def;
    }
    npcdrv::NPCDefense def0_1 = createDef(0x0, 0x1, 0x0);
    npcdrv::NPCDefense def1_1 = createDef(0x1, 0x1, 0x0);
    npcdrv::NPCDefense def2_1 = createDef(0x2, 0x1, 0x0);
    npcdrv::NPCDefense def3_1 = createDef(0x3, 0x1, 0x0);
    npcdrv::NPCDefense def4_1 = createDef(0x4, 0x1, 0x0);
    npcdrv::NPCDefense def7_1 = createDef(0x7, 0x1, 0x0);
    npcdrv::NPCDefense def10_1 = createDef(0xa, 0x1, 0x0);
    npcdrv::NPCDefense def11_1 = createDef(0xb, 0x1, 0x0);
    npcdrv::NPCDefense def12_1 = createDef(0xc, 0x1, 0x0);
    npcdrv::NPCDefense def13_1 = createDef(0xd, 0x1, 0x0);
    npcdrv::NPCDefense def15_1 = createDef(0xf, 0x1, 0x0);
    npcdrv::NPCDefense def17_1 = createDef(0x11, 0x1, 0x0);
    npcdrv::NPCDefense def29_1 = createDef(0x1d, 0x1, 0x0);
    npcdrv::NPCDefense def31_1 = createDef(0x1f, 0x1, 0x0);

    // Initiators / Terminators (41 for sure, 48 unsure?)
    // Use def0 for enemies with no default defense
    npcdrv::NPCDefense def0 = createDef(0x0, 0x0, 0x0);
    npcdrv::NPCDefense def48 = createDef(0x30, 0x32, 0x0);
    npcdrv::NPCDefense defterm = createDef(41, 0x64, 0x0);
    static npcdrv::NPCDefense defaultDefenses[] = {def0, def48, defterm};

    // Add/reduce damage to certain enemies; later, maybe set DEFs for enemies that could actually use it and don't override all defenses with damage reduction.
    // There are certainly a few in this array that can use DEFs, DR is just easier for me right now.
    s32 (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
    void (*marioTakeDamage)(wii::mtx::Vec3 *position, u32 flags, s32 damage);
    s32 (*npcDamageMario)(npcdrv::NPCEntry *npcEntry, npcdrv::NPCPart *part, wii::mtx::Vec3 *position, u32 status, s32 damage, u32 flags);
    s32 (*npcHandleHitXp)(mario::MarioWork *marioWork, npcdrv::NPCEntry *npcEntry, s32 killXp, s32 unk_variant);
    s32 (*npcTakeDamage)(npcdrv::NPCEntry *npc, npcdrv::NPCPart *npcPart, s32 defenseType, s32 power, u32 flags, s32 param_6);
    void patchMarioDamage()
    {
        marioCalcDamageToEnemy = patch::hookFunction(mario::marioCalcDamageToEnemy,
                                                     [](s32 damageType, s32 tribeId)
                                                     {
                                                         // handle enemy damage reduction
                                                         s32 dmg = marioCalcDamageToEnemy(damageType, tribeId); // mod::marioCalcDamageToEnemyNew when combined with the spindash library
                                                         s32 paramitaTimer = swdrv::swByteGet(1610);
                                                         s32 floor = swdrv::swByteGet(1);
                                                         if (paramitaTimer > 20)
                                                         {
                                                             if (floor < 150)
                                                             {
                                                                 dmg = (dmg * 2);
                                                             }
                                                             else if (floor < 175)
                                                             {
                                                                 dmg = (dmg * 3);
                                                             }
                                                             else
                                                             {
                                                                 dmg = (dmg * 4);
                                                             }
                                                         }
                                                         s32 disorderId = swdrv::swByteGet(1630);
                                                         if (disorderId == DisorderId::DISORDER_RED)
                                                         {
                                                             difficulty = swdrv::swByteGet(1620);
                                                             switch (difficulty)
                                                             {
                                                             case 0:
                                                                 dmg = dmg + 1;
                                                                 break;
                                                             case 1:
                                                                 dmg = dmg + 2;
                                                                 break;
                                                             case 2:
                                                                 dmg = dmg + 3;
                                                                 break;
                                                             }
                                                         }
                                                         // Kilo Muth, DEF 3 -> 1
                                                         // Ice Bro, DEF 3 --> 1
                                                         // Red Spike Top, DEF 6 --> 4
                                                         // Sky-Blue Spiny, DEF 6 --> 4
                                                         if (tribeId == 506 || tribeId == 61 || tribeId == 29 || tribeId == 38)
                                                         {
                                                             dmg += 2;
                                                         }
                                                         // Spunia, DEF 0 --> 1
                                                         // Green Shy Guy, DEF 0 --> 1
                                                         else if (tribeId == 496 || tribeId == 530)
                                                         {
                                                             dmg -= 1;
                                                         }
                                                         // Shady Bros, DEF 3 --> 2
                                                         // Red Chomp & Skellobytes, DEF 4? --> 1 less
                                                         if (tribeId == 61 || tribeId == 55 || tribeId == 49 || tribeId == 225 || tribeId == 228 || tribeId == 123)
                                                         {
                                                             dmg += 1;
                                                         }
                                                         // Green (Broom) Magikoopa, DEF 0 --> 3
                                                         // (Flip) Shady Koopa, DEF 0 --> 3
                                                         else if (tribeId == 473 || tribeId == 474 || tribeId == 465 || tribeId == 466)
                                                         {
                                                             dmg -= 3;
                                                         }
                                                         // Blue Shy Guy, DEF 0 --> 2
                                                         else if (tribeId == 531)
                                                         {
                                                             dmg -= 2;
                                                         }
                                                         // Dark Koopatrol, DEF 8 --> 5
                                                         else if (tribeId == 19)
                                                         {
                                                             dmg += 3;
                                                         }
                                                         // Gold Chomp, DEF 10 --> 6
                                                         else if (tribeId == 124)
                                                         {
                                                             dmg += 4;
                                                         }
                                                         if (dmg < 0)
                                                         {
                                                             dmg = 0;
                                                         }
                                                         return dmg;
                                                     });

        marioTakeDamage = patch::hookFunction(mario::marioTakeDamage,
                                              [](wii::mtx::Vec3 *position, u32 flags, s32 damage)
                                              {
                                                  // Vulnerability Hex
                                                  s32 curse = swdrv::swByteGet(1601);
                                                  if (curse == CurseId::MERLUNA_HEX)
                                                  {
                                                      s32 floor = swdrv::swByteGet(1);
                                                      if (floor < 49)
                                                      {
                                                          damage = damage + 1;
                                                      }
                                                      else if (floor < 174)
                                                      {
                                                          damage = damage + 2;
                                                      }
                                                      else
                                                      {
                                                          damage = damage + 3;
                                                      }
                                                      if (damage < 0)
                                                      {
                                                          damage = 0;
                                                      }
                                                  }
                                                  // Migraine
                                                  if (curse == CurseId::MERLUNA_MIGRAINE)
                                                  {
                                                      s32 procMigraine = system::rand() % 100;
                                                      if (procMigraine < 40) // 40%
                                                      {
                                                          mario_status::marioStatusApplyStatuses(STATUS_FLIPPED_CONTROLS, 2);
                                                          swdrv::swSet(1670);
                                                      }
                                                      else if (procMigraine < 60) // 20%
                                                      {
                                                          mario_status::marioStatusApplyStatuses(STATUS_NO_SKILLS, 2);
                                                          swdrv::swSet(1670);
                                                      }
                                                      else if (procMigraine < 70) // 10%
                                                      {
                                                          mario_status::marioStatusApplyStatuses(STATUS_SLOW, 2);
                                                          swdrv::swSet(1670);
                                                      }
                                                      else if (procMigraine < 75) // 5%, otherwise no status.
                                                      {
                                                          mario_status::marioStatusApplyStatuses(STATUS_NO_JUMP, 2);
                                                          swdrv::swSet(1670);
                                                      }
                                                  }
                                                  // Disorder: Red/Apathy
                                                  s32 disorderId = swdrv::swByteGet(1630);
                                                  if (disorderId == DisorderId::DISORDER_RED)
                                                  {
                                                      difficulty = swdrv::swByteGet(1620);
                                                      switch (difficulty)
                                                      {
                                                      case 0:
                                                          damage = damage + 1;
                                                          break;
                                                      case 1:
                                                          damage = damage + 1;
                                                          break;
                                                      case 2:
                                                          damage = damage + 2;
                                                          break;
                                                      }
                                                  }
                                                  marioTakeDamage(position, flags, damage);
                                              });

        npcDamageMario = patch::hookFunction(npcdrv::npcDamageMario,
                                             [](npcdrv::NPCEntry *npcEntry, npcdrv::NPCPart *part, wii::mtx::Vec3 *position, u32 status, s32 damage, u32 flags)
                                             {
                                                 if (npcEntry == nullptr) // Patches a crash that occurs when npcDamageMario is called with no npcEntry pointer
                                                 {
                                                     if (part == nullptr && status == 0 && damage == 20 && flags == 4) // Dark Bowser's fire meets these conditions
                                                     {
                                                         difficulty = swdrv::swByteGet(1620);
                                                         switch (difficulty)
                                                         {
                                                         case 0:
                                                             damage = damage / 2;
                                                             break;
                                                         case 2:
                                                             damage = damage * 1.5;
                                                             break;
                                                         }
                                                     }
                                                     return npcDamageMario(npcEntry, part, position, status, damage, flags);
                                                 }
                                                 if (npcEntry->tribeId == 62) // Ice Bro projectiles will freeze you
                                                 {
                                                     status = status + 0x2000;
                                                 }
                                                 if (npcEntry->tribeId <= 333 && npcEntry->tribeId >= 330) // Patch Shadoo damage
                                                 {
                                                     difficulty = swdrv::swByteGet(1620);
                                                     switch (difficulty)
                                                     {
                                                     case 0:
                                                         damage = damage / 2;
                                                         break;
                                                     case 2:
                                                         damage = damage * 1.5;
                                                         break;
                                                     }
                                                 }
                                                 if (npcEntry->unkShellSfx == "holo") // Holographic enemies in the Pit will have 1.5x direct ATK
                                                 {
                                                     if (damage % 2 == 1) // Variable is odd, so round up.
                                                     {
                                                         damage = ((f32)damage * 1.5) + 0.5;
                                                     }
                                                     else
                                                     {
                                                         damage = damage * 1.5;
                                                     }
                                                 }
                                                 return npcDamageMario(npcEntry, part, position, status, damage, flags);
                                             });

        npcHandleHitXp = patch::hookFunction(temp_unk::npcHandleHitXp,
                                             [](mario::MarioWork *marioWork, npcdrv::NPCEntry *npcEntry, s32 killXp, s32 unk_variant)
                                             {
                                                 s32 disorderId = swdrv::swByteGet(1630);
                                                 if (disorderId == DisorderId::DISORDER_ORANGE) // If Dread is active, multiply XP
                                                 {
                                                    difficulty = swdrv::swByteGet(1620);
                                                    switch (difficulty)
                                                    {
                                                        case 0:
                                                        killXp = (msl::math::floor((f32)killXp * 1.5));
                                                        break;
                                                        case 1:
                                                        killXp = (msl::math::floor((f32)killXp * 1.25));
                                                        break;
                                                        case 2:
                                                        killXp = (msl::math::floor((f32)killXp * 1.1));
                                                        break;
                                                    }
                                                 }
                                                 if (disorderId == DisorderId::DISORDER_CYAN && killXp > 0) // If Recalcitrance is active, chance to multiply XP negatively and heal
                                                 {
                                                    difficulty = swdrv::swByteGet(1620);
                                                    s32 odds = system::rand() % 100;
                                                    u8 thresh = 0;
                                                    f32 xpMod = 0;
                                                    s32 hpMod = 0;
                                                    switch (difficulty)
                                                    {
                                                        case 0:
                                                        thresh = 20;
                                                        xpMod = -1.5;
                                                        hpMod = 2;
                                                        break;
                                                        case 1:
                                                        thresh = 35;
                                                        xpMod = -2;
                                                        hpMod = 2;
                                                        break;
                                                        case 2:
                                                        thresh = 50;
                                                        xpMod = -3;
                                                        hpMod = 1;
                                                        break;
                                                    }
                                                    if (thresh > odds)
                                                    {
                                                        killXp = (msl::math::floor((f32)killXp * xpMod));
                                                        mario::MarioWork *mario = mario::marioGetPtr();
                                                        f32 height = mario_hit::marioGetHeight();
                                                        eff_spm_recovery::effSpmRecoveryEntry(mario->position.x, ((mario->position.y) + height), mario->position.z, hpMod);
                                                        mario_pouch::pouchAddHp(hpMod);
                                                    }
                                                 }
                                                 if (npcEntry->unkShellSfx == "holo") // Holographic enemies in the Pit will give 2x score
                                                 {
                                                     killXp = killXp * 2;
                                                 }
                                                 return npcHandleHitXp(marioWork, npcEntry, killXp, unk_variant);
                                             });

        npcTakeDamage = patch::hookFunction(npcdrv::npcTakeDamage,
                                            [](npcdrv::NPCEntry *npc, npcdrv::NPCPart *npcPart, s32 defenseType, s32 power, u32 flags, s32 param_6)
                                            {
                                                difficulty = swdrv::swByteGet(1620);
                                                if (defenseType == 5 && difficulty == 2)
                                                {
                                                    if (npcPart->owner->unkShellSfx != 0 && msl::string::strcmp(npcPart->owner->unkShellSfx, "holo") != 0)
                                                    {
                                                        if (npcPart != 0)
                                                        {
                                                            spmario_snd::spsndSFXOn_3D("SFX_E_CRUSH1", &npcPart->position);
                                                            eff_spm_hit::effSpmHitEntry(npcPart->position.x, npcPart->position.y, npcPart->position.z, 0);
                                                        }
                                                        else if (npc != 0)
                                                        {
                                                            spmario_snd::spsndSFXOn_3D("SFX_E_CRUSH1", &npc->position);
                                                            eff_spm_hit::effSpmHitEntry(npc->position.x, npc->position.y, npc->position.z, 0);
                                                        }
                                                        return npcTakeDamage(npc, npcPart, defenseType, power, flags, param_6);
                                                    }
                                                    else if (npcPart->owner->tribeId == 76 || npcPart->owner->tribeId == 70 || npcPart->owner->tribeId == 72 || npcPart->owner->tribeId == 74) // Manually exclude Striker shells
                                                    {
                                                        if (npcPart != 0)
                                                        {
                                                            spmario_snd::spsndSFXOn_3D("SFX_E_CRUSH1", &npcPart->position);
                                                            eff_spm_hit::effSpmHitEntry(npcPart->position.x, npcPart->position.y, npcPart->position.z, 0);
                                                        }
                                                        else if (npc != 0)
                                                        {
                                                            spmario_snd::spsndSFXOn_3D("SFX_E_CRUSH1", &npc->position);
                                                            eff_spm_hit::effSpmHitEntry(npc->position.x, npc->position.y, npc->position.z, 0);
                                                        }
                                                        return npcTakeDamage(npc, npcPart, defenseType, power, flags, param_6);
                                                    }
                                                    else
                                                    {
                                                        power = 0;
                                                        npc = 0;
                                                        npcPart = 0;
                                                        flags = 0;
                                                        return npcTakeDamage(npc, npcPart, defenseType, power, flags, param_6);
                                                    }
                                                }
                                                else if (defenseType == 5 || defenseType == 29) // Hit by Koopa shell or Barried projectile
                                                // POTENTIAL TODO: Rework this *somehow* to make barried projectiles not play this stuff every frame...? It's vanilla behavior... but it sucks.
                                                {
                                                    if (npcPart != 0)
                                                    {
                                                        spmario_snd::spsndSFXOn_3D("SFX_E_CRUSH1", &npcPart->position);
                                                        eff_spm_hit::effSpmHitEntry(npcPart->position.x, npcPart->position.y, npcPart->position.z, 0);
                                                    }
                                                    else if (npc != 0)
                                                    {
                                                        spmario_snd::spsndSFXOn_3D("SFX_E_CRUSH1", &npc->position);
                                                        eff_spm_hit::effSpmHitEntry(npc->position.x, npc->position.y, npc->position.z, 0);
                                                    }
                                                }
                                                else if (defenseType == 6) // Thoreau collision
                                                {
                                                    if (npcPart != 0)
                                                    {
                                                        spmario_snd::spsndSFXOn_3D("SFX_E_CRUSH1", &npcPart->position);
                                                        eff_spm_hit::effSpmHitEntry(npcPart->position.x, npcPart->position.y, npcPart->position.z, 0);
                                                    }
                                                    if (npc != 0)
                                                    {
                                                        spmario_snd::spsndSFXOn_3D("SFX_E_CRUSH1", &npc->position);
                                                        eff_spm_hit::effSpmHitEntry(npc->position.x, npc->position.y, npc->position.z, 0);
                                                    }
                                                }
                                                return npcTakeDamage(npc, npcPart, defenseType, power, flags, param_6);
                                            });
    }

    // Called to remove default segments and their shadows from the map (Top 7 are the hitobjs, followed by mapobjs for visible segment and dropshadow)
    s32 danClearMapParts()
    {
        // Hitobjs for default segs
        hitdrv::hitGrpFlagOn(0, "pCube11_q00009", 1);
        hitdrv::hitGrpFlagOn(0, "pCube12_q00010", 1);
        hitdrv::hitGrpFlagOn(0, "pCube15_q00011", 1);
        hitdrv::hitGrpFlagOn(0, "pCube16_q00012", 1);
        hitdrv::hitGrpFlagOn(0, "pCube17_q00013", 1);
        hitdrv::hitGrpFlagOn(0, "pCube18_q00014", 1);
        hitdrv::hitGrpFlagOn(0, "pCube19_q00015", 1);

        // dan_01 mapobjs for default segs and shadows
        mapdrv::mapGrpFlagOn(0, "pCube11_q00028", 1);
        mapdrv::mapGrpFlagOn(0, "pCube11_q00029", 1);
        mapdrv::mapGrpFlagOn(0, "pCube12_q00030", 1);
        mapdrv::mapGrpFlagOn(0, "pCube12_q00031", 1);
        mapdrv::mapGrpFlagOn(0, "pCube15_q00032", 1);
        mapdrv::mapGrpFlagOn(0, "pCube15_q00033", 1);
        mapdrv::mapGrpFlagOn(0, "pCube16_q00034", 1);
        mapdrv::mapGrpFlagOn(0, "pCube16_q00035", 1);
        mapdrv::mapGrpFlagOn(0, "pCube17_q00036", 1);
        mapdrv::mapGrpFlagOn(0, "pCube17_q00037", 1);
        mapdrv::mapGrpFlagOn(0, "pCube18_q00038", 1);
        mapdrv::mapGrpFlagOn(0, "pCube18_q00039", 1);
        mapdrv::mapGrpFlagOn(0, "pCube19_q00040", 1);
        mapdrv::mapGrpFlagOn(0, "pCube19_q00041", 1);

        // dan_02 mapobjs for default segs and shadows (1 shared with dan_01)
        mapdrv::mapGrpFlagOn(0, "pCube11_q00036", 1);
        mapdrv::mapGrpFlagOn(0, "pCube11_q00037", 1);
        mapdrv::mapGrpFlagOn(0, "pCube12_q00036", 1);
        mapdrv::mapGrpFlagOn(0, "pCube12_q00037", 1);
        mapdrv::mapGrpFlagOn(0, "pCube15_q00036", 1);
        mapdrv::mapGrpFlagOn(0, "pCube15_q00037", 1);
        mapdrv::mapGrpFlagOn(0, "pCube16_q00038", 1);
        mapdrv::mapGrpFlagOn(0, "pCube16_q00039", 1);
        mapdrv::mapGrpFlagOn(0, "pCube17_q00039", 1);
        mapdrv::mapGrpFlagOn(0, "pCube17_q00040", 1);
        mapdrv::mapGrpFlagOn(0, "pCube18_q00040", 1);
        mapdrv::mapGrpFlagOn(0, "pCube18_q00041", 1);

        // dan_43 mapobjs (tails are offset by 1 from dan_01)
        mapdrv::mapGrpFlagOn(0, "pCube11_q00027", 1);
        mapdrv::mapGrpFlagOn(0, "pCube12_q00029", 1);
        mapdrv::mapGrpFlagOn(0, "pCube15_q00031", 1);
        mapdrv::mapGrpFlagOn(0, "pCube16_q00033", 1);
        mapdrv::mapGrpFlagOn(0, "pCube17_q00035", 1);
        mapdrv::mapGrpFlagOn(0, "pCube18_q00037", 1);
        mapdrv::mapGrpFlagOn(0, "pCube19_q00039", 1);
        return 2;
    }

    s32 generateMoverRng()
    {
        moverRNG = system::rand() % 1000;
        s32 floor = swdrv::swByteGet(1);
        if (floor >= 43 && floor <= 148)
        {
            moverRNG = 999;
        }
        else if (floor > 194)
        {
            moverRNG = 999;
        }
        s32 blockMovers = swdrv::swGet(1610);
        if (blockMovers == 1)
        {
            moverRNG = 999;
        }
        s32 hasDisorder = swdrv::swByteGet(1630);
        if (hasDisorder > 0)
        {
            moverRNG = 999;
        }
         moverRNG = 20; // DEBUG
        // THRESHOLD IS 14!!!!
        wii::os::OSReport("moverRNG: %d.\n", moverRNG);
        return 2;
    }

    // Gigantic shoutouts to L and Seeky for helping me get this function to work!!!!!! Loading custom TPLs into existing binaries is no small task!
    static void loadNewDanTex()
    {
        loadMap = seq_mapchange::seq_mapchange_wp->mapName;
        mapdrv::MapWorkGroup *mapWork = mapdrv::mapGetWork(); // Gets MapWorkGroup, which should contain the MapEntry for the current map
        mapdrv::MapEntry *mapEntry = &mapWork->entries[0];    // The MapEntry for current map
        s32 seq = seqdrv::seqGetSeq();
        if (msl::string::strstr(loadMap, "dan") != nullptr && msl::string::strstr(loadMap, "dan_70") == nullptr && seq != 4 && seq != 1)
        {
            generateMoverRng(); // Generates mover RNG in every dan map
            currentFloor = swdrv::swByteGet(1);
            bool overwriteMap = false;
            s32 danTexNum = -1;
            if (currentFloor >= 189 && msl::string::strstr(loadMap, "dan_44") != nullptr)
            {
                danTexNum = 1;
                overwriteMap = true;
            }
            else if (currentFloor >= 179 && msl::string::strstr(loadMap, "dan_44") != nullptr)
            {
                danTexNum = 0;
                overwriteMap = true;
            }
            else if (currentFloor >= 188 && currentFloor <= 190 && msl::string::strstr(loadMap, "dan_64") != nullptr)
            {
                danTexNum = 2;
                overwriteMap = true;
            }
            if (overwriteMap)
            {
                const char *danTexMapNames[] = {"dan_44_zaku", "dan_44_blank", "dan_64_zaku_insp"};
                const char *danTexMapName = danTexMapNames[danTexNum];
                const char *dvdRoot = system::getSpmarioDVDRoot();
                filemgr::FileEntry *bossRoomFile = filemgr::fileAllocf(0, "%s/map/%s.tpl", dvdRoot, danTexMapName);
                s32 tplSize = bossRoomFile->length;
                wii::tpl::TPLHeader *bossRoomTpl = (wii::tpl::TPLHeader *)memory::__memAlloc(memory::Heap::HEAP_MAP, tplSize);
                msl::string::memcpy(bossRoomTpl, bossRoomFile->sp->data, tplSize);
                filemgr::fileFree(bossRoomFile);
                wii::tpl::TPLBind(bossRoomTpl); // Initializes custom TPL in memory
                mapEntry->tpl = bossRoomTpl;    // Replaces normal tpl for map with the boss room TPL
            }
            else
            {
                wii::tpl::TPLBind(mapEntry->tpl);
            }
        }
        else
        {
            wii::tpl::TPLBind(mapEntry->tpl);
        }
    }

    static const char *getNextDanMapnameNew(s32 dungeonNo)
    {
        switch (dungeonNo)
        {
        // Rest Floors
        case 9:
            return "dan_21"; // Ch1
            break;
        case 19:
            return "dan_22"; // Ch2
            break;
        case 29:
            return "dan_23"; // Ch3
            break;
        case 39:
            return "dan_24"; // Ch4
            break;
        case 49:
        case 59:
        case 69:
        case 79:
        case 89:
        case 99:
        case 109:
        case 119:
        case 129:
        case 139:
        case 149:
            return "dan_61"; // Ch5, almost all of these are failsafes
        case 159:
            return "dan_62"; // Ch6
            break;
        case 169:
            return "dan_63"; // Ch7
            break;
        case 179:
            return "dan_64"; // Ch8
            break;
        case 189:
            return "dan_64"; // Also Ch8, texture replaced dynamically
            break;
        case 199:
            return "dan_70"; // Shadoo
            break;
        // Enemy rooms
        default:
            // Flipside
            if ((dungeonNo >= 0) && (dungeonNo <= 9))
                return "dan_01";
            else if ((dungeonNo >= 10) && (dungeonNo <= 19))
                return "dan_02";
            else if ((dungeonNo >= 20) && (dungeonNo <= 29))
                return "dan_03";
            else if ((dungeonNo >= 30) && (dungeonNo <= 39))
                return "dan_04";
            else if ((dungeonNo >= 40) && (dungeonNo <= 149))
                return "dan_41";
            else if ((dungeonNo >= 150) && (dungeonNo <= 159))
                return "dan_42";
            else if ((dungeonNo >= 160) && (dungeonNo <= 169))
                return "dan_43";
            else if ((dungeonNo >= 170) && (dungeonNo <= 199)) // 71-79 are White Pure Heart, 81-89 are Bleck/Tippi, 91-99 are Shadoo Blank
                return "dan_44";
        }
    }

    static void debugModeGayFrame()
    {
        framedrv::FrameEntry * frame = framedrv::framedrv_wp->entries;
        s32 cur = 0;
        for (cur = 0; cur < framedrv::framedrv_wp->num; cur = cur + 1)
        {
            //if (frame->type != 3) // FRAME_TYPE_EVT
           // {
                frame->color = {frameR, frameG, frameB, 255};
          //  }
            frame = frame + 1;
        }
        msl::string::strncmp(spmario::gp->mapName, "ls", 2);
    }

    static void patchBackCursya()
    {
        if (msl::string::strstr(spmario::gp->mapName, "dan") == 0)
        {
            seqdrv::seqSetSeq(seqdrv::SEQ_MAPCHANGE, "mac_02", "dokan_3");
            return 0;
        }
        else
        {
            seqdrv::seqSetSeq(seqdrv::SEQ_MAPCHANGE, "mac_05", "dokan_1");
            return 0;
        }
    }

    static void neutralize()
    {
        return;
    }

    static void handleHouraiDoll()
    {
        s32 blessing = swdrv::swByteGet(1600);
        houraiActivation = swdrv::swGet(1671);
        if (blessing == BlessId::MERLUNA_HOURAI && !houraiActivation)
        {
            mario_motion::marioChgMot(MOT_DAMAGE);
            mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
            mario::marioKeyOff();
            swdrv::swSet(1671);
            houraiStoredAtk = pouch->attack;
            s32 halvedAtk = (f32)(msl::math::floor((f32)(pouch->attack / 2)));
            if ((halvedAtk * 2) < pouch->attack) // Workaround since ceil just doesn't seem to exist in SPM
            {
                pouch->attack = (halvedAtk + 1); // i.e. if original ATK is 5, desired result is 3. Floors to 2. 4 < 5, so 2 + 1 = 3.
            }
            else
            {
                pouch->attack = halvedAtk; // i.e. if original ATK is 4, desired result is 2. Floors to 2. 4 !< 4, so remains 2.
            }
            if (pouch->maxHp != 999)
            {
                pouch->maxHp = (pouch->maxHp + 10);
            }
            else
            {
                hpMaxed = true;
            }
            pouch->hp = pouch->maxHp;
            swdrv::swSet(1672);
            return;
        }
        else
        {
            swdrv::swByteSet(1600, 0);
            swdrv::swByteSet(1601, 0);
            swdrv::swByteSet(1602, 0);
            mario_motion::marioChgMot(24); // MOT_DEATH
        }
    }

    static void disorderCoinAttrition()
    {
        hud::hud_wp->countdownTimer = hud::hud_wp->countdownTimer - 1;
        s32 disorderId = swdrv::swByteGet(1630);
        if (disorderId == DisorderId::DISORDER_YELLOW)
        {
            mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
            difficulty = swdrv::swByteGet(1620);
            u8 thresh = 0;
            u16 coinsLost = 0;
            f32 mult = 0;
            switch (difficulty)
            {
            case 0:
                thresh = 30;
                mult = system::rand() % 11 + 5;
                break;
            case 1:
                thresh = 50;
                mult = system::rand() % 21 + 10;
                break;
            case 2:
                thresh = 70;
                mult = system::rand() % 21 + 20;
                break;
            }
            mult = mult / 100;
            coinsLost = (u16)msl::math::floor(pouch->coins * mult);
            if (hud::hud_wp->countdownTimer == 299)
            {
                if (pouch->coins > 0 && coinsLost != 0)
                {
                    pouch->coins = pouch->coins - coinsLost;
                }
            }
            else if (hud::hud_wp->countdownTimer < 290)
            {
                s32 odds = system::rand() % 100;
                if (pouch->coins > 0 && odds < thresh)
                {
                    pouch->coins = pouch->coins - 1;
                }
            }
        }
        return;
    }

    s32 handleBlessingWearOff(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        houraiActivation = swdrv::swGet(1671);
        if (houraiActivation)
        {
            swdrv::swClear(1671);
            mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
            pouch->attack = houraiStoredAtk;
            s32 currentHp = pouch->hp;
            if (!hpMaxed)
            {
                pouch->maxHp = (pouch->maxHp - 10);
            }
            if (currentHp > pouch->maxHp)
            {
                pouch->hp = pouch->maxHp;
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(handleBlessingWearOff, 0)

    s32 evt_dan_read_data_new(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)entry;

        // Allocate work on first run
        // (check is a relD leftover, this only runs once on retail anyway)
        if (isFirstCall)
        {
            dan::dan_wp = (dan::DanWork *)memory::__memAlloc(memory::Heap::HEAP_MAP, sizeof(dan::DanWork));
            msl::string::memset(dan::dan_wp, 0, sizeof(*dan::dan_wp));
            dan::dan_wp->dungeons = (dan::DanDungeon *)memory::__memAlloc(memory::Heap::HEAP_MAP, sizeof(dan::DanDungeon[DUNGEON_MAX]));
            msl::string::memset(dan::dan_wp->dungeons, 0, sizeof(dan::DanDungeon[DUNGEON_MAX]));
        }

        // Reset Pit chests
        swdrv::swClear(433);
        swdrv::swClear(434);
        swdrv::swClear(435);
        swdrv::swClear(436);
        swdrv::swClear(437);
        swdrv::swClear(438);
        swdrv::swClear(439);
        swdrv::swClear(440);
        swdrv::swClear(441);
        swdrv::swClear(442);
        swdrv::swClear(443);
        swdrv::swClear(444);
        swdrv::swClear(445);
        swdrv::swClear(446);
        swdrv::swClear(447);
        swdrv::swClear(448);
        swdrv::swClear(449);
        swdrv::swClear(450);
        bool nipples = swdrv::swGet(409);

        // Clear Merluna judgement flag
        swdrv::swClear(1669);

        // Prepare pit text to be read
        /*    u32 size = wii::cx::CXGetCompressionHeader(lz_embedded::pitText).decompSize;
            char *decompPitText = (char *)memory::__memAlloc(0, size);
            wii::cx::CXUncompressLZ(lz_embedded::pitText, decompPitText);
            parse::parseInit(decompPitText, size); */
        i = 0;
        currentFloor = swdrv::swByteGet(1);
        s32 shadooEntries = swdrv::swByteGet(24);

        // Global room generation RNG value (1-100, 5 decimal places)
        double thighs = system::rand() % 99 + 1;
        double divisor = 10;
        double ass = (system::rand() % 10) / divisor;
        divisor = 100;
        double booty = (system::rand() % 10) / divisor;
        divisor = 1000;
        double butt = (system::rand() % 10) / divisor;
        divisor = 10000;
        double bottom = (system::rand() % 10) / divisor;
        divisor = 100000;
        double buttocks = (system::rand() % 10) / divisor;
        boobies = (thighs + ass + booty + butt + bottom + buttocks);

        // Check if room 48, then shove you into Flopside Pit
        if (currentFloor == 48)
        {
            swdrv::swByteSet(1, 148);
        }

        // Resets no if pit is started
        if (currentFloor == 0)
        {
            no = -1;
        }
        no = no + 1;

        // Ensure Shadoo is fought on first room entry, but never again
        if (shadooEntries == 0)
        {
            swdrv::swByteSet(24, 1);
        }

        // Start pit room randomization process
        // Establish default doors + other vars
        s32 activeDoorCount = 19; // Actually 20, but let's not complicate things
        s32 doorOn[31] = {2, 3, 6, 7, 9, 12, 13, 16, 18, 19, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32};
        s32 roomDecCode = 0;
        s32 segmentCount = 0;
        bool structureGenerated = true;
        bool segment400 = false;
        bool segment1 = false;
        bool segment800 = false;
        bool segment80 = false;
        bool segment40 = false;
        bool segment2 = false;
        bool segment2000 = false;
        bool segment1000 = false;
        bool segment8 = false;
        bool segment4 = false;
        bool segment4000 = false;
        bool segment200 = false;
        bool segment100 = false;
        bool segment10 = false;
        bool segment8000 = false;
        bool segment20 = false;
        bool segment10000 = false;
        bool segment20000 = false;
        bool segment40000 = false;
        bool segment80000 = false;

        if (currentFloor != 198)
        {
            // Determine # of segments to generate for current room
            s32 segmentRNG = system::rand() % 152;
            if (segmentRNG >= 0 && segmentRNG < 42)
            {
                segmentCount = 3;
            }
            else if (segmentRNG >= 42 && segmentRNG < 45)
            {
                segmentCount = 1;
            }
            else if (segmentRNG >= 45 && segmentRNG < 109)
            {
                segmentCount = 4;
            }
            else if (segmentRNG >= 109 && segmentRNG < 122)
            {
                segmentCount = 2;
            }
            else if (segmentRNG >= 122 && segmentRNG < 124)
            {
                segmentCount = 0;
            }
            else
            {
                segmentCount = 5;
            }
            s32 disorderId = swdrv::swByteGet(1630);
            if (disorderId == DisorderId::DISORDER_GREEN) // Guarantee a maximally complex layout while Indifference is active
            {
                segmentCount = 16;
            }
            wii::os::OSReport("Queueing %d segment generations for this room.\n", segmentCount);
            i = 0;
            // Enclosed structure generation. If more than 2 segments are generated at any time, the remainder is subtracted from segmentCount
            s32 structureRNG = system::rand() % 180;
            s32 structureMiscRNG = system::rand() % 100;
            if (disorderId == DisorderId::DISORDER_GREEN && structureRNG > 30) // Guarantee an enclosed structure while Indifference is active
            {
                structureRNG = system::rand() % 31;
            }
            // Split
            if (structureRNG >= 0 && structureRNG < 8)
            {
                wii::os::OSReport("!!! Split Structure is generating !!!\n");
                segment2000 = true;
                segment8 = true;
                if (structureMiscRNG < 70)
                {
                    segment20000 = true;
                }
                else if (structureMiscRNG < 92)
                {
                    segment80000 = true;
                }
                else
                {
                    segment1 = true;
                    segment20 = true;
                    segment400 = true;
                    segment8000 = true;
                    segment10000 = true;
                    segment20000 = true;
                    segment40000 = true;
                    segment80000 = true;
                    segmentCount = segmentCount - 4;
                }
            }
            // Quarter
            else if (structureRNG >= 8 && structureRNG < 19)
            {
                wii::os::OSReport("!!! Quarter Structure is generating !!!\n");
                if (structureMiscRNG < 31)
                {
                    segment100 = true;
                    segment8 = true;
                    if (segment40000)
                    {
                        if (boobies < 50)
                        {
                            segment80000 = true;
                        }
                    }
                    else
                    {
                        segment80000 = true;
                    }
                }
                else if (structureMiscRNG < 54)
                {
                    segment40 = true;
                    segment8 = true;
                    if (segment40000)
                    {
                        if (boobies < 50)
                        {
                            segment80000 = true;
                        }
                    }
                    else
                    {
                        segment80000 = true;
                    }
                }
                else if (structureMiscRNG < 77)
                {
                    segment100 = true;
                    segment2000 = true;
                    if (segment10000)
                    {
                        if (boobies < 50)
                        {
                            segment20000 = true;
                        }
                    }
                    else
                    {
                        segment20000 = true;
                    }
                }
                else
                {
                    segment40 = true;
                    segment2000 = true;
                    if (segment10000)
                    {
                        if (boobies < 50)
                        {
                            segment20000 = true;
                        }
                    }
                    else
                    {
                        segment20000 = true;
                    }
                }
            }
            // Cube
            else if (structureRNG >= 19 && structureRNG < 21)
            {
                wii::os::OSReport("!!! Cube Structure is generating !!!\n");
                if (structureMiscRNG < 50)
                {
                    segment400 = true;
                    segment80 = true;
                    segment1 = true;
                    segment20 = true;
                    segmentCount = segmentCount - 2;
                    segment20000 = true;
                    segment40000 = true;
                }
                else
                {
                    segment8000 = true;
                    segment200 = true;
                    segment20 = true;
                    segment1 = true;
                    segmentCount = segmentCount - 2;
                    segment20000 = true;
                    segment40000 = true;
                }
            }
            // Pillar
            else if (structureRNG >= 21 && structureRNG < 23)
            {
                wii::os::OSReport("!!! Pillar Structure is generating !!!\n");
                if (structureMiscRNG < 60)
                {
                    segment800 = true;
                    segment80 = true;
                    segment2 = true;
                }
                else
                {
                    segment4000 = true;
                    segment200 = true;
                    segment10 = true;
                }
                if (boobies < 50)
                {
                    segment20000 = true;
                }
                else
                {
                    segment80000 = true;
                }
            }
            // Small
            else if (structureRNG >= 23 && structureRNG < 26)
            {
                wii::os::OSReport("!!! Small Structure is generating !!!\n");
                if (structureMiscRNG < 33)
                {
                    segment800 = true;
                    segment400 = true;
                    segment10 = true;
                    segment20 = true;
                    segment8000 = true;
                    segment10000 = true;
                    segment80000 = true;
                    segmentCount = segmentCount - 3;
                }
                else if (structureMiscRNG < 67)
                {
                    segment8000 = true;
                    segment4000 = true;
                    segment1 = true;
                    segment2 = true;
                    segment400 = true;
                    segment10000 = true;
                    segment80000 = true;
                    segmentCount = segmentCount - 3;
                }
                else
                {
                    if (boobies < 25)
                    {
                        segment8000 = true;
                        segment4000 = true;
                        segment400 = true;
                        segment10000 = true;
                        segmentCount = segmentCount - 1;
                    }
                    else if (boobies < 50)
                    {
                        segment800 = true;
                        segment400 = true;
                        segment8000 = true;
                        segment10000 = true;
                        segmentCount = segmentCount - 1;
                    }
                    else if (boobies < 75)
                    {
                        segment10 = true;
                        segment20 = true;
                        segment80000 = true;
                    }
                    else
                    {
                        segment1 = true;
                        segment2 = true;
                        segment80000 = true;
                    }
                }
            }
            // I'm sorry Tartt but I am leaving the stream during Lobates. They bore the hell out of me and frankly I can think of a whole host of things I'd rather be doing, including playing with a rubber band. Please
            else if (structureRNG >= 26 && structureRNG < 32)
            {
                wii::os::OSReport("!!! Lobate Structure is generating !!!\n");
                if (structureMiscRNG <= 66)
                {
                    if (boobies < 25)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment200 = true;
                        segment8000 = true;
                        segment20000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 50)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment80 = true;
                        segment400 = true;
                        segment20000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 75)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment200 = true;
                        segment20 = true;
                        segment20000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment80 = true;
                        segment1 = true;
                        segment20000 = true;
                        segmentCount = segmentCount - 2;
                    }
                }
                else
                {
                    if (structureMiscRNG >= 80)
                    {
                        segment20000 = true;
                    }
                    else
                    {
                        segment80000 = true;
                    }
                    if (boobies < 12.5)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment80 = true;
                        segment800 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 25)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment200 = true;
                        segment4000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 37.5)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment80 = true;
                        segment2 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 50)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment200 = true;
                        segment10 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 62.5)
                    {
                        segment2 = true;
                        segment4 = true;
                        segment200 = true;
                        segment4000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 75)
                    {
                        segment10 = true;
                        segment200 = true;
                        segment800 = true;
                        segment1000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 87.5)
                    {
                        segment4 = true;
                        segment10 = true;
                        segment80 = true;
                        segment800 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else
                    {
                        segment2 = true;
                        segment80 = true;
                        segment1000 = true;
                        segment4000 = true;
                        segmentCount = segmentCount - 2;
                    }
                }
            }
            // Structure does not generate
            else
            {
                structureGenerated = false;
            }
            wii::os::OSReport("Structure check passed with RNG val %d, %d segments to generate remaining.\n", structureRNG, segmentCount);

            // Random segment generation + add'l doors
            while (segmentCount > 0 && i < 20)
            {
                s32 segmentID = system::rand() % 16;
                switch (segmentID)
                {
                case 0:
                    // Segment 400
                    if ((segment400) ||
                        (segment800) ||
                        (segment1000 && segment2000) ||
                        (segment1000 && segment4000) ||
                        (segment1000 && segment8000) ||
                        (segment10 && segment200 && segment1000) ||
                        (segment2 && segment80) ||
                        (segment1 && segment4 && segment200 && segment1000) ||
                        (segment1 && segment4 && segment8 && segment2000) ||
                        (segment1 && segment80) ||
                        (segment40 && segment80) ||
                        (segment4 && segment8 && segment80) ||
                        (segment4 && segment10 && segment80) ||
                        (segment4 && segment20 && segment80) ||
                        (segment10 && segment200 && segment1000) ||
                        (segment20 && segment200 && segment1000) ||
                        (segment80 && segment4 && segment200 && segment4000) ||
                        (segment80 && segment4 && segment200 && segment8000) ||
                        (segment80 && segment4 && segment200 && segment100) ||
                        (segment1000 && segment200 && segment4 && segment2) ||
                        (segment1000 && segment200 && segment4 && segment8) ||
                        (segment100 && segment200 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment400 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 1;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg400 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 1:
                    // Segment 1
                    if ((segment1) ||
                        (segment2) ||
                        (segment4 && segment8) ||
                        (segment4 && segment10) ||
                        (segment4 && segment20) ||
                        (segment4 && segment200 && segment4000) ||
                        (segment80 && segment800) ||
                        (segment4 && segment200 && segment400 && segment1000) ||
                        (segment8 && segment400 && segment1000 && segment2000) ||
                        (segment80 && segment400) ||
                        (segment40 && segment80) ||
                        (segment80 && segment1000 && segment2000) ||
                        (segment80 && segment1000 && segment4000) ||
                        (segment80 && segment1000 && segment8000) ||
                        (segment4 && segment200 && segment4000) ||
                        (segment4 && segment200 && segment8000) ||
                        (segment80 && segment1000 && segment200 && segment10) ||
                        (segment80 && segment1000 && segment200 && segment20) ||
                        (segment80 && segment1000 && segment200 && segment100) ||
                        (segment800 && segment1000 && segment200 && segment4) ||
                        (segment2000 && segment1000 && segment200 && segment4) ||
                        (segment4 && segment100 && segment200))
                    {
                        break;
                    }
                    else
                    {
                        segment1 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 17;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg1 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 2:
                    // Segment 40
                    if ((segment80 && segment800) ||
                        (segment40) ||
                        (segment2 && segment80) ||
                        (segment80 && segment400) ||
                        (segment1 && segment80) ||
                        (segment2000) ||
                        (segment8) ||
                        (segment100) ||
                        (segment4 && segment20 && segment80) ||
                        (segment4 && segment10 && segment80) ||
                        (segment80 && segment1000 && segment8000) ||
                        (segment80 && segment1000 && segment200 && segment10) ||
                        (segment80 && segment1000 && segment200 && segment20) ||
                        (segment80 && segment1000 && segment200 && segment100) ||
                        (segment80 && segment4 && segment200 && segment4000) ||
                        (segment80 && segment4 && segment200 && segment8000) ||
                        (segment80 && segment4 && segment200 && segment100) ||
                        (segment80 && segment1000 && segment4000))
                    {
                        break;
                    }
                    else
                    {
                        segment40 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 10;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 11;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg40 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 3:
                    // Segment 1000
                    if ((segment400 && segment8000) ||
                        (segment400 && segment4000) ||
                        (segment800 && segment8000) ||
                        (segment1000) ||
                        (segment800 && segment4000) ||
                        (segment800 && segment2000) ||
                        (segment2000 && segment4000) ||
                        (segment2000 && segment8000) ||
                        (segment400 && segment2000) ||
                        (segment2 && segment10 && segment80 && segment200) ||
                        (segment4 && segment80 && segment200) ||
                        (segment40 && segment80 && segment2000) ||
                        (segment4 && segment8 && segment80 && segment2000) ||
                        (segment4 && segment8 && segment200 && segment2000) ||
                        (segment2 && segment8 && segment80 && segment2000) ||
                        (segment8 && segment10 && segment200 && segment2000) ||
                        (segment2 && segment80 && segment8000) ||
                        (segment2 && segment80 && segment4000) ||
                        (segment2 && segment80 && segment2000) ||
                        (segment1 && segment80 && segment8000) ||
                        (segment1 && segment80 && segment4000) ||
                        (segment1 && segment80 && segment2000) ||
                        (segment40 && segment80 && segment8000) ||
                        (segment40 && segment80 && segment4000) ||
                        (segment40 && segment80 && segment2000) ||
                        (segment10 && segment200 && segment800) ||
                        (segment10 && segment200 && segment400) ||
                        (segment10 && segment200 && segment2000) ||
                        (segment20 && segment200 && segment800) ||
                        (segment20 && segment200 && segment400) ||
                        (segment20 && segment200 && segment2000) ||
                        (segment100 && segment200 && segment800) ||
                        (segment100 && segment200 && segment2000) ||
                        (segment1 && segment4 && segment200 && segment400) ||
                        (segment4 && segment20 && segment80 && segment8000) ||
                        (segment2 && segment80 && segment200 && segment20) ||
                        (segment2 && segment80 && segment200 && segment100) ||
                        (segment1 && segment80 && segment200 && segment10) ||
                        (segment1 && segment80 && segment200 && segment20) ||
                        (segment1 && segment80 && segment200 && segment100) ||
                        (segment40 && segment80 && segment200 && segment10) ||
                        (segment40 && segment80 && segment200 && segment20) ||
                        (segment40 && segment80 && segment200 && segment100) ||
                        (segment400 && segment200 && segment4 && segment2) ||
                        (segment400 && segment200 && segment4 && segment8) ||
                        (segment800 && segment200 && segment4 && segment1) ||
                        (segment800 && segment200 && segment4 && segment2) ||
                        (segment800 && segment200 && segment4 && segment8) ||
                        (segment2000 && segment200 && segment4 && segment1) ||
                        (segment2000 && segment200 && segment4 && segment2) ||
                        (segment2000 && segment200 && segment4 && segment8) ||
                        (segment8000 && segment80 && segment4 && segment10) ||
                        (segment8000 && segment80 && segment4 && segment8) ||
                        (segment4000 && segment80 && segment4 && segment20) ||
                        (segment4000 && segment80 && segment4 && segment10) ||
                        (segment4000 && segment80 && segment4 && segment8) ||
                        (segment2000 && segment80 && segment4 && segment20) ||
                        (segment2000 && segment80 && segment4 && segment10) ||
                        (segment2000 && segment80 && segment4 && segment8) ||
                        (segment100 && segment200 && segment400))
                    {
                        break;
                    }
                    else
                    {
                        segment1000 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 4;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 5;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg1000 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 4:
                    // Segment 4
                    if ((segment1 && segment20) ||
                        (segment4) ||
                        (segment2 && segment10) ||
                        (segment1 && segment10) ||
                        (segment2 && segment20) ||
                        (segment2 && segment8) ||
                        (segment8 && segment10) ||
                        (segment8 && segment20) ||
                        (segment1 && segment8) ||
                        (segment80 && segment200 && segment800 && segment4000) ||
                        (segment80 && segment200 && segment1000) ||
                        (segment8 && segment40 && segment80) ||
                        (segment8 && segment100 && segment200) ||
                        (segment8 && segment80 && segment1000 && segment2000) ||
                        (segment8 && segment200 && segment1000 && segment2000) ||
                        (segment8 && segment80 && segment800 && segment2000) ||
                        (segment8 && segment200 && segment2000 && segment4000) ||
                        (segment20 && segment80 && segment800) ||
                        (segment10 && segment80 && segment800) ||
                        (segment8 && segment80 && segment800) ||
                        (segment20 && segment80 && segment400) ||
                        (segment10 && segment80 && segment400) ||
                        (segment8 && segment80 && segment400) ||
                        (segment20 && segment40 && segment80) ||
                        (segment10 && segment40 && segment80) ||
                        (segment1 && segment200 && segment4000) ||
                        (segment2 && segment200 && segment4000) ||
                        (segment8 && segment200 && segment4000) ||
                        (segment1 && segment200 && segment8000) ||
                        (segment2 && segment200 && segment8000) ||
                        (segment8 && segment200 && segment8000) ||
                        (segment1 && segment100 && segment200) ||
                        (segment1 && segment200 && segment400 && segment1000) ||
                        (segment20 && segment80 && segment1000 && segment8000) ||
                        (segment800 && segment80 && segment200 && segment8000) ||
                        (segment800 && segment80 && segment200 && segment100) ||
                        (segment400 && segment80 && segment200 && segment4000) ||
                        (segment400 && segment80 && segment200 && segment8000) ||
                        (segment400 && segment80 && segment200 && segment100) ||
                        (segment40 && segment80 && segment200 && segment4000) ||
                        (segment40 && segment80 && segment200 && segment8000) ||
                        (segment40 && segment80 && segment200 && segment100) ||
                        (segment400 && segment1000 && segment200 && segment2) ||
                        (segment400 && segment1000 && segment200 && segment8) ||
                        (segment800 && segment1000 && segment200 && segment1) ||
                        (segment800 && segment1000 && segment200 && segment2) ||
                        (segment800 && segment1000 && segment200 && segment8) ||
                        (segment2000 && segment1000 && segment200 && segment1) ||
                        (segment2000 && segment1000 && segment200 && segment2) ||
                        (segment2000 && segment1000 && segment200 && segment8) ||
                        (segment8000 && segment1000 && segment80 && segment10) ||
                        (segment8000 && segment1000 && segment80 && segment8) ||
                        (segment4000 && segment1000 && segment80 && segment20) ||
                        (segment4000 && segment1000 && segment80 && segment10) ||
                        (segment4000 && segment1000 && segment80 && segment8) ||
                        (segment2000 && segment1000 && segment80 && segment20) ||
                        (segment2000 && segment1000 && segment80 && segment10) ||
                        (segment2000 && segment1000 && segment80 && segment8) ||
                        (segment2 && segment100 && segment200))
                    {
                        break;
                    }
                    else
                    {
                        segment4 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 20;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 21;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg4 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 5:
                    // Segment 100
                    if ((segment200 && segment4000) ||
                        (segment100) ||
                        (segment10 && segment200) ||
                        (segment100 && segment200) ||
                        (segment20 && segment200) ||
                        (segment200 && segment8000) ||
                        (segment2000) ||
                        (segment8) ||
                        (segment40) ||
                        (segment1 && segment4 && segment200) ||
                        (segment2 && segment4 && segment200) ||
                        (segment200 && segment400 && segment1000) ||
                        (segment2 && segment80 && segment1000 && segment200) ||
                        (segment1 && segment80 && segment1000 && segment200) ||
                        (segment40 && segment80 && segment1000 && segment200) ||
                        (segment800 && segment80 && segment4 && segment200) ||
                        (segment400 && segment80 && segment4 && segment200) ||
                        (segment40 && segment80 && segment4 && segment200) ||
                        (segment200 && segment800 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment100 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 14;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 15;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg100 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 6:
                    // Segment 8000
                    if ((segment4000) ||
                        (segment8000) ||
                        (segment1000 && segment2000) ||
                        (segment1000 && segment800) ||
                        (segment1000 && segment400) ||
                        (segment2 && segment80 && segment1000) ||
                        (segment10 && segment200) ||
                        (segment4 && segment20 && segment80 && segment1000) ||
                        (segment4 && segment8 && segment20 && segment2000) ||
                        (segment20 && segment200) ||
                        (segment100 && segment200) ||
                        (segment4 && segment8 && segment200) ||
                        (segment2 && segment4 && segment200) ||
                        (segment1 && segment4 && segment200) ||
                        (segment2 && segment80 && segment1000) ||
                        (segment1 && segment80 && segment1000) ||
                        (segment800 && segment80 && segment4 && segment200) ||
                        (segment400 && segment80 && segment4 && segment200) ||
                        (segment40 && segment80 && segment4 && segment200) ||
                        (segment1000 && segment80 && segment4 && segment10) ||
                        (segment1000 && segment80 && segment4 && segment8) ||
                        (segment40 && segment80 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment8000 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 8;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg8000 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 7:
                    // Segment 20
                    if ((segment10) ||
                        (segment20) ||
                        (segment4 && segment8) ||
                        (segment2 && segment4) ||
                        (segment1 && segment4) ||
                        (segment4 && segment80 && segment800) ||
                        (segment200 && segment4000) ||
                        (segment4 && segment80 && segment1000 && segment8000) ||
                        (segment8 && segment1000 && segment2000 && segment8000) ||
                        (segment200 && segment8000) ||
                        (segment100 && segment200) ||
                        (segment200 && segment1000 && segment2000) ||
                        (segment200 && segment800 && segment1000) ||
                        (segment200 && segment400 && segment1000) ||
                        (segment4 && segment80 && segment800) ||
                        (segment4 && segment80 && segment400) ||
                        (segment2 && segment80 && segment1000 && segment200) ||
                        (segment1 && segment80 && segment1000 && segment200) ||
                        (segment40 && segment80 && segment1000 && segment200) ||
                        (segment4000 && segment1000 && segment80 && segment4) ||
                        (segment2000 && segment1000 && segment80 && segment4) ||
                        (segment4 && segment40 && segment80))
                    {
                        break;
                    }
                    else
                    {
                        segment20 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 24;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg20 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 8:
                    // Segment 800
                    if ((segment400) ||
                        (segment800) ||
                        (segment80 && segment2) ||
                        (segment80 && segment40) ||
                        (segment80 && segment1) ||
                        (segment1000 && segment2000) ||
                        (segment1000 && segment4000) ||
                        (segment1000 && segment8000) ||
                        (segment4 && segment8 && segment80) ||
                        (segment4 && segment10 && segment80) ||
                        (segment4 && segment20 && segment80) ||
                        (segment4 && segment80 && segment200 && segment4000) ||
                        (segment10 && segment200 && segment1000) ||
                        (segment20 && segment200 && segment1000) ||
                        (segment80 && segment4 && segment200 && segment8000) ||
                        (segment80 && segment4 && segment200 && segment100) ||
                        (segment1000 && segment200 && segment4 && segment1) ||
                        (segment1000 && segment200 && segment4 && segment2) ||
                        (segment1000 && segment200 && segment4 && segment8) ||
                        (segment100 && segment200 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment800 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg800 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 9:
                    // Segment 80
                    if ((segment2 && segment800) ||
                        (segment80) ||
                        (segment1 && segment40) ||
                        (segment40 && segment400) ||
                        (segment2 && segment400) ||
                        (segment1 && segment800) ||
                        (segment1 && segment400) ||
                        (segment40 && segment1000 && segment2000) ||
                        (segment40 && segment800) ||
                        (segment4 && segment8 && segment40) ||
                        (segment2 && segment40) ||
                        (segment4 && segment200 && segment1000) ||
                        (segment4 && segment20 && segment1000 && segment8000) ||
                        (segment2 && segment1000 && segment8000) ||
                        (segment4 && segment200 && segment800 && segment4000) ||
                        (segment2 && segment10 && segment200 && segment1000) ||
                        (segment2 && segment1000 && segment8000) ||
                        (segment2 && segment1000 && segment4000) ||
                        (segment2 && segment1000 && segment2000) ||
                        (segment1 && segment1000 && segment8000) ||
                        (segment1 && segment1000 && segment4000) ||
                        (segment1 && segment1000 && segment2000) ||
                        (segment40 && segment1000 && segment8000) ||
                        (segment40 && segment1000 && segment4000) ||
                        (segment4 && segment20 && segment800) ||
                        (segment4 && segment20 && segment400) ||
                        (segment4 && segment20 && segment40) ||
                        (segment4 && segment10 && segment800) ||
                        (segment4 && segment10 && segment400) ||
                        (segment4 && segment10 && segment40) ||
                        (segment4 && segment8 && segment800) ||
                        (segment2 && segment1000 && segment200 && segment20) ||
                        (segment2 && segment1000 && segment200 && segment100) ||
                        (segment1 && segment1000 && segment200 && segment10) ||
                        (segment1 && segment1000 && segment200 && segment20) ||
                        (segment1 && segment1000 && segment200 && segment100) ||
                        (segment40 && segment1000 && segment200 && segment10) ||
                        (segment40 && segment1000 && segment200 && segment20) ||
                        (segment40 && segment1000 && segment200 && segment100) ||
                        (segment800 && segment4 && segment200 && segment8000) ||
                        (segment800 && segment4 && segment200 && segment100) ||
                        (segment400 && segment4 && segment200 && segment4000) ||
                        (segment400 && segment4 && segment200 && segment8000) ||
                        (segment400 && segment4 && segment200 && segment100) ||
                        (segment40 && segment4 && segment200 && segment4000) ||
                        (segment40 && segment4 && segment200 && segment8000) ||
                        (segment40 && segment4 && segment200 && segment100) ||
                        (segment8000 && segment1000 && segment4 && segment10) ||
                        (segment8000 && segment1000 && segment4 && segment8) ||
                        (segment4000 && segment1000 && segment4 && segment20) ||
                        (segment4000 && segment1000 && segment4 && segment10) ||
                        (segment4000 && segment1000 && segment4 && segment8) ||
                        (segment2000 && segment1000 && segment4 && segment20) ||
                        (segment2000 && segment1000 && segment4 && segment10) ||
                        (segment2000 && segment1000 && segment4 && segment8) ||
                        (segment4 && segment8 && segment400))
                    {
                        break;
                    }
                    else
                    {
                        segment80 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg80 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 10:
                    // Segment 2
                    if ((segment1) ||
                        (segment2) ||
                        (segment80 && segment800) ||
                        (segment40 && segment80) ||
                        (segment80 && segment400) ||
                        (segment4 && segment8) ||
                        (segment4 && segment10) ||
                        (segment4 && segment20) ||
                        (segment80 && segment1000 && segment2000) ||
                        (segment80 && segment1000 && segment4000) ||
                        (segment80 && segment1000 && segment8000) ||
                        (segment4 && segment200 && segment4000) ||
                        (segment10 && segment80 && segment200 && segment1000) ||
                        (segment4 && segment200 && segment8000) ||
                        (segment80 && segment1000 && segment200 && segment20) ||
                        (segment80 && segment1000 && segment200 && segment100) ||
                        (segment400 && segment1000 && segment200 && segment4) ||
                        (segment800 && segment1000 && segment200 && segment4) ||
                        (segment2000 && segment1000 && segment200 && segment4) ||
                        (segment4 && segment100 && segment200))
                    {
                        break;
                    }
                    else
                    {
                        segment2 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg2 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 11:
                    // Segment 2000
                    if ((segment8) ||
                        (segment2000) ||
                        (segment800 && segment1000) ||
                        (segment1000 && segment4000) ||
                        (segment400 && segment1000) ||
                        (segment1000 && segment8000) ||
                        (segment40) ||
                        (segment100) ||
                        (segment2 && segment80 && segment1000) ||
                        (segment1 && segment80 && segment1000) ||
                        (segment10 && segment200 && segment1000) ||
                        (segment1000 && segment200 && segment4 && segment1) ||
                        (segment1000 && segment200 && segment4 && segment2) ||
                        (segment1000 && segment200 && segment4 && segment8) ||
                        (segment1000 && segment80 && segment4 && segment20) ||
                        (segment1000 && segment80 && segment4 && segment10) ||
                        (segment1000 && segment80 && segment4 && segment8) ||
                        (segment20 && segment200 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment2000 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg2000 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 12:
                    // Segment 8
                    if ((segment2000) ||
                        (segment8) ||
                        (segment2 && segment4) ||
                        (segment4 && segment10) ||
                        (segment1 && segment4) ||
                        (segment4 && segment20) ||
                        (segment40) ||
                        (segment100) ||
                        (segment4 && segment80 && segment800) ||
                        (segment4 && segment80 && segment400) ||
                        (segment4 && segment200 && segment4000) ||
                        (segment400 && segment1000 && segment200 && segment4) ||
                        (segment800 && segment1000 && segment200 && segment4) ||
                        (segment2000 && segment1000 && segment200 && segment4) ||
                        (segment8000 && segment1000 && segment80 && segment4) ||
                        (segment4000 && segment1000 && segment80 && segment4) ||
                        (segment2000 && segment1000 && segment80 && segment4) ||
                        (segment4 && segment200 && segment8000))
                    {
                        break;
                    }
                    else
                    {
                        segment8 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg8 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 13:
                    // Segment 4000
                    if ((segment8000) ||
                        (segment4000) ||
                        (segment10 && segment200) ||
                        (segment100 && segment200) ||
                        (segment20 && segment200) ||
                        (segment1000 && segment2000) ||
                        (segment1000 && segment800) ||
                        (segment1000 && segment400) ||
                        (segment4 && segment8 && segment200) ||
                        (segment2 && segment4 && segment200) ||
                        (segment1 && segment4 && segment200) ||
                        (segment4 && segment80 && segment200 && segment800) ||
                        (segment2 && segment80 && segment1000) ||
                        (segment1 && segment80 && segment1000) ||
                        (segment400 && segment80 && segment4 && segment200) ||
                        (segment40 && segment80 && segment4 && segment200) ||
                        (segment1000 && segment80 && segment4 && segment20) ||
                        (segment1000 && segment80 && segment4 && segment10) ||
                        (segment1000 && segment80 && segment4 && segment8) ||
                        (segment40 && segment80 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment4000 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg4000 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 14:
                    // Segment 200
                    if ((segment10 && segment4000) ||
                        (segment200) ||
                        (segment20 && segment100) ||
                        (segment100 && segment8000) ||
                        (segment10 && segment8000) ||
                        (segment20 && segment4000) ||
                        (segment20 && segment8000) ||
                        (segment100 && segment1000 && segment2000) ||
                        (segment100 && segment200 && segment2000) ||
                        (segment4 && segment8 && segment100) ||
                        (segment10 && segment100) ||
                        (segment100 && segment4000) ||
                        (segment4 && segment80 && segment1000) ||
                        (segment1 && segment4 && segment400 && segment1000) ||
                        (segment10 && segment400 && segment1000) ||
                        (segment4 && segment80 && segment800 && segment4000) ||
                        (segment2 && segment10 && segment80 && segment1000) ||
                        (segment10 && segment400 && segment1000) ||
                        (segment10 && segment800 && segment1000) ||
                        (segment10 && segment1000 && segment2000) ||
                        (segment20 && segment400 && segment1000) ||
                        (segment20 && segment800 && segment1000) ||
                        (segment20 && segment1000 && segment2000) ||
                        (segment100 && segment400 && segment1000) ||
                        (segment100 && segment800 && segment1000) ||
                        (segment1 && segment4 && segment4000) ||
                        (segment1 && segment4 && segment8000) ||
                        (segment1 && segment4 && segment100) ||
                        (segment2 && segment4 && segment4000) ||
                        (segment2 && segment4 && segment8000) ||
                        (segment2 && segment4 && segment100) ||
                        (segment4 && segment8 && segment4000) ||
                        (segment2 && segment80 && segment1000 && segment20) ||
                        (segment2 && segment80 && segment1000 && segment100) ||
                        (segment1 && segment80 && segment1000 && segment10) ||
                        (segment1 && segment80 && segment1000 && segment20) ||
                        (segment1 && segment80 && segment1000 && segment100) ||
                        (segment40 && segment80 && segment1000 && segment10) ||
                        (segment40 && segment80 && segment1000 && segment20) ||
                        (segment40 && segment80 && segment1000 && segment100) ||
                        (segment800 && segment80 && segment4 && segment8000) ||
                        (segment800 && segment80 && segment4 && segment100) ||
                        (segment400 && segment80 && segment4 && segment4000) ||
                        (segment400 && segment80 && segment4 && segment8000) ||
                        (segment400 && segment80 && segment4 && segment100) ||
                        (segment40 && segment80 && segment4 && segment4000) ||
                        (segment40 && segment80 && segment4 && segment8000) ||
                        (segment40 && segment80 && segment4 && segment100) ||
                        (segment400 && segment1000 && segment4 && segment2) ||
                        (segment400 && segment1000 && segment4 && segment8) ||
                        (segment800 && segment1000 && segment4 && segment1) ||
                        (segment800 && segment1000 && segment4 && segment2) ||
                        (segment800 && segment1000 && segment4 && segment8) ||
                        (segment2000 && segment1000 && segment4 && segment1) ||
                        (segment2000 && segment1000 && segment4 && segment2) ||
                        (segment2000 && segment1000 && segment4 && segment8) ||
                        (segment4 && segment8 && segment8000))
                    {
                        break;
                    }
                    else
                    {
                        segment200 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg200 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 15:
                    // Segment 10
                    if ((segment20) ||
                        (segment10) ||
                        (segment200 && segment4000) ||
                        (segment100 && segment200) ||
                        (segment200 && segment8000) ||
                        (segment4 && segment8) ||
                        (segment2 && segment4) ||
                        (segment1 && segment4) ||
                        (segment200 && segment1000 && segment2000) ||
                        (segment200 && segment800 && segment1000) ||
                        (segment200 && segment400 && segment1000) ||
                        (segment2 && segment80 && segment200 && segment1000) ||
                        (segment4 && segment80 && segment800) ||
                        (segment4 && segment80 && segment400) ||
                        (segment1 && segment80 && segment1000 && segment200) ||
                        (segment40 && segment80 && segment1000 && segment200) ||
                        (segment8000 && segment1000 && segment80 && segment4) ||
                        (segment4000 && segment1000 && segment80 && segment4) ||
                        (segment2000 && segment1000 && segment80 && segment4) ||
                        (segment4 && segment40 && segment80))
                    {
                        break;
                    }
                    else
                    {
                        segment10 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg10 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;
                }
                ++i;
            }

            // Room layout cleanup: choose from possible doors and add pipes
            // Pipes
            if (!structureGenerated)
            {
                s32 pipeRNG = system::rand() % 133;
                if (pipeRNG >= 0 && pipeRNG < 39)
                {
                    segment20000 = true;
                }
                else if (pipeRNG >= 39 && pipeRNG < 76)
                {
                    segment80000 = true;
                }
                else if (pipeRNG >= 76 && pipeRNG < 80 && segment1 && segment20)
                {
                    segment40000 = true;
                }
                else if (pipeRNG >= 80 && pipeRNG < 82 && segment400 && segment8000)
                {
                    segment10000 = true;
                }
                else if (pipeRNG >= 82 && pipeRNG < 83)
                {
                    segment20000 = true;
                    segment80000 = true;
                }
                else if (pipeRNG >= 83 && pipeRNG < 86 && segment1 && segment20 && segment400 && segment8000)
                {
                    segment10000 = true;
                    segment20000 = true;
                    segment40000 = true;
                    segment80000 = true;
                }
            }

            // Add all segment values
            if (segment400)
            {
                roomDecCode = roomDecCode + 1024;
            }
            if (segment800)
            {
                roomDecCode = roomDecCode + 2048;
            }
            if (segment1000)
            {
                roomDecCode = roomDecCode + 4096;
            }
            if (segment2000)
            {
                roomDecCode = roomDecCode + 8192;
            }
            if (segment4000)
            {
                roomDecCode = roomDecCode + 16384;
            }
            if (segment8000)
            {
                roomDecCode = roomDecCode + 32768;
            }
            if (segment40)
            {
                roomDecCode = roomDecCode + 64;
            }
            if (segment80)
            {
                roomDecCode = roomDecCode + 128;
            }
            if (segment100)
            {
                roomDecCode = roomDecCode + 256;
            }
            if (segment200)
            {
                roomDecCode = roomDecCode + 512;
            }
            if (segment1)
            {
                roomDecCode = roomDecCode + 1;
            }
            if (segment2)
            {
                roomDecCode = roomDecCode + 2;
            }
            if (segment4)
            {
                roomDecCode = roomDecCode + 4;
            }
            if (segment8)
            {
                roomDecCode = roomDecCode + 8;
            }
            if (segment10)
            {
                roomDecCode = roomDecCode + 16;
            }
            if (segment20)
            {
                roomDecCode = roomDecCode + 32;
            }

            wii::os::OSReport("Finished generating. Roomcode w/o pipes is %d.\n", roomDecCode);

            if (segment10000)
            {
                roomDecCode = roomDecCode + 65536;
            }
            if (segment20000)
            {
                roomDecCode = roomDecCode + 131072;
            }
            if (segment40000)
            {
                roomDecCode = roomDecCode + 262144;
            }
            if (segment80000)
            {
                roomDecCode = roomDecCode + 524288;
            }

            if (currentFloor == 198)
            {
                roomDecCode = 0;
                activeDoorCount = 19;
            }
        }

        // Doors
        s32 successfulDoors = 0;
        s32 doorExit = 0;
        s32 doorEntrance = 0;
        s32 doorId = 0;
        bool currentDoorSuccess = false;
        bool doorOnToggleableSegment = false;
        while (successfulDoors != 2)
        {
            currentDoorSuccess = false;
            doorOnToggleableSegment = false;
            doorId = system::rand() % 32 + 1;
            while (doorEntrance == doorId)
            {
                doorId = system::rand() % 32 + 1;
            }
            wii::os::OSReport("Generated doorId %d.\n", doorId);
            if (doorId == 1)
            {
                doorOnToggleableSegment = true;
                if (segment400)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 4 || doorId == 5)
            {
                doorOnToggleableSegment = true;
                if (segment1000)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 8)
            {
                doorOnToggleableSegment = true;
                if (segment8000)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 10 || doorId == 11)
            {
                doorOnToggleableSegment = true;
                if (segment40)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 14 || doorId == 15)
            {
                doorOnToggleableSegment = true;
                if (segment100)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 17)
            {
                doorOnToggleableSegment = true;
                if (segment1)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 20 || doorId == 21)
            {
                doorOnToggleableSegment = true;
                if (segment4)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 24)
            {
                doorOnToggleableSegment = true;
                if (segment20)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            wii::os::OSReport("All toggleable segment checks finished; doorOnToggleableSegment is %d, successfulDoors is %d, currentDoorSuccess is %d.\n", doorOnToggleableSegment, successfulDoors, currentDoorSuccess);
            if (!doorOnToggleableSegment)
            {
                if (doorId >= 25)
                {
                    if (boobies > 35)
                    {
                        successfulDoors = successfulDoors + 1;
                        currentDoorSuccess = true;
                        wii::os::OSReport("doorId was not on a toggleable segment; successfulDoors is %d.\n", successfulDoors);
                    }
                }
                else
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId was not on a toggleable segment; successfulDoors is %d.\n", successfulDoors);
                }
            }
            if (currentDoorSuccess)
            {
                if (successfulDoors == 1)
                {
                    doorEntrance = doorId;
                }
                else if (successfulDoors == 2)
                {
                    doorExit = doorId;
                }
            }
            wii::os::OSReport("doorEntrance: %d. doorExit: %d.\n", doorEntrance, doorExit);
        }

        // NEW Super Item Substitution Bros. Wii
        i = 0;
        currentFloor = swdrv::swByteGet(1);
        s32 itemRarity = 0; // Common by default. 1 is Uncommon, 2 is Rare
        s32 n = 0;
        f32 scaling = 0;
        s32 itemArraySize = 0;
        s32 itemSubrarity = 0;
        s32 itemId = 0;
        rfcItems[0] = 0;
        rfcItems[1] = 0;
        rfcItems[2] = 0;
        s32 floorFloor = currentFloor;
        bool itemAssigned = false;
        if (floorFloor > 100)
        {
            floorFloor = floorFloor - 100;
        }
        // Get 3 items.
        for (i = 0; i < 3; ++i)
        {
            itemRarity = 0;
            n = 0;
            // Determine item rarity. One success makes it Uncommon, two successes makes it Rare, none makes it Common.
            do
            {
                n = n + 1;
                rand100Num = system::rand() % 100;
                if (rand100Num < (s32)msl::math::floor(floorFloor / 1.2))
                {
                    itemRarity = itemRarity + 1;
                }
            } while (n < 2);

            // Determine the item via subrarity. If subrarity check fails, loop again until it doesn't.
            do
            {
                itemAssigned = false;
                switch (itemRarity)
                {
                case 0:
                    itemArraySize = (sizeof(rfcCommon) / 8);         // Divide by 4 to get array item length, further divide by 2 to get item ID count
                    itemId = ((system::rand() % itemArraySize) * 2); // Even numbered rand will always be an item ID
                    itemSubrarity = rfcCommon[itemId + 1];           // Subrarity for an item is indexed 1 position after its resp. item ID every time
                    itemId = rfcCommon[itemId];                      // Reassign itemId to true item ID
                                                                     //            wii::os::OSReport("Item %d is Common. itemId is %d.\n", i, itemId);
                    break;
                case 1:
                    itemArraySize = (sizeof(rfcUncommon) / 8);
                    itemId = ((system::rand() % itemArraySize) * 2);
                    itemSubrarity = rfcUncommon[itemId + 1];
                    itemId = rfcUncommon[itemId];
                    //            wii::os::OSReport("Item %d is Uncommon. itemId is %d.\n", i, itemId);
                    break;
                default:
                    itemArraySize = (sizeof(rfcRare) / 8);
                    itemId = ((system::rand() % itemArraySize) * 2);
                    itemSubrarity = rfcRare[itemId + 1];
                    itemId = rfcRare[itemId];
                    //            wii::os::OSReport("Item %d is Rare. itemId is %d.\n", i, itemId);
                    break;
                }
                rand100Num = system::rand() % 4;
                if (itemId != rfcItems[0] && itemId != rfcItems[1] && itemId != rfcItems[2] && itemSubrarity > rand100Num)
                {
                    rfcItems[i] = itemId; // Assigns the item to the rfcItems array
                    wii::os::OSReport("Item %d was assigned to rfcItems. itemId is %d.\n", i, itemId);
                    itemAssigned = true;
                }
            } while (!itemAssigned); // Loops until the rand is below the subrarity value AND item isn't identical to previously assigned items
        }

        i = 0;
        for (i = 0; i < 200; ++i)
        {
            // Placeholder item, true items are determined by above protocol and chosen upon opening chest
            dan::dan_wp->dungeons[i].item = itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");

            // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
            dan::dan_wp->dungeons[i].map = roomDecCode;

            // Door substitution
            dan::dan_wp->dungeons[i].doors[0].enter = doorEntrance;
            dan::dan_wp->dungeons[i].doors[0].exit = doorExit;
            dan::dan_wp->dungeons[i].doorCount = 1;
        }

        s32 e = 0;
        if (currentFloor == 0)
        {
            s32 enemyArrayVal = 1;
            s32 enemyName = 0;
            for (s32 roomGens = 0; roomGens <= 199; roomGens = roomGens + 1)
            {
                // Enemy generation setup
                e = 0;
                s32 enemyTypes = 0;
                s32 arrayRNG = 0;
                s32 enemyAmt = 0;
                s32 enemyDifference = 0;
                s32 enemyMin = 0;
                s32 enemyMax = 0;
                //  s32 spDoorRNG = 0;
                s32 tribeArray = 0;
                s32 vsOdds = 0;
                s32 eGenTR[3] = {-1, -1, -1}; // "Enemies Generated This Room"
                s32 enemyTribe = 0;
                s32 enemyBonus = 0;
                s32 limiterMod = 0;
                s32 enemyGenLim1 = 0;
                s32 enemyGenLim2 = 0;
                s32 enemyGenRNG = system::rand() % 100;
                difficulty = swdrv::swByteGet(1620);
                switch (difficulty)
                {
                case 0:
                    enemyGenLim1 = 30;
                    enemyGenLim2 = 85;
                    break;
                case 1:
                    enemyGenLim1 = 20;
                    enemyGenLim2 = 75;
                    break;
                case 2:
                    enemyGenLim1 = 25;
                    enemyGenLim2 = 50;
                    break;
                }
                if (enemyGenRNG < enemyGenLim1)
                {
                    enemyTypes = 1;
                }
                else if (enemyGenRNG < enemyGenLim2)
                {
                    enemyTypes = 2;
                }
                else
                {
                    enemyTypes = 3;
                }
                enemyConfigArray[enemyArrayVal] = enemyTypes;
                enemyArrayVal = enemyArrayVal + 1;
                mod::DanLevelData *danLevelData = nullptr;
                // Enemy Substition Protocol v3: Generate enemy data for each floor on Pit entry.
                // v3 makes great use of a new file called "npcdata.h", which contains Pit Rando-specific data on enemies and this generation process.
                while (e < enemyTypes)
                {
                    // Get "level data," referencing the scaling nature of the Pit. These provide thresholds for enemy Lv & bonus enemies to spawn, independent of Difficulty Options.
                    if (roomGens < 25)
                    {
                        danLevelData = mod::danLevelDataPtr(1);
                    }
                    else if (roomGens < 150)
                    {
                        danLevelData = mod::danLevelDataPtr(2);
                    }
                    else if (roomGens < 175)
                    {
                        danLevelData = mod::danLevelDataPtr(3);
                    }
                    else
                    {
                        danLevelData = mod::danLevelDataPtr(4);
                    }
                    // Set enemy Lv threshold modifiers based on Difficulty.
                    difficulty = swdrv::swByteGet(1620);
                    limiterMod = system::rand() % 11;
                    if (difficulty == 0)
                    {
                        limiterMod = limiterMod - 5; // Ranges from -5 to 5
                    }
                    else if (difficulty == 1)
                    {
                        limiterMod = limiterMod - 10; // Ranges from -10 to +0
                    }
                    else
                    {
                        limiterMod = limiterMod - 25; // Ranges from -15 to -25
                    }
                    // Roll through each limiter. Lower enemyGenRNG values tend toward lower Lv enemies.
                    enemyGenRNG = system::rand() % 100;
                    if (enemyGenRNG < ((danLevelData->lv1Limiter) + limiterMod))
                    {
                        arrayRNG = system::rand() % (sizeof(mod::lv1Tribes) / 4);
                        tribeArray = 1;
                        enemyTribe = mod::lv1Tribes[arrayRNG];
                        wii::os::OSReport("Room #%d: tribeArray %d selected; (enemyGenRNG = %d) < (lv1Lim + limMod = %d). limMod = %d, lim = %d.\n", roomGens, tribeArray, enemyGenRNG, ((danLevelData->lv1Limiter) + limiterMod), limiterMod, danLevelData->lv1Limiter);
                    }
                    else if (enemyGenRNG < ((danLevelData->lv2Limiter) + limiterMod))
                    {
                        arrayRNG = system::rand() % (sizeof(mod::lv2Tribes) / 4);
                        tribeArray = 2;
                        enemyTribe = mod::lv2Tribes[arrayRNG];
                        wii::os::OSReport("Room #%d: tribeArray %d selected; (enemyGenRNG = %d) < (lv2Lim + limMod = %d). limMod = %d, lim = %d.\n", roomGens, tribeArray, enemyGenRNG, ((danLevelData->lv2Limiter) + limiterMod), limiterMod, danLevelData->lv2Limiter);
                    }
                    else if (enemyGenRNG < ((danLevelData->lv3Limiter) + limiterMod))
                    {
                        arrayRNG = system::rand() % (sizeof(mod::lv3Tribes) / 4);
                        tribeArray = 3;
                        enemyTribe = mod::lv3Tribes[arrayRNG];
                        wii::os::OSReport("Room #%d: tribeArray %d selected; (enemyGenRNG = %d) < (lv3Lim + limMod = %d). limMod = %d, lim = %d.\n", roomGens, tribeArray, enemyGenRNG, ((danLevelData->lv3Limiter) + limiterMod), limiterMod, danLevelData->lv3Limiter);
                    }
                    else
                    {
                        arrayRNG = system::rand() % (sizeof(mod::lv4Tribes) / 4);
                        tribeArray = 4;
                        enemyTribe = mod::lv4Tribes[arrayRNG];
                        wii::os::OSReport("Room #%d: tribeArray %d selected; (enemyGenRNG = %d) > (lv3Lim + limMod = %d). limMod = %d, lim = %d.\n", roomGens, tribeArray, enemyGenRNG, ((danLevelData->lv3Limiter) + limiterMod), limiterMod, danLevelData->lv3Limiter);
                    }
                    // Pulls danEnemy from the static array of DanNPCData structs.
                    mod::DanNPCData **danEnemies = mod::danNpcGetPtr();
                    mod::DanNPCData *danEnemy = danEnemies[enemyTribe];
                    vsOdds = system::rand() % 100;
                    // This loop runs through enemies of the same tribe array until it finds one that passes an odds check.
                    // It MUST NOT be an enemy that has already generated in this room.
                    do
                    {
                        if (tribeArray == 1)
                        {
                            arrayRNG = system::rand() % (sizeof(mod::lv1Tribes) / 4);
                            enemyTribe = mod::lv1Tribes[arrayRNG];
                        }
                        else if (tribeArray == 2)
                        {
                            arrayRNG = system::rand() % (sizeof(mod::lv2Tribes) / 4);
                            enemyTribe = mod::lv2Tribes[arrayRNG];
                        }
                        else if (tribeArray == 3)
                        {
                            arrayRNG = system::rand() % (sizeof(mod::lv3Tribes) / 4);
                            enemyTribe = mod::lv3Tribes[arrayRNG];
                        }
                        else
                        {
                            arrayRNG = system::rand() % (sizeof(mod::lv4Tribes) / 4);
                            enemyTribe = mod::lv4Tribes[arrayRNG];
                        }
                        vsOdds = system::rand() % 100;
                        danEnemy = danEnemies[enemyTribe];
                    } while (danEnemy->odds < vsOdds || danEnemy->name == eGenTR[0] || danEnemy->name == eGenTR[1] || danEnemy->name == eGenTR[2]);
                    // Once an enemy is determined, the number of times it will spawn in the Pit are determined by its min and max fields, as well as some other factors.
                    // This data is all thrown into enemyConfigArray, which is a master array of all NPC data read during the final generation step.
                    // The enemy's name is also thrown into eGenTR so that it does generate in this room again.
                    eGenTR[e] = danEnemy->name;
                    enemyConfigArray[enemyArrayVal] = danEnemy->name;
                    enemyArrayVal = enemyArrayVal + 1;
                    // First enemy type gets a relative enemy boost.
                    if (e == 0)
                    {
                        enemyDifference = (danEnemy->max - danEnemy->min) + danLevelData->bonusMax;
                        if (enemyDifference == 0)
                        {
                            enemyDifference = 1;
                        }
                        enemyAmt = (system::rand() % enemyDifference) + 1;
                        enemyAmt = enemyAmt + danEnemy->min;
                    }
                    // All other enemies get a relative enemy nerf, followed by a potential bonus.
                    else
                    {
                        if (danEnemy->min <= 3)
                        {
                            enemyMin = 1;
                        }
                        else if (danEnemy->min <= 5)
                        {
                            enemyMin = 2;
                        }
                        if (danEnemy->max <= 3)
                        {
                            enemyMax = 1;
                        }
                        else if (danEnemy->max <= 5)
                        {
                            enemyMax = 2;
                        }
                        else
                        {
                            enemyMax = 3;
                        }
                        enemyDifference = enemyMax - enemyMin + 1;
                        enemyBonus = system::rand() % (danLevelData->bonusMax + 1);
                        enemyBonus = enemyBonus + danLevelData->bonusMin;
                        enemyAmt = system::rand() % enemyDifference;
                        enemyAmt = enemyAmt + danEnemy->min + enemyBonus;
                    }
                    // If there's only one enemy type, it gets an extra bonus.
                    if (enemyTypes == 1)
                    {
                        if (boobies <= 25)
                        {
                            enemyAmt = enemyAmt + 2;
                        }
                        else if (boobies <= 75)
                        {
                            enemyAmt = enemyAmt + 3;
                        }
                        else
                        {
                            enemyAmt = enemyAmt + 4;
                        }
                    }
                    // One final boost to the enemy amount on Hard Difficulty!
                    if (difficulty == 2)
                    {
                        vsOdds = system::rand() % 100;
                        if (vsOdds > 50)
                        {
                            if (enemyTribe != 4)
                            {
                                enemyBonus = system::rand() % 3;
                            }
                            else
                            {
                                enemyBonus = system::rand() % 2;
                            }
                            enemyAmt = enemyAmt + enemyBonus;
                        }
                    }
                    enemyConfigArray[enemyArrayVal] = enemyAmt;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyName = (enemyArrayVal - 3);
                    wii::os::OSReport("Room #%d: %s generated %d times. lv1Limiter for this floor: %d. e = %d, enemyTypes = %d. %d (vsOdds) < %d (danEnemy odds).\n", roomGens, msgdrv::msgSearch(item_data::itemDataTable[npcdrv::npcGetTribe((danEnemy->name) - 1)->catchCardItemId].nameMsg), enemyAmt, danLevelData->lv1Limiter, e, enemyTypes, vsOdds, danEnemy->odds);
                    e = e + 1;
                }
                // Fill empty slots with dummy data if enemy types < 3
                if (enemyTypes == 1)
                {
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                }
                else if (enemyTypes == 2)
                {
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                }
            }
        }

        // Determine enemy data for current room
        if (moverRNG > 14)
        {
            for (i = 0; i < 200; ++i)
            {
                enemyArrayOffset = (currentFloor * 10 + 1);
                dan::dan_wp->dungeons[i].enemyCount = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                dan::dan_wp->dungeons[i].enemies[1].name = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                dan::dan_wp->dungeons[i].enemies[1].num = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                dan::dan_wp->dungeons[i].enemies[1].pos = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                if (enemyConfigArray[enemyArrayOffset] != 0)
                {
                    dan::dan_wp->dungeons[i].enemies[2].name = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    dan::dan_wp->dungeons[i].enemies[2].num = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    dan::dan_wp->dungeons[i].enemies[2].pos = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    if (enemyConfigArray[enemyArrayOffset] != 0)
                    {
                        dan::dan_wp->dungeons[i].enemies[3].name = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                        dan::dan_wp->dungeons[i].enemies[3].num = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                        dan::dan_wp->dungeons[i].enemies[3].pos = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                    }
                }
            }
        }
        //  Uncomment this and replace with any enemy name to add enemy to first 3 Floors. May break stuff sometimes
        /* dan::dan_wp->dungeons[0].enemies[1].name = 201;
         dan::dan_wp->dungeons[0].enemies[1].num = 10;
         dan::dan_wp->dungeons[0].enemies[2].name = 99;
         dan::dan_wp->dungeons[0].enemies[2].num = 10;
         dan::dan_wp->dungeons[0].enemies[3].name = 11;
         dan::dan_wp->dungeons[0].enemies[3].num = 7;
         dan::dan_wp->dungeons[0].enemyCount = 3; */

        // Replace Flimm inventory every floor; this sets a number of random items from the custom rotenShopItemPools.
        f32 flimmMult = 0;
        difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            flimmMult = 1.3;
            break;
        case 1:
            flimmMult = 1.6;
            break;
        case 2:
            flimmMult = 2;
            break;
        }
        s32 poolItem = 0;
        for (i = 0; i < 67; i = i + 3)
        {
            if (i <= 15)
            {
                poolItem = system::rand() % 86;
                newRotenShopItems[i] = rotenShopItemPool[poolItem];
                u16 cost = (u16)msl::math::floor(((u16)item_data::itemDataTable[rotenShopItemPool[poolItem]].buyPrice) * flimmMult);
                if (cost >= 999)
                {
                    cost = 999;
                }
                newRotenShopItems[i + 1] = cost;
            }
            else
            {
                poolItem = system::rand() % 19;
                newRotenShopItems[i] = rotenShopLowerClassItemPool[poolItem];
                u16 cost = (u16)msl::math::floor(((u16)item_data::itemDataTable[rotenShopLowerClassItemPool[poolItem]].buyPrice) * flimmMult);
                if (cost >= 999)
                {
                    cost = 999;
                }
                newRotenShopItems[i + 1] = cost;
            }
        }
        for (i = 0; i < 70; i++)
        {
            dan::dan_rotenShopItems[i] = newRotenShopItems[i];
        }

        // Mover logic
        if (moverRNG <= 14)
        {
            for (i = 0; i < 200; ++i)
            {
                dan::dan_wp->dungeons[i].enemies[1].name = 1;
                dan::dan_wp->dungeons[i].enemies[1].num = 1;
                dan::dan_wp->dungeons[i].enemies[1].pos = 26;
                dan::dan_wp->dungeons[i].enemyCount = 1;
                dan::dan_wp->dungeons[i].doors[0].enter = 28;
                dan::dan_wp->dungeons[i].doors[0].exit = 29;
                dan::dan_wp->dungeons[i].map = 0;
            }
            danClearMapParts();
        }

        blessingRoomCounter = swdrv::swByteGet(1602);
        if (blessingRoomCounter > 0)
        {
            blessingRoomCounter = blessingRoomCounter - 1;
            swdrv::swByteSet(1602, blessingRoomCounter);
            if (blessingRoomCounter == 0)
            {
                swdrv::swByteSet(1600, 0);
            }
        }

        s32 paramitaTimer = swdrv::swByteGet(1610);
        if (paramitaTimer > 0)
        {
            paramitaTimer = paramitaTimer + 2;
            swdrv::swByteSet(1610, paramitaTimer);
        }

        // Free pit text
        parse::parsePop();

        return EVT_RET_CONTINUE;
    }

    static const char *(*msgSearchReal)(const char *msgName);
    static const char *(*msgSearchNoFallbackReal)(const char *msgName);
    static const char *(*searchGetNpcMsgReal)(npcdrv::NPCEntry *npc);
    void (*pausewinSetMessageCardReal)(pausewin::PausewinEntry *entry, s32 itemId);
    u32 (*animGroupBaseAsyncReal)(const char *animPoseName, s32 param_2, void *readDoneCb);
    void (*spsndSFXOnReal)(const char *name);
    void (*spsndSFXOnVolReal)(const char *name, u8 volume);
    itemdrv::ItemEntry *(*itemEntryReal)(const char * name, s32 type, s32 behaviour, f64 x, f64 y, f64 z, evtmgr::EvtScriptCode * pickupScript, evtmgr::EvtVar switchNumber);
    static void messagePatch()
    {
        msgSearchReal = patch::hookFunction(msgdrv::msgSearch,
                                            [](const char *msgName)
                                            {
                                                // Handle custom music name modifiers & the blue whacka bump
                                                if (customSelectType == CustomSelects::SELECT_MUSIC)
                                                {
                                                    if (msl::string::strcmp(msgName, "in_honoo_sakuretsu") == 0)
                                                    {
                                                        return vMusicName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "in_koori_no_ibuki") == 0)
                                                    {
                                                        return ttMusicName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "in_kaminari_dokkan") == 0)
                                                    {
                                                        return plMusicName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "in_kirakira_otoshi") == 0)
                                                    {
                                                        return noMusicName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "in_pow_block") == 0)
                                                    {
                                                        return jdMusicName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "msg_window_select_1") == 0)
                                                    {
                                                        return selectMusicBox;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "msg_window_title_1") == 0)
                                                    {
                                                        return selectMusicBlueText;
                                                    }
                                                }
                                                if (customSelectType == CustomSelects::SELECT_FEATURES)
                                                {
                                                    if (msl::string::strcmp(msgName, "in_honoo_sakuretsu") == 0)
                                                    {
                                                        return moverFeaturesName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "in_koori_no_ibuki") == 0)
                                                    {
                                                        return merlunaFeaturesName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "in_kaminari_dokkan") == 0)
                                                    {
                                                        return marioFeaturesName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "msg_window_select_1") == 0)
                                                    {
                                                        return selectJimboBox;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "msg_window_title_1") == 0)
                                                    {
                                                        return selectJimboBlueText;
                                                    }
                                                }
                                                if (customSelectType == CustomSelects::SELECT_PATCHES)
                                                {
                                                    if (msl::string::strcmp(msgName, "in_honoo_sakuretsu") == 0)
                                                    {
                                                        return lockPatchesName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "in_koori_no_ibuki") == 0)
                                                    {
                                                        return hpPatchesName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "msg_window_select_1") == 0)
                                                    {
                                                        return selectJimboBox;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "msg_window_title_1") == 0)
                                                    {
                                                        return selectJimboBlueText;
                                                    }
                                                }
                                                if (customSelectType == CustomSelects::SELECT_ACCESSIBILITY)
                                                {
                                                    if (msl::string::strcmp(msgName, "in_honoo_sakuretsu") == 0)
                                                    {
                                                        return explosionShakeAccessName;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "msg_window_select_1") == 0)
                                                    {
                                                        return selectJimboBox;
                                                    }
                                                    else if (msl::string::strcmp(msgName, "msg_window_title_1") == 0)
                                                    {
                                                        return selectJimboBlueText;
                                                    }
                                                }
                                                if (msl::string::strcmp(msgName, "msg_blue_bump_name") == 0)
                                                {
                                                    return mystBumpName;
                                                }
                                                else if (msl::string::strcmp(msgName, "msg_blue_bump_desc") == 0)
                                                {
                                                    return mystBumpDesc;
                                                }
                                                else if (msl::string::strcmp(msgName, "msg_blue_bump_desc_ex") == 0)
                                                {
                                                    return mystBumpDesc;
                                                }
                                                // Patch all custom/modified NPCs to return their new name
                                                i = 0;
                                                while (patchEnemyList[i] != -1)
                                                {
                                                    if (msl::string::strcmp(msgName, item_data::itemDataTable[npcdrv::npcGetTribe(patchEnemyList[i])->catchCardItemId].nameMsg) == 0 && msl::string::strcmp(msgName, "ename_000") != 0)
                                                    {
                                                        return msgSearchTribeToTattle(0, patchEnemyList[i], Tribe2Tattle_Types::CARD_NAME);
                                                    }
                                                    ++i;
                                                };
                                                if (msl::string::strcmp(msgName, "D100_entrance_03") == 0)
                                                    // flopsite pit 1st pipe interaction (should never be seen)
                                                    return D100_entrance_03;
                                                else if (msl::string::strcmp(msgName, "mac_kanban_004") == 0)
                                                    // flopside pit entrance sign
                                                    return mac_kanban_004;
                                                else
                                                    return msgSearchReal(msgName);
                                            });

        msgSearchNoFallbackReal = patch::hookFunction(msgdrv::msgSearchNoFallback,
                                                      [](const char *msgName)
                                                      {
                                                          // Handle custom music description modifiers & the blue whacka bump
                                                          if (customSelectType == CustomSelects::SELECT_MUSIC)
                                                          {
                                                              if (msl::string::strcmp(msgName, "msg_honoo_sakuretsu_ex") == 0)
                                                              {
                                                                  return vMusicDesc;
                                                              }
                                                              else if (msl::string::strcmp(msgName, "msg_koori_no_ibuki_ex") == 0)
                                                              {
                                                                  return ttMusicDesc;
                                                              }
                                                              else if (msl::string::strcmp(msgName, "msg_kaminari_dokkan_ex") == 0)
                                                              {
                                                                  return plMusicDesc;
                                                              }
                                                              else if (msl::string::strcmp(msgName, "msg_kirakira_otoshi_ex") == 0)
                                                              {
                                                                  return noMusicDesc;
                                                              }
                                                              else if (msl::string::strcmp(msgName, "msg_pow_block_ex") == 0)
                                                              {
                                                                  return jdMusicDesc;
                                                              }
                                                          }
                                                          if (customSelectType == CustomSelects::SELECT_FEATURES)
                                                          {
                                                              if (msl::string::strcmp(msgName, "msg_honoo_sakuretsu_ex") == 0)
                                                              {
                                                                  return moverFeaturesDesc;
                                                              }
                                                              else if (msl::string::strcmp(msgName, "msg_koori_no_ibuki_ex") == 0)
                                                              {
                                                                  return merlunaFeaturesDesc;
                                                              }
                                                              else if (msl::string::strcmp(msgName, "msg_kaminari_dokkan_ex") == 0)
                                                              {
                                                                  return marioFeaturesDesc;
                                                              }
                                                          }
                                                          if (customSelectType == CustomSelects::SELECT_PATCHES)
                                                          {
                                                              if (msl::string::strcmp(msgName, "msg_honoo_sakuretsu_ex") == 0)
                                                              {
                                                                  return lockPatchesDesc;
                                                              }
                                                              else if (msl::string::strcmp(msgName, "msg_koori_no_ibuki_ex") == 0)
                                                              {
                                                                  return hpPatchesDesc;
                                                              }
                                                          }
                                                          if (customSelectType == CustomSelects::SELECT_ACCESSIBILITY)
                                                          {
                                                              if (msl::string::strcmp(msgName, "msg_honoo_sakuretsu_ex") == 0)
                                                              {
                                                                  return explosionShakeAccessDesc;
                                                              }
                                                          }
                                                          if (msl::string::strcmp(msgName, "msg_blue_bump_name") == 0)
                                                          {
                                                              return mystBumpName;
                                                          }
                                                          else if (msl::string::strcmp(msgName, "msg_blue_bump_desc") == 0)
                                                          {
                                                              return mystBumpDesc;
                                                          }
                                                          else if (msl::string::strcmp(msgName, "msg_blue_bump_desc_ex") == 0)
                                                          {
                                                              return mystBumpDesc;
                                                          }

                                                          // Patch all custom/modified NPCs to return their new name
                                                          i = 0;
                                                          while (patchEnemyList[i] != -1)
                                                          {
                                                              if (msl::string::strcmp(msgName, item_data::itemDataTable[npcdrv::npcGetTribe(patchEnemyList[i])->catchCardItemId].nameMsg) == 0 && msl::string::strcmp(msgName, "ename_000") != 0)
                                                              {
                                                                  return msgSearchTribeToTattle(0, patchEnemyList[i], Tribe2Tattle_Types::CARD_NAME);
                                                              }
                                                              ++i;
                                                          };
                                                          return msgSearchNoFallbackReal(msgName);
                                                      });

        searchGetNpcMsgReal = patch::hookFunction(search::searchGetNpcMsg,
                                                  [](npcdrv::NPCEntry *npc)
                                                  {
                                                      // Patch all custom/modified NPCs to return a custom tattle instead of a message name that doesn't exist
                                                      i = 0;
                                                      if (msl::string::strstr(npc->name, "rebear") != nullptr || msl::string::strstr(npc->name, "dan_card") != nullptr ||
                                                          msl::string::strstr(npc->name, "mover") != nullptr || msl::string::strstr(npc->name, "dan_koburon") != nullptr ||
                                                          msl::string::strstr(npc->name, "jimbo") != nullptr)
                                                      {
                                                          return msgSearchTribeToTattle(npc, npc->tribeId, Tribe2Tattle_Types::TATTLE);
                                                      }
                                                      while (patchEnemyList[i] != -1)
                                                      {
                                                          if (npc->tribeId == patchEnemyList[i])
                                                          {
                                                              return msgSearchTribeToTattle(npc, npc->tribeId, Tribe2Tattle_Types::TATTLE);
                                                          }
                                                          ++i;
                                                      };
                                                      return searchGetNpcMsgReal(npc);
                                                  });

        animGroupBaseAsyncReal = patch::hookFunction(animdrv::animGroupBaseAsync,
                                                     [](const char *animPoseName, s32 param_2, void *readDoneCb)
                                                     {
                                                         // Patch functions that rely on ag2tg to fileAsyncf custom textures
                                                         bool patch = false;
                                                         for (s32 idx = 0; msl::string::strcmp(models[idx], "terminator") != 0; idx = idx + 1)
                                                         {
                                                             if (msl::string::strcmp(animPoseName, models[idx]) == 0)
                                                             {
                                                                 patch = true;
                                                                 filemgr::FileEntry *texture = filemgr::fileAsyncf(4, 0, "%s/%s-", "a", animPoseName);
                                                                 return (-(s32)texture | (u32)texture) >> 0x1f;
                                                             }
                                                         }
                                                         if (!patch)
                                                             return animGroupBaseAsyncReal(animPoseName, param_2, readDoneCb);
                                                     });

        pausewinSetMessageCardReal = patch::hookFunction(pausewin::pausewinSetMessageCard,
                                                         [](pausewin::PausewinEntry *entry, s32 itemId)
                                                         {
                                                             // Patch all custom/modified NPCs to toss a custom description into pausewinCardDescBuf
                                                             i = 0;
                                                             while (patchEnemyList[i] != -1)
                                                             {
                                                                 if (item_data::itemDataTable[itemId].tribe == patchEnemyList[i])
                                                                 {
                                                                     entry->msg = msgSearchTribeToTattle(0, patchEnemyList[i], Tribe2Tattle_Types::CARD_DESC);
                                                                     u16 outlines[6] = {0, 0, 0, 0, 0, 0};
                                                                     fontmgr::FontGetMessageWidthLine(entry->msg, outlines);
                                                                     entry->curMsgLine = 0;
                                                                     entry->msgWidth = outlines[0] + 1;
                                                                     entry->unk_float = 0;
                                                                     return;
                                                                 }
                                                                 ++i;
                                                             };
                                                             pausewinSetMessageCardReal(entry, itemId);
                                                         });

        spsndSFXOnReal = patch::hookFunction(spmario_snd::spsndSFXOn,
                                             [](const char *name)
                                             {
                                                 u8 hpSoundsPatched = swdrv::swGet(1621);
                                                 if (hpSoundsPatched == 1)
                                                 {
                                                     if (msl::string::strcmp(name, "SFX_SYS_PINCH1") == 0 || msl::string::strcmp(name, "SFX_SYS_DANGER1") == 0)
                                                     {
                                                         return;
                                                     }
                                                 }
                                                 spmario_snd::__spsndSFXOn(name, -1, 255, 0, 0, 0);
                                                 return;
                                             });

        spsndSFXOnVolReal = patch::hookFunction(spmario_snd::spsndSFXOnVol,
                                                [](const char *name, u8 volume)
                                                {
                                                    u8 hpSoundsPatched = swdrv::swGet(1621);
                                                    if (hpSoundsPatched == 1)
                                                    {
                                                        if (msl::string::strcmp(name, "SFX_SYS_PINCH1") == 0 || msl::string::strcmp(name, "SFX_SYS_DANGER1") == 0)
                                                        {
                                                            return;
                                                        }
                                                    }
                                                    spmario_snd::__spsndSFXOn(name, volume, 255, 0, 0, 0);
                                                    return;
                                                });

        itemEntryReal = patch::hookFunction(itemdrv::itemEntry,
                                                [](const char * name, s32 type, s32 behaviour, f32 x, f32 y, f32 z, evtmgr::EvtScriptCode * pickupScript, evtmgr::EvtVar switchNumber)
                                                {
                                                    if (type == 1 && switchNumber == 0 && behaviour != 0) // Coins dropped by enemies are no longer searchable
                                                    {
                                                        itemdrv::ItemEntry * item = itemEntryReal(name, type, behaviour, x, y, z, pickupScript, switchNumber);
                                                        item->flags = (item->flags | 0x4000000);
                                                        return item;
                                                    }
                                                    return itemEntryReal(name, type, behaviour, x, y, z, pickupScript, switchNumber);
                                                });
    }

    // I literally had to port this entire thing just to override a condition that makes certain enemies force the Pit to fucking crash upon entry. Thank you Intelligent Systems, very cool
    s32 evt_dan_get_enemy_info_new(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)isFirstCall;

        // Get dungeon and enemy index
        evtmgr::EvtScriptCode *args = entry->pCurData;
        //    s32 no = evtmgr_cmd::evtGetValue(entry, args[0]);
        s32 enemyTribex = evtmgr_cmd::evtGetValue(entry, args[1]);
        dan::DanDungeon *dungeon = dan::dan_wp->dungeons + no;

        if ((enemyTribex < 0) || (enemyTribex >= 16))
        {
            // Return 0 for invalid enemies
            evtmgr_cmd::evtSetValue(entry, args[2], 0);
            evtmgr_cmd::evtSetValue(entry, args[3], 0);

            return EVT_RET_CONTINUE;
        }
        else
        {
            // Get enemy
            dan::DanEnemy *enemy = dungeon->enemies + enemyTribex;

            if (enemy->num > 0)
            {
                // Find template with correct tribe id
                s32 tribeId = enemy->name - 1;
                s32 i;
                npcdrv::NPCEnemyTemplate *curTemplate = npcdrv::npcEnemyTemplates;
                for (i = 0; i < NPCTEMPLATE_MAX; i++, curTemplate++)
                {
                    npcdrv::npcEnemyTemplates[10].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[13].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[74].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[76].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[55].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[101].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[321].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[322].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[323].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[324].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[82].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[316].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[317].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[318].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[319].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[320].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[276].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[277].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[279].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[23].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[125].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[30].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[113].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[200].unknown_0x8 = 0;
                    npcdrv::npcEnemyTemplates[332].unknown_0x8 = 0;

                    if (((curTemplate->unknown_0x8 & 1) == 0) && (curTemplate->tribeId == tribeId))
                        break;
                }
                assertf(i < NPCTEMPLATE_MAX, "みつかりませんでした[%d]", tribeId);

                // Return template id and num
                evtmgr_cmd::evtSetValue(entry, args[2], i);
                evtmgr_cmd::evtSetValue(entry, args[3], enemy->num);
            }
            else
            {
                // Return 0 for empty enemy slots
                evtmgr_cmd::evtSetValue(entry, args[2], 0);
                evtmgr_cmd::evtSetValue(entry, args[3], 0);
            }

            return EVT_RET_CONTINUE;
        }
    }

    static seqdef::SeqFunc *seq_gameMainReal;

    void youSuckDisplay(seqdrv::SeqWork *wp)
    {
        if (youSuck)
        {
            wii::gx::GXColor funnyColor = {255, 255, 255, 255};
            f32 scale = 2.8f;
            char buffer[50];
            const char *youSuckText = "YOU SUCK";
            msl::stdio::sprintf(buffer, "%s", youSuckText);
            const char *msg = buffer;
            fontmgr::FontDrawStart();
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColor();
            f32 x = -((fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            fontmgr::FontDrawString(x, 200.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void new_dan_gameover()
    {
        youSuck = true;
    }

    void merlunaBlessingDisplay(seqdrv::SeqWork *wp)
    {
        blessingRoomCounter = swdrv::swByteGet(1602);
        if (blessingRoomCounter > 0)
        {
            roomOnHud = swdrv::swGet(422);
            s32 alpha = 225;
            if (roomOnHud)
            {
                alpha = 80;
            }
            wii::gx::GXColor funnyColor = {18, 227, 178, 255};
            f32 scale = 0.64f;
            char buffer[100];
            blessingNum = swdrv::swByteGet(1600);
            houraiActivation = swdrv::swGet(1671);
            // Iterate through exceptional cases that modify message text
            if (houraiActivation)
            {
                funnyColor = {227, 178, 18, 255};
                msl::stdio::sprintf(buffer, "Affliction: Phoenix's Tail");
                if (roomOnHud)
                {
                    alpha = 80;
                }
            }
            else if (blessingNum == BlessId::MERLUNA_PARAMITA)
            {
                s32 paramitaTimer = swdrv::swByteGet(1610);
                if (paramitaTimer > 20)
                {
                    paramitaTimer = paramitaTimer - 20;
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Active, %d)", paramitaTimer);
                }
                else if (paramitaTimer > 0)
                {
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Cooldown, %d)", paramitaTimer);
                }
                else
                {
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Press B to use)", paramitaTimer);
                }
            }
            else
            {
                const char *blessingNames[] = {"DUMMY", "Spectre", "Hourai Doll", "Paramita"};
                const char *blessingDisp = blessingNames[blessingNum];
                msl::stdio::sprintf(buffer, "Blessing: %s", blessingDisp);
            }
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            fontmgr::FontDrawString(-360, -170.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void merlunaBlessingNumDisplay(seqdrv::SeqWork *wp)
    {
        blessingRoomCounter = swdrv::swByteGet(1602);
        if (blessingRoomCounter > 0)
        {
            roomOnHud = swdrv::swGet(422);
            s32 alpha = 225;
            if (roomOnHud)
            {
                alpha = 80;
            }
            wii::gx::GXColor funnyColor = {8, 110, 102, 255};
            f32 scale = 0.64f;
            char buffer[50];
            houraiActivation = swdrv::swGet(1671);
            if (houraiActivation)
            {
                funnyColor = {140, 110, 18, 225};
                if (roomOnHud)
                {
                    funnyColor = {140, 110, 18, 80};
                }
            }
            msl::stdio::sprintf(buffer, "Neutralizes in %d rooms", blessingRoomCounter);
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            fontmgr::FontDrawString(-360, -190.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void merlunaCurseDisplay(seqdrv::SeqWork *wp)
    {
        curseNum = swdrv::swByteGet(1601);
        if (curseNum > 0)
        {
            roomOnHud = swdrv::swGet(422);
            s32 alpha = 225;
            if (roomOnHud)
            {
                alpha = 80;
            }
            wii::gx::GXColor funnyColor = {138, 0, 207, 255};
            f32 scale = 0.64f;
            char buffer[100];
            const char *curseNames[] = {"DUMMY", "Tatarian Aster", "Vulnerability Hex", "Migraine"};
            const char *curseDisp = curseNames[curseNum];
            msl::stdio::sprintf(buffer, "Curse: %s", curseDisp);
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            fontmgr::FontDrawString(-360, -170.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void merlunaCurseNumDisplay(seqdrv::SeqWork *wp)
    {
        curseNum = swdrv::swByteGet(1601);
        if (curseNum > 0)
        {
            roomOnHud = swdrv::swGet(422);
            s32 alpha = 225;
            if (roomOnHud)
            {
                alpha = 80;
            }
            wii::gx::GXColor funnyColor = {71, 4, 99, 255};
            f32 scale = 0.64f;
            char buffer[50];
            msl::stdio::sprintf(buffer, "Pay Merluna to clear");
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            fontmgr::FontDrawString(-360, -190.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void disorderDisplay(seqdrv::SeqWork *wp)
    {
        disorderNum = swdrv::swByteGet(1630);
        if (disorderNum > 0)
        {
            s32 alpha = 225;
            wii::gx::GXColor funnyColor = {25, 25, 25, 255};
            switch (disorderNum)
            {
                case DisorderId::DISORDER_RED:
                funnyColor.r = 255;
                break;
                case DisorderId::DISORDER_ORANGE:
                funnyColor.r = 255;
                funnyColor.g = 127;
                break;
                case DisorderId::DISORDER_YELLOW:
                funnyColor.r = 220;
                funnyColor.g = 210;
                break;
                case DisorderId::DISORDER_GREEN:
                funnyColor.r = 30;
                funnyColor.g = 255;
                funnyColor.b = 70;
                break;
                case DisorderId::DISORDER_CYAN:
                funnyColor.r = 50;
                funnyColor.g = 230;
                funnyColor.b = 255;
                break;
                case DisorderId::DISORDER_BLUE:
                funnyColor.r = 40;
                funnyColor.g = 60;
                funnyColor.b = 255;
                break;
                case DisorderId::DISORDER_PURPLE:
                funnyColor.r = 128;
                funnyColor.g = 64;
                funnyColor.b = 255;
                break;
                case DisorderId::DISORDER_WHITE:
                funnyColor.r = 255;
                funnyColor.g = 255;
                funnyColor.b = 255;
                break;
            }
            f32 scale = 0.64f;
            char buffer[100];
            const char *disorderNames[] = {"DUMMY", "Apathy", "Dread", "Prejudice", "Indifference", "Recalcitrance", "Depravity", "Servility", "Melancholy", "Ruin"};
            const char *disorderDisp = disorderNames[disorderNum];
            msl::stdio::sprintf(buffer, "Disorder: %s", disorderDisp);
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            f32 x = (360 - (fontmgr::FontGetMessageWidth(msg) * scale));
            fontmgr::FontDrawString(x, -170.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void disorderNumDisplay(seqdrv::SeqWork *wp)
    {
        disorderRooms = swdrv::swByteGet(1631);
        if (disorderRooms > 0)
        {
            s32 alpha = 225;
            wii::gx::GXColor funnyColor = {25, 25, 25, 255};
            disorderNum = swdrv::swByteGet(1630);
            switch (disorderNum)
            {
                case DisorderId::DISORDER_RED:
                funnyColor.r = 125;
                break;
                case DisorderId::DISORDER_ORANGE:
                funnyColor.r = 125;
                funnyColor.g = 63;
                break;
                case DisorderId::DISORDER_YELLOW:
                funnyColor.r = 105;
                funnyColor.g = 100;
                break;
                case DisorderId::DISORDER_GREEN:
                funnyColor.r = 10;
                funnyColor.g = 125;
                funnyColor.b = 30;
                break;
                case DisorderId::DISORDER_CYAN:
                funnyColor.g = 90;
                funnyColor.b = 115;
                break;
                case DisorderId::DISORDER_BLUE:
                funnyColor.g = 40;
                funnyColor.b = 127;
                break;
                case DisorderId::DISORDER_PURPLE:
                funnyColor.r = 64;
                funnyColor.g = 42;
                funnyColor.b = 127;
                break;
                case DisorderId::DISORDER_WHITE:
                funnyColor.r = 127;
                funnyColor.g = 127;
                funnyColor.b = 127;
                break;
            }
            f32 scale = 0.64f;
            char buffer[50];
            msl::stdio::sprintf(buffer, "%d rooms remaining", disorderRooms);
            const char *msg = buffer;
            fontmgr::FontDrawStart_alpha(alpha);
            fontmgr::FontDrawEdge();
            fontmgr::FontDrawColor(&funnyColor);
            fontmgr::FontDrawScale(scale);
            fontmgr::FontDrawNoiseOff();
            fontmgr::FontDrawRainbowColorOff();
            f32 x = (360 - (fontmgr::FontGetMessageWidth(msg) * scale));
            fontmgr::FontDrawString(x, -190.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void textDisplay(seqdrv::SeqWork *wp)
    {
        merlunaBlessingDisplay(wp);
        merlunaBlessingNumDisplay(wp);
        merlunaCurseDisplay(wp);
        merlunaCurseNumDisplay(wp);
        disorderDisplay(wp);
        disorderNumDisplay(wp);
        youSuckDisplay(wp);
    }

    static seqdef::SeqFunc *seq_titleMainReal;
    static void seq_titleMainOverride(seqdrv::SeqWork *wp)
    {
        wii::gx::GXColor notgreen = {230, 116, 216, 255};
        f32 scale = 0.8f;
        const char *msg = "SPM Pit Randomizer beta v2.2";
        fontmgr::FontDrawStart();
        fontmgr::FontDrawEdge();
        fontmgr::FontDrawColor(&notgreen);
        fontmgr::FontDrawScale(scale);
        fontmgr::FontDrawNoiseOff();
        fontmgr::FontDrawRainbowColorOff();
        f32 x = -((fontmgr::FontGetMessageWidth(msg) * scale) / 2);
        fontmgr::FontDrawString(x, 200.0f, msg);
        seq_titleMainReal(wp);
    }
    static void titleScreenCustomTextPatch()
    {
        seq_titleMainReal = seqdef::seq_data[seqdrv::SEQ_TITLE].main;
        seqdef::seq_data[seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
        seq_gameMainReal = seqdef::seq_data[seqdrv::SEQ_GAME].main;
        seqdef::seq_data[seqdrv::SEQ_GAME].main = &textDisplay;
    }

    static void danOverwrite()
    {
        patch::hookFunction(dan::evt_dan_read_data, evt_dan_read_data_new);
        patch::hookFunction(seq_title::getNextDanMapname, getNextDanMapnameNew);
        writeBranchLink(mapdrv::mapLoad, 0x3B8, loadNewDanTex);
        writeBranchLink(mot_damage::onHpEquals0, 0x68, handleHouraiDoll);
        writeBranchLink(hud::hudMain, 0x47C, disorderCoinAttrition);
        // just throwing this here
        writeBranchLink(temp_unk::backCursyaHit, 0x104, patchBackCursya);
        // also throwing these here
        writeBranchLink(npcdrv::func_801b19e4, 0x6B0, neutralize);
        writeBranchLink(npcdrv::func_801b19e4, 0x694, neutralize);
        writeBranchLink(npcdrv::func_801b19e4, 0x588, neutralize);
        writeBranchLink(npcdrv::func_801b19e4, 0x494, neutralize);
        // lol why not
        writeBranchLink(framedrv::frameDispCb, 0x3EC, debugModeGayFrame);
    }

    static void danYouSuck()
    {
        patch::hookFunction(dan::danCountdownDone, new_dan_gameover);
    }

    static void danDontFuckingCrash()
    {
        patch::hookFunction(dan::evt_dan_get_enemy_info, evt_dan_get_enemy_info_new);
    }

    static void dimenPatch()
    {
        patch::hookFunction(npc_dimeen_l::npc_dimen_determine_move_pos, dimen_determine_move_pos_new);
    }

    s32 updateDebugFrameColor(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // Every frame, progress color(s) by 4 units.
        // PHASE 0    PHASE 1      PHASE 2    PHASE 3      PHASE 4    PHASE 5      PHASE 0
        // 255,0,0 -> 255,255,0 -> 0,255,0 -> 0,255,255 -> 0,0,255 -> 255,0,255 -> 255,0,0
        switch (frameColPhase)
        {
            case 0:
            frameG = frameG + 16;
            if (frameG > 255)
            {
                frameG = 255;
                frameColPhase = 1;
            }
            break;
            case 1:
            frameR = frameR - 16;
            if (frameR < 0)
            {
                frameR = 0;
                frameColPhase = 2;
            }
            break;
            case 2:
            frameB = frameB + 16;
            if (frameB > 255)
            {
                frameB = 255;
                frameColPhase = 3;
            }
            break;
            case 3:
            frameG = frameG - 16;
            if (frameG < 0)
            {
                frameG = 0;
                frameColPhase = 4;
            }
            break;
            case 4:
            frameR = frameR + 16;
            if (frameR > 255)
            {
                frameR = 255;
                frameColPhase = 5;
            }
            break;
            case 5:
            frameB = frameB - 16;
            if (frameB < 0)
            {
                frameB = 0;
                frameColPhase = 0;
            }
            break;
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(updateDebugFrameColor, 0)

    s32 bump_use_selected(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        pausewin::pausewin_pluswinWp->selectedItemId = 45;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(bump_use_selected, 0)

    s32 animPoseSetMaterialEvtColorWrapper(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 blueToggle = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        mario::MarioWork *mario = mario::marioGetPtr();
        s32 marioRemoved = swdrv::swGet(1612);
        /*    // DEBUG
            if (spmario::gp->frameCounter % 30 == 0)
            {
                wii::gx::GXColor col = animdrv::animPoseGetMaterialEvtColor(mario->animPoseIds[0]);
                wii::os::OSReport("AnimPose[0] GXColor: %d, %d, %d, %d\n", col.r, col.g, col.b, col.a);
            } */
        // END DEBUG
        if (mario->character == 0 && marioRemoved == 1)
        {
            static wii::gx::GXColor marioIfHeWereInvisible = {255, 255, 255, 0};
            for (i = 0; i < 8; ++i)
            {
                if (mario->animPoseIds[i] != -1)
                {
                    animdrv::animPoseSetMaterialEvtColor(mario->animPoseIds[i], marioIfHeWereInvisible);
                    animdrv::animPoseSetMaterialFlagOn(mario->animPoseIds[i], 0x2000);
                }
            }
            if ((mario->effectFlags & 0x2) != 0) // Remove stylish trail
            {
                mario->effectFlags = mario->effectFlags & ~0x2;
            }
        }
        else if (blueToggle == 1)
        {
            static wii::gx::GXColor marioIfHeWereBlue = {60, 100, 255, 255};
            for (i = 0; i < 8; ++i)
            {
                if (mario->animPoseIds[i] != -1)
                {
                    animdrv::animPoseSetMaterialEvtColor(mario->animPoseIds[i], marioIfHeWereBlue);
                    animdrv::animPoseSetMaterialFlagOn(mario->animPoseIds[i], 0x2000);
                }
            }
        }
        else
        {
            static wii::gx::GXColor marioIfHeWerentBlue = {255, 255, 255, 255};
            for (i = 0; i < 8; ++i)
            {
                if (mario->animPoseIds[i] != -1)
                {
                    animdrv::animPoseSetMaterialEvtColor(mario->animPoseIds[i], marioIfHeWerentBlue);
                    animdrv::animPoseSetMaterialFlagOff(mario->animPoseIds[i], 0x2000);
                }
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(animPoseSetMaterialEvtColorWrapper, 1)

    s32 update_bump_tex(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // Whacka Bump
        if (swdrv::swGet(1643) == 0)
        {
            item_data::itemDataTable[45].iconId = 0 + TPLPATCH_ICON_REDIRECT; // Icon/Item ID of the unused peach in vanilla
        }
        else
        {
            item_data::itemDataTable[45].iconId = 61; // Icon of unused "negative" gray key in vanilla
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(update_bump_tex, 0)

    EVT_BEGIN(bump_use_evt)
    IF_EQUAL(GSWF(1643), 0)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(bumpUseMsg), 0, 0)
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(bumpUnuseMsg), 0, 0)
    END_IF()
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(yesNoSelect_YesByDefault))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 0)
    USER_FUNC(bump_use_selected)
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(bump_item_use)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    // GENERATE EFFS
    // WHITE
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(10), LW(11), LW(12))
    USER_FUNC(evt_mario::evt_mario_get_height, LW(13))
    DIVF(LW(13), FLOAT(2.0))
    ADDF(LW(11), LW(13))
    ADDF(LW(10), FLOAT(10))
    ADDF(LW(12), FLOAT(10))
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(10), LW(11), LW(12))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 8, LW(10), LW(11), LW(12), FLOAT(1.5), 0, 0, 0, 0, 0, 0, 0)
    SUBF(LW(12), FLOAT(20))
    SUBF(LW(10), FLOAT(20))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 8, LW(10), LW(11), LW(12), FLOAT(1.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(30)
    // PURPLE
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(10), LW(11), LW(12))
    USER_FUNC(evt_mario::evt_mario_get_height, LW(13))
    DIVF(LW(13), FLOAT(2.0))
    ADDF(LW(11), LW(13))
    ADDF(LW(10), FLOAT(10))
    ADDF(LW(12), FLOAT(10))
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(10), LW(11), LW(12))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 13, LW(10), LW(11), LW(12), FLOAT(2.2), 0, 0, 0, 0, 0, 0, 0)
    SUBF(LW(12), FLOAT(20))
    SUBF(LW(10), FLOAT(20))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 13, LW(10), LW(11), LW(12), FLOAT(2.2), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
    // END EFF GENERATION
    IF_EQUAL(GSWF(1643), 0)
    SET(GSWF(1643), 1)
    ELSE()
    SET(GSWF(1643), 0)
    END_IF()
    USER_FUNC(update_bump_tex)
    WAIT_MSEC(1200)
    IF_EQUAL(GSWF(1643), 0)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(bumpYoureNotBlueNow), 0, 0)
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(bumpYoureBlueNow), 0, 0)
    END_IF()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
    EVT_END()

    EVT_BEGIN(bump_item_use_hook)
    IF_EQUAL(LW(5), 45)
    RUN_EVT(bump_item_use)
    RETURN()
    END_IF()
    RETURN_FROM_CALL()

    static mario_pouch::MarioPouchWork *pluswinWhackaBumpFunc()
    {
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        if (pouch->keyItem[pausewin::pausewin_pluswinWp->submenuSelectedButton] == 45)
        {
            spmario_snd::spsndSFXOn("SFX_SYS_MENU_DESIDE1");
            pausewin::pausewin_pluswinWp->keyItemEvt = evtmgr::evtEntryType((EvtScriptCode *)bump_use_evt, 0, 0, 0);
            pausewin::pausewin_pluswinWp->keyItemEvtId = pausewin::pausewin_pluswinWp->keyItemEvt->id;
        }
        return mario_pouch::pouchGetPtr();
    }

    static void pluswinWhackaBump()
    {
        writeBranchLink(pausewin::pluswinKeyItemMain, 0x5E4, pluswinWhackaBumpFunc);
    }

    static void patchNpcRgbaFuncs()
    {
        patch::hookFunction(evt_npc::evt_npc_set_color, [](evtmgr::EvtEntry *evtEntry, bool firstRun)
                            {
            // Patch all NPCs with Tribe IDs to ignore this function, except for Shadoo.
            evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
            char * name = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
            npcdrv::NPCEntry * npc = evt_npc::evtNpcNameToPtr(evtEntry, name);
            if (npc->tribeId > 0)
            {
                if (npc->tribeId <= 333 && npc->tribeId >= 330)
                {
                    (npc->m_Anim).red = 127;
                    (npc->m_Anim).green = 127;
                    (npc->m_Anim).blue = 127;
                    animdrv::animPoseSetDispCallback2((npc->m_Anim).m_nPoseId, mi4::mi4MimiHolographicEffect, evtEntry);
                }
                else return 2;
            }
            else
            {
                s32 red = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
                s32 green = evtmgr_cmd::evtGetValue(evtEntry, args[2]);
                s32 blue = evtmgr_cmd::evtGetValue(evtEntry, args[3]);
                s32 alpha = evtmgr_cmd::evtGetValue(evtEntry, args[4]);
                // a builtin clamp function that isn't evt opcode CLAMP_INT would be really nice
                if (red > 255) red = 255;
                if (red < 0) red = 0;
                if (blue > 255) blue = 255;
                if (blue < 0) blue = 0;
                if (green > 255) green = 255;
                if (green < 0) green = 0;
                if (alpha > 255) alpha = 255;
                if (alpha < 0) alpha = 0;
                (npc->m_Anim).red = (u8)red;
                (npc->m_Anim).blue = (u8)blue;
                (npc->m_Anim).green = (u8)green;
                (npc->m_Anim).alpha = (u8)alpha;
            }
            return 2; });

        patch::hookFunction(evt_npc::evt_npc_set_colorcopy, [](evtmgr::EvtEntry *entry, bool isFirstCall)
                            {
            (void)entry;
            (void)isFirstCall;
            return 2; });
        patch::hookFunction(evt_npc::evt_npc_set_colorcopytwo, [](evtmgr::EvtEntry *entry, bool isFirstCall)
                            {
            (void)entry;
            (void)isFirstCall;
            return 2; });
    }

    static void danPatchPitEnemies()
    {
        // Define default animposes for new enemies
        npcdrv::npcTribes[5].animPoseName = "e_kuribo_y";     // Unused Spiked Goomba --> Spiked Gloomba
        npcdrv::npcTribes[2].animPoseName = "e_kuribo_h";     // Dark Goomba --> Hyper Goomba
        npcdrv::npcTribes[6].animPoseName = "e_kuribo_h";     // Dark Spiked Goomba --> Hyper Spiked Goomba
        npcdrv::npcTribes[8].animPoseName = "e_kuribo_h";     // Dark Paragoomba --> Hyper Paragoomba
        npcdrv::npcTribes[16].animPoseName = "e_nokoteki_d";  // Dark Koopa --> Dark Koopa
        npcdrv::npcTribes[24].animPoseName = "e_nokoteki_d";  // Dark Paratroopa --> Dark Paratroopa
        npcdrv::npcTribes[446].animPoseName = "e_cheririn_a"; // Dark Cherbil --> Ash Cherbil
        npcdrv::npcTribes[38].animPoseName = "e_togezb";      // Dark Spiny --> Sky-Blue Spiny
        npcdrv::npcTribes[29].animPoseName = "e_mer";         // Dark Spike Top --> Red Spike Top
        npcdrv::npcTribes[98].animPoseName = "e_chorobon_g";  // Dark Fuzzy --> Green Fuzzy
        npcdrv::npcTribes[43].animPoseName = "e_karon_d";     // Dark Dull Bones --> Dark Bones
        npcdrv::npcTribes[44].animPoseName = "e_karon_d";     // Dark Bones projectile
        npcdrv::npcTribes[111].animPoseName = "e_kmond";      // Dark Ruff Puff --> Dark Puff
        npcdrv::npcTribes[95].animPoseName = "e_gabow";       // Dark Clubba --> White Clubba
        npcdrv::npcTribes[66].animPoseName = "e_kames";       // Dark Magikoopa --> Shady Magikoopa
        npcdrv::npcTribes[67].animPoseName = "e_kames";       // Dark Broom Magikoopa --> Shady Broom Magikoopa
        npcdrv::npcTribes[124].animPoseName = "e_wanwan_g";   // Dark Chomp --> Gold Chomp
        npcdrv::npcTribes[167].animPoseName = "e_ntl_p";      // Dark Tileoid --> Tileoid PU
        npcdrv::npcTribes[220].animPoseName = "e_nin_d";      // Dark Ninjoe --> Ninjeremiah
        npcdrv::npcTribes[221].animPoseName = "e_nin_d";      // Ninjeremiah projectile
        npcdrv::npcTribes[19].animPoseName = "e_togenokd";    // Dark Koopatrol --> Dark Koopatrol
        npcdrv::npcTribes[171].animPoseName = "e_jyama_b";    // Dark Jawbus --> Bawbus
        npcdrv::npcTribes[10].animPoseName = "e_k_kuribo";    // Dark Headbonk Goomba --> Kamikaze Goomba
        npcdrv::npcTribes[225].animPoseName = "e_tesita_bt";  // Dark Skellobit --> Skellobyte
        npcdrv::npcTribes[228].animPoseName = "e_tesita_sb";  // Dark Spiky Skellobit --> Spiky Skellobyte
        npcdrv::npcTribes[49].animPoseName = "e_buross_h";    // Dark Hammer Bro --> Shady Hammer Bro
        npcdrv::npcTribes[50].animPoseName = "e_buross_h";    // Shady Hammer Bro projectile
        npcdrv::npcTribes[55].animPoseName = "e_buross_b";    // Dark Boomerang Bro --> Shady Boomerang Bro
        npcdrv::npcTribes[56].animPoseName = "e_buross_b";    // Shady Boomerang Bro projectile
        npcdrv::npcTribes[61].animPoseName = "e_burosu_i";    // Dark Fire Bro --> Ice Bro
        npcdrv::npcTribes[62].animPoseName = "e_burosu_i";    // Ice Bro projectile
        npcdrv::npcTribes[101].animPoseName = "e_sinemoh";    // Dark Cleft --> Hyper Cleft
        npcdrv::npcTribes[75].animPoseName = "e_k_shoote4";   // Dark Striker --> Shady Striker
        npcdrv::npcTribes[86].animPoseName = "e_teresa_b";    // Dark Dark Boo --> Bomb Boo

        // Replace the Sky-Blue Spiny's parts list so its partsList[1] animPoseName displays properly as e_togezb
        msl::string::memcpy(&sbParts[0], &npcdrv::npcTribes[38].partsList[0], sizeof(npcdrv::npcTribes[38].partsList[0]));
        msl::string::memcpy(&sbParts[1], &npcdrv::npcTribes[38].partsList[1], sizeof(npcdrv::npcTribes[38].partsList[1]));
        npcdrv::npcTribes[38].partsList = sbParts;
        npcdrv::npcTribes[38].partsList[1].animPoseName = "e_togezb";

        // Patch Gold Chomps so their parts appear correctly!
        msl::string::memcpy(&gcParts[0], &npcdrv::npcTribes[124].partsList[0], sizeof(npcdrv::npcTribes[124].partsList[0]));
        msl::string::memcpy(&gcParts[1], &npcdrv::npcTribes[124].partsList[1], sizeof(npcdrv::npcTribes[124].partsList[1]));
        msl::string::memcpy(&gcParts[2], &npcdrv::npcTribes[124].partsList[2], sizeof(npcdrv::npcTribes[124].partsList[2]));
        msl::string::memcpy(&gcParts[3], &npcdrv::npcTribes[124].partsList[3], sizeof(npcdrv::npcTribes[124].partsList[3]));
        msl::string::memcpy(&gcParts[4], &npcdrv::npcTribes[124].partsList[4], sizeof(npcdrv::npcTribes[124].partsList[4]));
        msl::string::memcpy(&gcParts[5], &npcdrv::npcTribes[124].partsList[5], sizeof(npcdrv::npcTribes[124].partsList[5]));
        msl::string::memcpy(&gcParts[6], &npcdrv::npcTribes[124].partsList[6], sizeof(npcdrv::npcTribes[124].partsList[6]));
        npcdrv::npcTribes[124].partsList = gcParts;
        npcdrv::npcTribes[124].partsList[1].animPoseName = "e_wanwan_g";
        npcdrv::npcTribes[124].partsList[2].animPoseName = "e_wanwan_g";
        npcdrv::npcTribes[124].partsList[3].animPoseName = "e_wanwan_g";
        npcdrv::npcTribes[124].partsList[4].animPoseName = "e_wanwan_g";
        npcdrv::npcTribes[124].partsList[5].animPoseName = "e_wanwan_g";
        npcdrv::npcTribes[124].partsList[6].animPoseName = "e_wanwan_g";
        item_data::itemDataTable[359].animPoseName = "e_wanwan_g";
        npcdrv::npcTribes[124].coinDropChance = 100;
        npcdrv::npcTribes[124].coinDropBaseCount = 25;
        npcdrv::npcTribes[124].coinDropExtraChance = 80;
        npcdrv::npcTribes[124].coinDropExtraMax = 50;

        // Bawbus
        msl::string::memcpy(&bbParts[0], &npcdrv::npcTribes[171].partsList[0], sizeof(npcdrv::npcTribes[171].partsList[0]));
        msl::string::memcpy(&bbParts[1], &npcdrv::npcTribes[171].partsList[1], sizeof(npcdrv::npcTribes[171].partsList[1]));
        msl::string::memcpy(&bbParts[2], &npcdrv::npcTribes[171].partsList[2], sizeof(npcdrv::npcTribes[171].partsList[2]));
        msl::string::memcpy(&bbParts[3], &npcdrv::npcTribes[171].partsList[3], sizeof(npcdrv::npcTribes[171].partsList[3]));
        msl::string::memcpy(&bbParts[4], &npcdrv::npcTribes[171].partsList[4], sizeof(npcdrv::npcTribes[171].partsList[4]));
        msl::string::memcpy(&bbParts[5], &npcdrv::npcTribes[171].partsList[5], sizeof(npcdrv::npcTribes[171].partsList[5]));
        msl::string::memcpy(&bbParts[6], &npcdrv::npcTribes[171].partsList[6], sizeof(npcdrv::npcTribes[171].partsList[6]));
        msl::string::memcpy(&bbParts[7], &npcdrv::npcTribes[171].partsList[7], sizeof(npcdrv::npcTribes[171].partsList[7]));
        msl::string::memcpy(&bbParts[8], &npcdrv::npcTribes[171].partsList[8], sizeof(npcdrv::npcTribes[171].partsList[8]));
        msl::string::memcpy(&bbParts[9], &npcdrv::npcTribes[171].partsList[9], sizeof(npcdrv::npcTribes[171].partsList[9]));
        npcdrv::npcTribes[171].partsList = bbParts;
        npcdrv::npcTribes[171].partsList[1].animPoseName = "e_jyama_b";
        npcdrv::npcTribes[171].partsList[4].animPoseName = "e_jyama_b";
        npcdrv::npcTribes[171].partsList[5].animPoseName = "e_jyama_b";
        npcdrv::npcTribes[171].partsList[6].animPoseName = "e_jyama_b";
        npcdrv::npcTribes[171].partsList[7].animPoseName = "e_jyama_b";
        npcdrv::npcTribes[171].partsList[8].animPoseName = "e_jyama_b";
        npcdrv::npcTribes[171].partsList[9].animPoseName = "e_jyama_b";
        item_data::itemDataTable[388].animPoseName = "e_card_jyama_b";

        // Patch Dark Enemies from appearing as all-black on their Cards.
        s32 id = 0;
        for (id = 282; id < 537; ++id)
        {
            item_data::itemDataTable[id].dispFlags = 0;
        }

        /* Didn't pan out, maybe one day though.
                // Just for fun, let's make Dark Luigi actually be Luigi
                npcdrv::npcTribes[331].animPoseName = "p_luigi";
                npcdrv::npcTribes[331].animDefs = luigiAnims;
                */

        // Adjust new NPC stats
        // Spinia
        npcdrv::npcTribes[495].catchCardItemId = 344;
        item_data::itemDataTable[344].tribe = 495;
        item_data::itemDataTable[344].animPoseName = "e_hannya_g";
        item_data::itemDataTable[344].animName = "Z_1";
        npcdrv::npcTribes[495].catchCardDefense = 5;
        npcdrv::npcTribes[495].maxHp = 4;
        npcdrv::npcTribes[495].killXp = 200;
        npcdrv::npcTribes[495].coinDropChance = 100;
        npcdrv::npcTribes[495].coinDropBaseCount = 0;
        npcdrv::npcTribes[495].coinDropExtraChance = 45;
        npcdrv::npcTribes[495].coinDropExtraMax = 1;
        npcdrv::npcTribes[495].attackStrength = 1;
        npcdrv::npcTribes[495].bounceEjection = 2;
        npcdrv::npcTribes[495].jumpEjection = 0;

        // Spunia
        npcdrv::npcTribes[496].catchCardItemId = 350;
        item_data::itemDataTable[350].tribe = 496;
        item_data::itemDataTable[350].animPoseName = "e_hannya_p";
        item_data::itemDataTable[350].animName = "Z_1";
        npcdrv::npcTribes[496].catchCardDefense = 20;
        npcdrv::npcTribes[496].maxHp = 9;
        npcdrv::npcTribes[496].killXp = 600;
        npcdrv::npcTribes[496].coinDropChance = 100;
        npcdrv::npcTribes[496].coinDropBaseCount = 0;
        npcdrv::npcTribes[496].coinDropExtraChance = 45;
        npcdrv::npcTribes[496].coinDropExtraMax = 3;
        npcdrv::npcTribes[496].attackStrength = 3;
        npcdrv::npcTribes[496].bounceEjection = 2;
        npcdrv::npcTribes[496].jumpEjection = 0;

        // Shady Koopa, ATK 5 DEF 3
        npcdrv::npcTribes[465].catchCardItemId = 304;
        item_data::itemDataTable[304].tribe = 465;
        item_data::itemDataTable[304].animPoseName = "e_uranoko";
        item_data::itemDataTable[304].animName = "NKT_Z_1";
        npcdrv::npcTribes[465].catchCardDefense = 30;
        npcdrv::npcTribes[465].maxHp = 15;
        npcdrv::npcTribes[465].killXp = 300;
        npcdrv::npcTribes[465].coinDropChance = 100;
        npcdrv::npcTribes[465].coinDropBaseCount = 0;
        npcdrv::npcTribes[465].coinDropExtraChance = 75;
        npcdrv::npcTribes[465].coinDropExtraMax = 5;
        npcdrv::npcTribes[465].attackStrength = 4;
        npcdrv::npcTribes[465].bounceEjection = 1;
        npcdrv::npcTribes[465].jumpEjection = 0;

        // Flip Shady Koopa, ATK 5 DEF 3
        npcdrv::npcTribes[466].catchCardItemId = 304;
        npcdrv::npcTribes[466].catchCardDefense = 30;
        npcdrv::npcTribes[466].maxHp = 15;
        npcdrv::npcTribes[466].killXp = 300;
        npcdrv::npcTribes[466].coinDropChance = 100;
        npcdrv::npcTribes[466].coinDropBaseCount = 0;
        npcdrv::npcTribes[466].coinDropExtraChance = 75;
        npcdrv::npcTribes[466].coinDropExtraMax = 5;
        npcdrv::npcTribes[466].attackStrength = 4;
        npcdrv::npcTribes[466].bounceEjection = 1;
        npcdrv::npcTribes[466].jumpEjection = 0;

        // Spiked Gloomba, ATK 3
        npcdrv::npcTribes[5].catchCardItemId = 336;
        item_data::itemDataTable[336].tribe = 5;
        item_data::itemDataTable[336].animPoseName = "e_kuribo_y";
        item_data::itemDataTable[336].animName = "toge_kuribo_Z_2";
        npcdrv::npcTribes[5].catchCardDefense = 6;
        npcdrv::npcTribes[5].maxHp = 2;
        npcdrv::npcTribes[5].killXp = 200;
        npcdrv::npcTribes[5].coinDropChance = 100;
        npcdrv::npcTribes[5].coinDropBaseCount = 0;
        npcdrv::npcTribes[5].coinDropExtraChance = 45;
        npcdrv::npcTribes[5].coinDropExtraMax = 1;
        npcdrv::npcTribes[5].attackStrength = 3;

        // Green Magikoopa, DEF 2
        npcdrv::npcTribes[473].catchCardItemId = 353;
        item_data::itemDataTable[353].tribe = 473;
        item_data::itemDataTable[353].animPoseName = "e_kamek_g";
        item_data::itemDataTable[353].animName = "KMG_Z_1";
        npcdrv::npcTribes[473].catchCardDefense = 20;
        npcdrv::npcTribes[473].maxHp = 6;
        npcdrv::npcTribes[473].killXp = 600;
        npcdrv::npcTribes[473].coinDropChance = 100;
        npcdrv::npcTribes[473].coinDropBaseCount = 0;
        npcdrv::npcTribes[473].coinDropExtraChance = 60;
        npcdrv::npcTribes[473].coinDropExtraMax = 4;
        npcdrv::npcTribes[473].attackStrength = 3;
        npcdrv::npcTribes[473].bounceEjection = 2;
        npcdrv::npcTribes[473].jumpEjection = 0;

        // Green Broom Magikoopa, DEF 2
        npcdrv::npcTribes[474].catchCardItemId = 353;
        npcdrv::npcTribes[474].catchCardDefense = 20;
        npcdrv::npcTribes[474].maxHp = 6;
        npcdrv::npcTribes[474].killXp = 600;
        npcdrv::npcTribes[474].coinDropChance = 100;
        npcdrv::npcTribes[474].coinDropBaseCount = 0;
        npcdrv::npcTribes[474].coinDropExtraChance = 60;
        npcdrv::npcTribes[474].coinDropExtraMax = 4;
        npcdrv::npcTribes[474].attackStrength = 3;
        npcdrv::npcTribes[474].bounceEjection = 2;
        npcdrv::npcTribes[474].jumpEjection = 0;

        // Green Magi Projectile
        npcdrv::npcTribes[475].catchCardItemId = 353;

        // White Magikoopa
        npcdrv::npcTribes[476].catchCardItemId = 352;
        item_data::itemDataTable[352].tribe = 476;
        item_data::itemDataTable[352].animPoseName = "e_kamek_w";
        item_data::itemDataTable[352].animName = "KMW_Z_1";
        npcdrv::npcTribes[476].catchCardDefense = 20;
        npcdrv::npcTribes[476].maxHp = 18;
        npcdrv::npcTribes[476].killXp = 600;
        npcdrv::npcTribes[476].coinDropChance = 100;
        npcdrv::npcTribes[476].coinDropBaseCount = 0;
        npcdrv::npcTribes[476].coinDropExtraChance = 60;
        npcdrv::npcTribes[476].coinDropExtraMax = 4;
        npcdrv::npcTribes[476].attackStrength = 3;
        npcdrv::npcTribes[476].bounceEjection = 2;
        npcdrv::npcTribes[476].jumpEjection = 0;

        // White Broom Magikoopa
        npcdrv::npcTribes[477].catchCardItemId = 352;
        npcdrv::npcTribes[477].catchCardDefense = 20;
        npcdrv::npcTribes[477].maxHp = 18;
        npcdrv::npcTribes[477].killXp = 600;
        npcdrv::npcTribes[477].coinDropChance = 100;
        npcdrv::npcTribes[477].coinDropBaseCount = 0;
        npcdrv::npcTribes[477].coinDropExtraChance = 60;
        npcdrv::npcTribes[477].coinDropExtraMax = 4;
        npcdrv::npcTribes[477].attackStrength = 3;
        npcdrv::npcTribes[477].bounceEjection = 2;
        npcdrv::npcTribes[477].jumpEjection = 0;

        // White Magi Projectile
        npcdrv::npcTribes[478].catchCardItemId = 352;

        // Red Magikoopa
        npcdrv::npcTribes[479].catchCardItemId = 351;
        item_data::itemDataTable[351].tribe = 479;
        item_data::itemDataTable[351].animPoseName = "e_kamek_r";
        item_data::itemDataTable[351].animName = "KMR_Z_1";
        npcdrv::npcTribes[479].catchCardDefense = 20;
        npcdrv::npcTribes[479].maxHp = 6;
        npcdrv::npcTribes[479].killXp = 600;
        npcdrv::npcTribes[479].coinDropChance = 100;
        npcdrv::npcTribes[479].coinDropBaseCount = 0;
        npcdrv::npcTribes[479].coinDropExtraChance = 60;
        npcdrv::npcTribes[479].coinDropExtraMax = 4;
        npcdrv::npcTribes[479].attackStrength = 5;
        npcdrv::npcTribes[479].bounceEjection = 2;
        npcdrv::npcTribes[479].jumpEjection = 0;

        // Red Broom Magikoopa
        npcdrv::npcTribes[480].catchCardItemId = 351;
        npcdrv::npcTribes[480].catchCardDefense = 20;
        npcdrv::npcTribes[480].maxHp = 5;
        npcdrv::npcTribes[480].killXp = 600;
        npcdrv::npcTribes[480].coinDropChance = 100;
        npcdrv::npcTribes[480].coinDropBaseCount = 0;
        npcdrv::npcTribes[480].coinDropExtraChance = 60;
        npcdrv::npcTribes[480].coinDropExtraMax = 4;
        npcdrv::npcTribes[480].attackStrength = 5;
        npcdrv::npcTribes[480].bounceEjection = 2;
        npcdrv::npcTribes[480].jumpEjection = 0;

        // Red Magi Projectile, ATK 6
        npcdrv::npcTribes[481].catchCardItemId = 351;

        // Bleepboxer, ATK 3
        npcdrv::npcTribes[504].catchCardItemId = 384;
        item_data::itemDataTable[384].tribe = 504; // Vanilla card data works fine
        npcdrv::npcTribes[504].catchCardDefense = 15;
        npcdrv::npcTribes[504].maxHp = 12;
        npcdrv::npcTribes[504].killXp = 500;
        npcdrv::npcTribes[504].coinDropChance = 100;
        npcdrv::npcTribes[504].coinDropBaseCount = 0;
        npcdrv::npcTribes[504].coinDropExtraChance = 45;
        npcdrv::npcTribes[504].coinDropExtraMax = 3;
        npcdrv::npcTribes[504].attackStrength = 4;
        npcdrv::npcTribes[504].bounceEjection = 2;
        npcdrv::npcTribes[504].jumpEjection = 0;

        // Kilo Muth, ATK 2
        npcdrv::npcTribes[506].catchCardItemId = 427;
        item_data::itemDataTable[427].tribe = 506; // Vanilla card data works fine
        npcdrv::npcTribes[506].catchCardDefense = 15;
        npcdrv::npcTribes[506].maxHp = 25;
        npcdrv::npcTribes[506].killXp = 700;
        npcdrv::npcTribes[506].attackStrength = 2;
        npcdrv::npcTribes[506].bounceEjection = 2;
        npcdrv::npcTribes[506].jumpEjection = 0;

        // Bawbus, ATK 3
        npcdrv::npcTribes[171].maxHp = 15;
        npcdrv::npcTribes[171].killXp = 700;
        npcdrv::npcTribes[171].attackStrength = 3;
        npcdrv::npcTribes[171].catchCardDefense = 12;

        // Mega Muth and Muth HP drop
        npcdrv::npcTribes[257].maxHp = 50;
        npcdrv::npcTribes[258].maxHp = 75;

        // Ice Bro nerf, ATK 3 DEF 2
        npcdrv::npcTribes[61].catchCardDefense = 16;
        item_data::itemDataTable[316].animPoseName = "e_burosu_i";
        item_data::itemDataTable[316].animName = "BRO_Z_1";
        npcdrv::npcTribes[61].maxHp = 10;
        npcdrv::npcTribes[61].killXp = 800;
        npcdrv::npcTribes[61].coinDropChance = 100;
        npcdrv::npcTribes[61].coinDropBaseCount = 0;
        npcdrv::npcTribes[61].coinDropExtraChance = 50;
        npcdrv::npcTribes[61].coinDropExtraMax = 5;
        npcdrv::npcTribes[61].attackStrength = 3;

        // Dark Puff nerf
        npcdrv::npcTribes[111].catchCardDefense = 8;
        npcdrv::npcTribes[111].maxHp = 6;
        npcdrv::npcTribes[111].killXp = 300;
        npcdrv::npcTribes[111].coinDropChance = 100;
        npcdrv::npcTribes[111].coinDropBaseCount = 0;
        npcdrv::npcTribes[111].coinDropExtraChance = 40;
        npcdrv::npcTribes[111].coinDropExtraMax = 2;
        npcdrv::npcTribes[111].attackStrength = 2;

        // Flip Buzzy Beetle/Flip Spike Top patch
        npcdrv::npcTribes[470].catchCardItemId = 298;
        npcdrv::npcTribes[470].catchCardDefense = 2;
        npcdrv::npcTribes[470].maxHp = 1;
        npcdrv::npcTribes[470].killXp = 200;
        npcdrv::npcTribes[470].coinDropChance = 100;
        npcdrv::npcTribes[470].coinDropBaseCount = 0;
        npcdrv::npcTribes[470].coinDropExtraChance = 45;
        npcdrv::npcTribes[470].coinDropExtraMax = 2;
        npcdrv::npcTribes[470].attackStrength = 1;
        npcdrv::npcTribes[470].bounceEjection = 2;
        npcdrv::npcTribes[470].jumpEjection = 0;

        npcdrv::npcTribes[471].catchCardItemId = 299;
        npcdrv::npcTribes[471].catchCardDefense = 10;
        npcdrv::npcTribes[471].maxHp = 1;
        npcdrv::npcTribes[471].killXp = 300;
        npcdrv::npcTribes[471].coinDropChance = 100;
        npcdrv::npcTribes[471].coinDropBaseCount = 0;
        npcdrv::npcTribes[471].coinDropExtraChance = 45;
        npcdrv::npcTribes[471].coinDropExtraMax = 2;
        npcdrv::npcTribes[471].attackStrength = 2;
        npcdrv::npcTribes[471].bounceEjection = 2;
        npcdrv::npcTribes[471].jumpEjection = 0;

        // Green Fuzzy nerf
        npcdrv::npcTribes[98].catchCardDefense = 16;
        item_data::itemDataTable[333].animPoseName = "e_chorobon_g";
        item_data::itemDataTable[333].animName = "CBN_Z_1";
        npcdrv::npcTribes[98].maxHp = 10;
        npcdrv::npcTribes[98].killXp = 600;
        npcdrv::npcTribes[98].coinDropChance = 100;
        npcdrv::npcTribes[98].coinDropBaseCount = 0;
        npcdrv::npcTribes[98].coinDropExtraChance = 40;
        npcdrv::npcTribes[98].coinDropExtraMax = 2;
        npcdrv::npcTribes[98].attackStrength = 2;

        // Tileoid PU stats
        npcdrv::npcTribes[167].catchCardDefense = 12;
        item_data::itemDataTable[402].animPoseName = "e_ntl_p";
        item_data::itemDataTable[402].animName = "Z_1";
        npcdrv::npcTribes[167].maxHp = 1;
        npcdrv::npcTribes[167].killXp = 600;
        npcdrv::npcTribes[167].coinDropChance = 100;
        npcdrv::npcTribes[167].coinDropBaseCount = 0;
        npcdrv::npcTribes[167].coinDropExtraChance = 40;
        npcdrv::npcTribes[167].coinDropExtraMax = 6;
        npcdrv::npcTribes[167].attackStrength = 5;
        npcdrv::npcTribes[167].bounceEjection = 10;
        npcdrv::npcTribes[167].jumpEjection = 10;
        // We're actually gonna nerf the other Tileoids' ejection strengths, too!
        npcdrv::npcTribes[163].bounceEjection = 1.0;
        npcdrv::npcTribes[164].bounceEjection = 1.0;
        npcdrv::npcTribes[165].bounceEjection = 1.0;
        npcdrv::npcTribes[166].bounceEjection = 1.0;
        npcdrv::npcTribes[163].jumpEjection = 1.0;
        npcdrv::npcTribes[164].jumpEjection = 1.0;
        npcdrv::npcTribes[165].jumpEjection = 1.0;
        npcdrv::npcTribes[166].jumpEjection = 1.0;

        // White Clubba nerf
        npcdrv::npcTribes[95].maxHp = 30;
        npcdrv::npcTribes[95].attackStrength = 4;
        item_data::itemDataTable[324].animPoseName = "e_gabow";
        item_data::itemDataTable[324].animName = "gabon_Z_1";

        // Bomb Boo
        item_data::itemDataTable[356].animPoseName = "e_teresa_b";

        // Frackle point buff
        npcdrv::npcTribes[450].killXp = 10;

        // (Spiky) Skellobyte nerfs
        npcdrv::npcTribes[225].maxHp = 20;
        npcdrv::npcTribes[228].maxHp = 20;
        item_data::itemDataTable[438].animPoseName = "e_tesita_bt";
        item_data::itemDataTable[438].animName = "Z_1";
        item_data::itemDataTable[439].animPoseName = "e_tesita_sb";
        item_data::itemDataTable[439].animName = "Z_1";

        // Patch remaining enemies' card textures
        // Red Spike Top
        npcdrv::npcTribes[29].catchCardItemId = 300;
        item_data::itemDataTable[300].tribe = 29;
        item_data::itemDataTable[300].animPoseName = "e_mer";
        item_data::itemDataTable[300].animName = "TMT_Z_1";
        // Hyper Goomba
        item_data::itemDataTable[290].tribe = 2;
        item_data::itemDataTable[290].animPoseName = "e_kuribo_h";
        item_data::itemDataTable[290].animName = "kuribo_Z_1";
        // Hyper Paragoomba
        item_data::itemDataTable[286].tribe = 8;
        item_data::itemDataTable[286].animPoseName = "e_kuribo_h";
        item_data::itemDataTable[286].animName = "pata_kuribo_Z_2";
        // Hyper Spiked Goomba
        item_data::itemDataTable[284].tribe = 6;
        item_data::itemDataTable[284].animPoseName = "e_kuribo_h";
        item_data::itemDataTable[284].animName = "toge_kuribo_Z_2";
        // Sky-Blue Spiny
        item_data::itemDataTable[306].tribe = 38;
        item_data::itemDataTable[306].animPoseName = "e_togezb";
        item_data::itemDataTable[306].animName = "TGZ_Z_1";
        // Dark Koopa
        item_data::itemDataTable[294].tribe = 16;
        item_data::itemDataTable[294].animPoseName = "e_nokoteki_d";
        item_data::itemDataTable[294].animName = "NKT_Z_1";
        // Dark Paratroopa
        item_data::itemDataTable[297].tribe = 24;
        item_data::itemDataTable[297].animPoseName = "e_nokoteki_d";
        item_data::itemDataTable[297].animName = "PTP_Z_1";
        // Dark Paratroopa
        item_data::itemDataTable[297].tribe = 24;
        item_data::itemDataTable[297].animPoseName = "e_nokoteki_d";
        item_data::itemDataTable[297].animName = "PTP_Z_1";
        // Ash Cherbil
        item_data::itemDataTable[535].tribe = 446;
        item_data::itemDataTable[535].animPoseName = "e_cheririn_a";
        item_data::itemDataTable[535].animName = "Z_1";
        // Dark Bones
        item_data::itemDataTable[309].tribe = 43;
        item_data::itemDataTable[309].animPoseName = "e_karon_d";
        item_data::itemDataTable[309].animName = "KRN_Z_1";
        // Shady Magikoopa
        item_data::itemDataTable[318].tribe = 66;
        item_data::itemDataTable[318].animPoseName = "e_kames";
        item_data::itemDataTable[318].animName = "KMK_Z_1";
        // Shady Striker
        item_data::itemDataTable[322].tribe = 75;
        item_data::itemDataTable[322].animPoseName = "e_k_shoote4";
        item_data::itemDataTable[322].animName = "Z_1";
        // Ninjeremiah
        item_data::itemDataTable[434].tribe = 220;
        item_data::itemDataTable[434].animPoseName = "e_nin_d";
        item_data::itemDataTable[434].animName = "Z_1";
        // Dark Koopatrol
        item_data::itemDataTable[293].tribe = 19;
        item_data::itemDataTable[293].animPoseName = "e_togenokd";
        item_data::itemDataTable[293].animName = "Z_1";
        // Kamikaze Goomba
        item_data::itemDataTable[289].tribe = 10;
        item_data::itemDataTable[289].animPoseName = "e_k_kuribo";
        item_data::itemDataTable[289].animName = "z_kuribo_Z_1";
        // Shady Hammer Bro
        item_data::itemDataTable[314].tribe = 49;
        item_data::itemDataTable[314].animPoseName = "e_buross_h";
        item_data::itemDataTable[314].animName = "Z_1";
        // Shady Boomerang Bro
        item_data::itemDataTable[315].tribe = 55;
        item_data::itemDataTable[315].animPoseName = "e_buross_b";
        item_data::itemDataTable[315].animName = "Z_1";
        // Hyper Cleft
        item_data::itemDataTable[364].tribe = 101;
        item_data::itemDataTable[364].animPoseName = "e_sinemoh";
        item_data::itemDataTable[364].animName = "SIN_Z_1";

        // Create Dark Lakitu (Template 30, Tribe 534)
        npcdrv::npcEnemyTemplates[30].tribeId = 534;
        npcdrv::npcEnemyTemplates[30].unknown_0x0 = 1;
        npcdrv::npcEnemyTemplates[30].unknown_0x1 = 1;
        npcdrv::npcEnemyTemplates[30].unknown_0x2 = 0;
        npcdrv::npcEnemyTemplates[30].unknown_0x3 = 0;
        npcdrv::npcEnemyTemplates[30].unknown_0x8 = 0;
        npcdrv::npcEnemyTemplates[30].flags = 0x210;
        npcdrv::npcEnemyTemplates[30].onSpawnScript = npcdrv::npcEnemyTemplates[61].onSpawnScript;
        npcdrv::npcEnemyTemplates[30].initScript = 0;
        npcdrv::npcEnemyTemplates[30].moveScript = npcdrv::npcEnemyTemplates[61].moveScript;
        npcdrv::npcEnemyTemplates[30].onHitScript = npcdrv::npcEnemyTemplates[61].onHitScript;
        npcdrv::npcEnemyTemplates[30].pickupScript = npcdrv::npcEnemyTemplates[61].pickupScript;
        npcdrv::npcEnemyTemplates[30].throwScript = npcdrv::npcEnemyTemplates[61].throwScript;
        npcdrv::npcEnemyTemplates[30].deathScript = npcdrv::npcEnemyTemplates[61].deathScript;
        npcdrv::npcEnemyTemplates[30].atkScript = npcdrv::npcEnemyTemplates[61].atkScript;
        npcdrv::npcEnemyTemplates[30].miscScript = npcdrv::npcEnemyTemplates[61].miscScript;
        npcdrv::npcEnemyTemplates[30].kouraKickScript = npcdrv::npcEnemyTemplates[61].kouraKickScript;
        npcdrv::npcEnemyTemplates[30].unkDefinitionTable = npcdrv::npcEnemyTemplates[61].unkDefinitionTable;
        npcdrv::npcTribes[534].catchCardItemId = 381;
        item_data::itemDataTable[381].tribe = 534;
        item_data::itemDataTable[381].animPoseName = "e_jugemu_d";
        item_data::itemDataTable[381].animName = "JGM_Z_1";
        npcdrv::npcTribes[534].catchCardDefense = 20;
        npcdrv::npcTribes[534].maxHp = 13;
        npcdrv::npcTribes[534].killXp = 900;
        npcdrv::npcTribes[534].coinDropChance = 100;
        npcdrv::npcTribes[534].coinDropBaseCount = 0;
        npcdrv::npcTribes[534].coinDropExtraChance = 68;
        npcdrv::npcTribes[534].coinDropExtraMax = 6;
        npcdrv::npcTribes[534].attackStrength = 3;
        npcdrv::npcTribes[534].animPoseName = "e_jugemu_d";
        npcdrv::npcTribes[534].animDefs = npcdrv::npcTribes[34].animDefs;
        npcdrv::npcTribes[534].hitXp = 0;
        /*
                // Create KP Koopa (Template 7, Tribe 533)
                npcdrv::npcEnemyTemplates[7].tribeId = 533;
                npcdrv::npcEnemyTemplates[7].unknown_0x0 = 1;
                npcdrv::npcEnemyTemplates[7].unknown_0x1 = 1;
                npcdrv::npcEnemyTemplates[7].unknown_0x2 = 0;
                npcdrv::npcEnemyTemplates[7].unknown_0x3 = 0;
                npcdrv::npcEnemyTemplates[7].unknown_0x8 = 0;
                npcdrv::npcEnemyTemplates[7].flags = 0x18;
                npcdrv::npcEnemyTemplates[7].onSpawnScript = npcdrv::npcEnemyTemplates[9].onSpawnScript;
                npcdrv::npcEnemyTemplates[7].initScript = npcdrv::npcEnemyTemplates[9].initScript;
                npcdrv::npcEnemyTemplates[7].moveScript = npcdrv::npcEnemyTemplates[9].moveScript;
                npcdrv::npcEnemyTemplates[7].onHitScript = npcdrv::npcEnemyTemplates[9].onHitScript;
                npcdrv::npcEnemyTemplates[7].pickupScript = npcdrv::npcEnemyTemplates[9].pickupScript;
                npcdrv::npcEnemyTemplates[7].throwScript = npcdrv::npcEnemyTemplates[9].throwScript;
                npcdrv::npcEnemyTemplates[7].deathScript = npcdrv::npcEnemyTemplates[9].deathScript;
                npcdrv::npcEnemyTemplates[7].atkScript = 0;
                npcdrv::npcEnemyTemplates[7].miscScript = 0;
                npcdrv::npcEnemyTemplates[7].kouraKickScript = npcdrv::npcEnemyTemplates[9].kouraKickScript;
                npcdrv::npcEnemyTemplates[7].unkDefinitionTable = npcdrv::npcEnemyTemplates[9].unkDefinitionTable;
                npcdrv::npcTribes[533].catchCardItemId = 291;
                npcdrv::npcTribes[533].catchCardDefense = 8;
                npcdrv::npcTribes[533].maxHp = 5;
                npcdrv::npcTribes[533].killXp = 200;
                npcdrv::npcTribes[533].coinDropChance = 100;
                npcdrv::npcTribes[533].coinDropBaseCount = 0;
                npcdrv::npcTribes[533].coinDropExtraChance = 45;
                npcdrv::npcTribes[533].coinDropExtraMax = 3;
                npcdrv::npcTribes[533].attackStrength = 2;
                npcdrv::npcTribes[533].partsCount = 2;
                npcdrv::npcTribes[533].partsList[0] = npcdrv::npcTribes[14].partsList[0];
                npcdrv::npcTribes[533].partsList[1] = npcdrv::npcTribes[14].partsList[1];
                npcdrv::npcTribes[533].animPoseName = "e_touginoko";
                npcdrv::npcTribes[533].animDefs = npcdrv::npcTribes[14].animDefs;
                npcdrv::npcTribes[533].hitXp = 0;
        */
        // Create Red Shy Guy (Template 32, Tribe 532)
        npcdrv::npcEnemyTemplates[32].tribeId = 532;
        npcdrv::npcEnemyTemplates[32].unknown_0x0 = 1;
        npcdrv::npcEnemyTemplates[32].unknown_0x1 = 1;
        npcdrv::npcEnemyTemplates[32].unknown_0x2 = 0;
        npcdrv::npcEnemyTemplates[32].unknown_0x3 = 0;
        npcdrv::npcEnemyTemplates[32].unknown_0x8 = 0;
        npcdrv::npcEnemyTemplates[32].flags = 0x18;
        npcdrv::npcEnemyTemplates[32].onSpawnScript = npcdrv::npcEnemyTemplates[2].onSpawnScript;
        npcdrv::npcEnemyTemplates[32].initScript = npcdrv::npcEnemyTemplates[2].initScript;
        npcdrv::npcEnemyTemplates[32].moveScript = npcdrv::npcEnemyTemplates[2].moveScript;
        npcdrv::npcEnemyTemplates[32].onHitScript = npcdrv::npcEnemyTemplates[2].onHitScript;
        npcdrv::npcEnemyTemplates[32].pickupScript = npcdrv::npcEnemyTemplates[2].pickupScript;
        npcdrv::npcEnemyTemplates[32].throwScript = npcdrv::npcEnemyTemplates[2].throwScript;
        npcdrv::npcEnemyTemplates[32].deathScript = npcdrv::npcEnemyTemplates[2].deathScript;
        npcdrv::npcEnemyTemplates[32].atkScript = 0;
        npcdrv::npcEnemyTemplates[32].miscScript = 0;
        npcdrv::npcEnemyTemplates[32].kouraKickScript = 0;
        npcdrv::npcEnemyTemplates[32].unkDefinitionTable = npcdrv::npcEnemyTemplates[2].unkDefinitionTable;
        npcdrv::npcTribes[532].catchCardItemId = 330;
        item_data::itemDataTable[330].tribe = 532;
        item_data::itemDataTable[330].animPoseName = "e_heiho";
        item_data::itemDataTable[330].animName = "heiho_Z_1";
        npcdrv::npcTribes[532].catchCardDefense = 3;
        npcdrv::npcTribes[532].maxHp = 5;
        npcdrv::npcTribes[532].killXp = 200;
        npcdrv::npcTribes[532].coinDropChance = 100;
        npcdrv::npcTribes[532].coinDropBaseCount = 0;
        npcdrv::npcTribes[532].coinDropExtraChance = 45;
        npcdrv::npcTribes[532].coinDropExtraMax = 3;
        npcdrv::npcTribes[532].attackStrength = 2;
        npcdrv::npcTribes[532].animPoseName = "e_heiho";
        npcdrv::npcTribes[532].animDefs = heihoAnims;
        npcdrv::npcTribes[532].hitXp = 0;

        // Create Blue Shy Guy (Template 33, Tribe 531)
        npcdrv::npcEnemyTemplates[33].tribeId = 531;
        npcdrv::npcEnemyTemplates[33].unknown_0x0 = 1;
        npcdrv::npcEnemyTemplates[33].unknown_0x1 = 1;
        npcdrv::npcEnemyTemplates[33].unknown_0x2 = 0;
        npcdrv::npcEnemyTemplates[33].unknown_0x3 = 0;
        npcdrv::npcEnemyTemplates[33].unknown_0x8 = 0;
        npcdrv::npcEnemyTemplates[33].flags = 0x18;
        npcdrv::npcEnemyTemplates[33].onSpawnScript = npcdrv::npcEnemyTemplates[2].onSpawnScript;
        npcdrv::npcEnemyTemplates[33].initScript = npcdrv::npcEnemyTemplates[2].initScript;
        npcdrv::npcEnemyTemplates[33].moveScript = npcdrv::npcEnemyTemplates[2].moveScript;
        npcdrv::npcEnemyTemplates[33].onHitScript = npcdrv::npcEnemyTemplates[2].onHitScript;
        npcdrv::npcEnemyTemplates[33].pickupScript = npcdrv::npcEnemyTemplates[2].pickupScript;
        npcdrv::npcEnemyTemplates[33].throwScript = npcdrv::npcEnemyTemplates[2].throwScript;
        npcdrv::npcEnemyTemplates[33].deathScript = npcdrv::npcEnemyTemplates[2].deathScript;
        npcdrv::npcEnemyTemplates[33].atkScript = 0;
        npcdrv::npcEnemyTemplates[33].miscScript = 0;
        npcdrv::npcEnemyTemplates[33].kouraKickScript = 0;
        npcdrv::npcEnemyTemplates[33].unkDefinitionTable = npcdrv::npcEnemyTemplates[2].unkDefinitionTable;
        npcdrv::npcTribes[531].catchCardItemId = 330;
        npcdrv::npcTribes[531].catchCardDefense = 5;
        npcdrv::npcTribes[531].maxHp = 5;
        npcdrv::npcTribes[531].killXp = 300;
        npcdrv::npcTribes[531].coinDropChance = 100;
        npcdrv::npcTribes[531].coinDropBaseCount = 0;
        npcdrv::npcTribes[531].coinDropExtraChance = 45;
        npcdrv::npcTribes[531].coinDropExtraMax = 3;
        npcdrv::npcTribes[531].attackStrength = 2;
        npcdrv::npcTribes[531].animPoseName = "e_beiho";
        npcdrv::npcTribes[531].animDefs = heihoAnims;
        npcdrv::npcTribes[531].hitXp = 0;
        npcdrv::npcTribes[531].bounceEjection = 1;
        npcdrv::npcTribes[531].jumpEjection = 0;
        npcdrv::npcTribes[531].powBlockDeathSfx = "SFX_BS_DDN_CRASH_LANDING1";

        // Create Green Shy Guy (Template 34, Tribe 530)
        npcdrv::npcEnemyTemplates[34].tribeId = 530;
        npcdrv::npcEnemyTemplates[34].unknown_0x0 = 1;
        npcdrv::npcEnemyTemplates[34].unknown_0x1 = 1;
        npcdrv::npcEnemyTemplates[34].unknown_0x2 = 0;
        npcdrv::npcEnemyTemplates[34].unknown_0x3 = 0;
        npcdrv::npcEnemyTemplates[34].unknown_0x8 = 0;
        npcdrv::npcEnemyTemplates[34].flags = 0x18;
        npcdrv::npcEnemyTemplates[34].onSpawnScript = npcdrv::npcEnemyTemplates[2].onSpawnScript;
        npcdrv::npcEnemyTemplates[34].initScript = npcdrv::npcEnemyTemplates[2].initScript;
        npcdrv::npcEnemyTemplates[34].moveScript = npcdrv::npcEnemyTemplates[2].moveScript;
        npcdrv::npcEnemyTemplates[34].onHitScript = npcdrv::npcEnemyTemplates[2].onHitScript;
        npcdrv::npcEnemyTemplates[34].pickupScript = npcdrv::npcEnemyTemplates[2].pickupScript;
        npcdrv::npcEnemyTemplates[34].throwScript = npcdrv::npcEnemyTemplates[2].throwScript;
        npcdrv::npcEnemyTemplates[34].deathScript = npcdrv::npcEnemyTemplates[2].deathScript;
        npcdrv::npcEnemyTemplates[34].atkScript = 0;
        npcdrv::npcEnemyTemplates[34].miscScript = 0;
        npcdrv::npcEnemyTemplates[34].kouraKickScript = 0;
        npcdrv::npcEnemyTemplates[34].unkDefinitionTable = npcdrv::npcEnemyTemplates[2].unkDefinitionTable;
        npcdrv::npcTribes[530].catchCardItemId = 330;
        npcdrv::npcTribes[530].catchCardDefense = 5;
        npcdrv::npcTribes[530].maxHp = 5;
        npcdrv::npcTribes[530].killXp = 300;
        npcdrv::npcTribes[530].coinDropChance = 100;
        npcdrv::npcTribes[530].coinDropBaseCount = 0;
        npcdrv::npcTribes[530].coinDropExtraChance = 45;
        npcdrv::npcTribes[530].coinDropExtraMax = 3;
        npcdrv::npcTribes[530].attackStrength = 2;
        npcdrv::npcTribes[530].animPoseName = "e_geiho";
        npcdrv::npcTribes[530].animDefs = heihoAnims;
        npcdrv::npcTribes[530].hitXp = 0;
        npcdrv::npcTribes[530].bounceEjection = 1;
        npcdrv::npcTribes[530].jumpEjection = 0;

        // Create Yellow Shy Guy (Template 35, Tribe 529)
        npcdrv::npcEnemyTemplates[35].tribeId = 529;
        npcdrv::npcEnemyTemplates[35].unknown_0x0 = 1;
        npcdrv::npcEnemyTemplates[35].unknown_0x1 = 1;
        npcdrv::npcEnemyTemplates[35].unknown_0x2 = 0;
        npcdrv::npcEnemyTemplates[35].unknown_0x3 = 0;
        npcdrv::npcEnemyTemplates[35].unknown_0x8 = 0;
        npcdrv::npcEnemyTemplates[35].flags = 0x18;
        npcdrv::npcEnemyTemplates[35].onSpawnScript = npcdrv::npcEnemyTemplates[2].onSpawnScript;
        npcdrv::npcEnemyTemplates[35].initScript = npcdrv::npcEnemyTemplates[2].initScript;
        npcdrv::npcEnemyTemplates[35].moveScript = npcdrv::npcEnemyTemplates[2].moveScript;
        npcdrv::npcEnemyTemplates[35].onHitScript = npcdrv::npcEnemyTemplates[2].onHitScript;
        npcdrv::npcEnemyTemplates[35].pickupScript = npcdrv::npcEnemyTemplates[2].pickupScript;
        npcdrv::npcEnemyTemplates[35].throwScript = npcdrv::npcEnemyTemplates[2].throwScript;
        npcdrv::npcEnemyTemplates[35].deathScript = npcdrv::npcEnemyTemplates[2].deathScript;
        npcdrv::npcEnemyTemplates[35].atkScript = 0;
        npcdrv::npcEnemyTemplates[35].miscScript = 0;
        npcdrv::npcEnemyTemplates[35].kouraKickScript = 0;
        npcdrv::npcEnemyTemplates[35].unkDefinitionTable = 0;
        npcdrv::npcTribes[529].catchCardItemId = 330;
        npcdrv::npcTribes[529].catchCardDefense = 8;
        npcdrv::npcTribes[529].maxHp = 12;
        npcdrv::npcTribes[529].killXp = 300;
        npcdrv::npcTribes[529].coinDropChance = 100;
        npcdrv::npcTribes[529].coinDropBaseCount = 0;
        npcdrv::npcTribes[529].coinDropExtraChance = 45;
        npcdrv::npcTribes[529].coinDropExtraMax = 3;
        npcdrv::npcTribes[529].attackStrength = 2;
        npcdrv::npcTribes[529].animPoseName = "e_yeiho";
        npcdrv::npcTribes[529].animDefs = heihoAnims;
        npcdrv::npcTribes[529].hitXp = 0;
        npcdrv::npcTribes[529].bounceEjection = 1;
        npcdrv::npcTribes[529].jumpEjection = 0;
        npcdrv::NPCDefense testDefs[5] = {def1_1, def7_1, def0_1, def48, defterm};
        npcdrv::npcTribes[529].partsList[0].defenses = testDefs;
        //    wii::os::OSReport("def0_1: %d.\n", def0_1.defense);
        //    wii::os::OSReport("defterm: %d.\n", defterm.defense);

        // Let's replace the pit key texture while we're at it lol
        // Ty kiki!!! <3
        item_data::itemDataTable[48].iconId = 60;

        // Change some names for the blue whacka bump
        item_data::itemDataTable[45].nameMsg = mystBumpNamePtr;
        item_data::itemDataTable[45].descMsg = mystBumpDescPtr;
    }

    s32 patch_tileoid_pu(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        npcdrv::NPCEntry *npc = evtEntry->ownerNPC;
        if (npc->tribeId == 167)
        {
            npc_tile::TileoidWork *tileWrk = (npc_tile::TileoidWork *)npc->unitWork[0];
            tileWrk->rotationSpeedScale = 3.5;
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(patch_tileoid_pu, 0)

    s32 setHitFlags(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 flagVal = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        npcdrv::NPCEntry *ownerNpc = (npcdrv::NPCEntry *)evtEntry->ownerNPC;
        ownerNpc->parts[0].hitFlags = flagVal;
        ownerNpc->parts[1].hitFlags = flagVal;
        ownerNpc->parts[2].hitFlags = flagVal;
        ownerNpc->parts[3].hitFlags = flagVal;
        ownerNpc->parts[4].hitFlags = flagVal;
        ownerNpc->parts[5].hitFlags = flagVal;
        ownerNpc->parts[6].hitFlags = flagVal;
        ownerNpc->parts[7].hitFlags = flagVal;
        ownerNpc->parts[8].hitFlags = flagVal;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(setHitFlags, 1)

    s32 setFlag2c(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 partId = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 flagVal = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        npcdrv::NPCEntry *ownerNpc = (npcdrv::NPCEntry *)evtEntry->ownerNPC;
        ownerNpc->parts[partId].flag2c = flagVal;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(setFlag2c, 2)

    s32 setHitboxSize(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 partId = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        f32 xVal = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        f32 yVal = evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        f32 zVal = evtmgr_cmd::evtGetValue(evtEntry, args[3]);
        npcdrv::NPCEntry *ownerNpc = (npcdrv::NPCEntry *)evtEntry->ownerNPC;
        ownerNpc->parts[partId].hitBoxScale.x = xVal;
        ownerNpc->parts[partId].hitBoxScale.y = yVal;
        ownerNpc->parts[partId].hitBoxScale.z = zVal;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(setHitboxSize, 4)

    s32 declare_shadoo_stats(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        marioMaxHp = pouch->maxHp;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        npcdrv::NPCEntry *npc = npcdrv::npcNameToPtr_NoAssert(evtmgr_cmd::evtGetValue(evtEntry, args[0]));

        // Shadoo HP = Mario max HP at minimum.
        difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            npc->maxHp = marioMaxHp;
            npc->hp = npc->maxHp;
            shadooHealth = npc->maxHp;
            npcdrv::npcTribes[npc->tribeId].attackStrength = 5;
            break;
        case 1:
            npc->maxHp = marioMaxHp * 2;
            npc->hp = npc->maxHp;
            shadooHealth = npc->maxHp;
            npcdrv::npcTribes[npc->tribeId].attackStrength = 10;
            break;
        case 2:
            npc->maxHp = marioMaxHp * 3;
            npc->hp = npc->maxHp;
            shadooHealth = npc->maxHp;
            npcdrv::npcTribes[npc->tribeId].attackStrength = 15;
            break;
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(declare_shadoo_stats, 1)

    s32 get_mover_rng(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], moverRNG);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(get_mover_rng, 1)

    s32 evt_dan_read_mover_rng(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // This function currently handles rolling for Disorders and sets relevant GSWs.
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 disorderId = swdrv::swByteGet(1630);
        s32 roomsRemaining = swdrv::swByteGet(1631);
        s32 prevDisorderState = 0; // Used to handle post-disorder behavior if a disorder has just ended
        currentFloor = swdrv::swByteGet(1);
        mario_pouch::MarioPouchWork * pouch = mario_pouch::pouchGetPtr();
        if (roomsRemaining > 0)
        {
            prevDisorderState = 1;
            roomsRemaining = roomsRemaining - 1;
            swdrv::swByteSet(1631, roomsRemaining);
        }
        if (roomsRemaining == 0)
        {
            if (prevDisorderState == 1) // Post-Disorder behavior
            {
                s32 currentDisorder = swdrv::swByteGet(1630);
                switch (currentDisorder)
                {
                    case DisorderId::DISORDER_RED:
                    pouch->maxHp = pouch->maxHp + apathyStoredHp;
                    pouch->hp = pouch->hp + apathyStoredHp;
                    break;
                }
            }
            prevDisorderState = 0;
            disorderId = 0;
            swdrv::swByteSet(1631, roomsRemaining);
            swdrv::swByteSet(1630, disorderId);
        }
        s32 currentFloorLastDigit = currentFloor % 10;
        wii::os::OSReport("currentFloorLastDigit: %d\n", currentFloorLastDigit);
        if (disorderId > 0 || roomsRemaining > 0 || currentFloorLastDigit >= 4)
            return 2; // Return if there is an active disorder or if the Floor isn't 1-4
        // Roll through each difficulty to decide whether or not to set a disorder
        s32 moverRNG2 = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 difficulty2 = swdrv::swByteGet(1620);
        if (difficulty2 == 0 && moverRNG2 >= 15 && moverRNG2 < 30)
            goto setDisorder; // EASY DIFFICULTY, 15/1000
        if (difficulty2 == 1 && moverRNG2 >= 15 && moverRNG2 < 55)
            goto setDisorder; // MEDIUM DIFFICULTY, 40/1000
        if (difficulty2 == 2 && moverRNG2 >= 15 && moverRNG2 < 95)
            goto setDisorder; // HARD DIFFICULTY, 80/1000
        return 2;
    setDisorder:
        s32 disorderRNG = system::rand() % 5 + 1;
        switch (disorderRNG)
        {
            case DisorderId::DISORDER_RED:
            switch (difficulty2)
            {
                case 0:
                apathyStoredHp = msl::math::floor(pouch->maxHp * 0.05);
                break;
                case 1:
                apathyStoredHp = msl::math::floor(pouch->maxHp * 0.1);
                break;
                case 2:
                apathyStoredHp = msl::math::floor(pouch->maxHp * 0.15);
                break;
            }
            pouch->hp = pouch->hp - apathyStoredHp;
            pouch->maxHp = pouch->maxHp - apathyStoredHp;
            break;
        }
        swdrv::swByteSet(1631, 5);
        swdrv::swByteSet(1630, disorderRNG);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_dan_read_mover_rng, 1)

    s32 rand100(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        rand100Num = system::rand() % 100;
        wii::os::OSReport("rand100Num: %d.\n", rand100Num);
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], rand100Num);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(rand100, 1)

    s32 osReportFloat(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // arg1 is like PTR("Variable name:"), arg2 is actual value to pass through
        // You can also just use one variable if you only want to pass a ptr string or float on its own
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        char *introStr = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        f32 printFloat = evtmgr_cmd::evtGetFloat(evtEntry, args[1]);
        if (introStr == nullptr || introStr == 0)
        {
            wii::os::OSReport("%f\n", printFloat);
        }
        else
        {
            wii::os::OSReport("%s %f\n", introStr, printFloat);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(osReportFloat, 2)

    s32 osReportOther(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // arg1 is like PTR("Variable name:"), arg2 is actual value to pass through
        // You can also just use one variable if you only want to pass a ptr string or float on its own
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        char *introStr = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        f32 printFloat = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        if (introStr == nullptr || introStr == 0)
        {
            wii::os::OSReport("%f\n", printFloat);
        }
        else
        {
            wii::os::OSReport("%s %f\n", introStr, printFloat);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(osReportOther, 2)

    s32 evt_mario_motion_chg_mot(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        motId = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        mario_motion::marioChgMot(motId);
        mario::MarioWork *mario = mario::marioGetPtr();
        mario->flags = mario->flags &= ~(0x10000000);
        mario->invincibilityTimer = 60;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_mario_motion_chg_mot, 1)

    s32 evt_mario_chk_key(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        s32 marioKey = mario::marioChkKey(); // "is mario allowed to move?" returns true if so
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], marioKey);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_mario_chk_key, 1)

    s32 evtCompareStrings(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        char *mapName = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        char *comparison = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        s32 compstrReturn = 1;
        char *result = msl::string::strstr(mapName, comparison);
        if (result != 0)
        {
            compstrReturn = 1;
        }
        else
        {
            compstrReturn = 0;
        }
        evtmgr_cmd::evtSetValue(evtEntry, args[2], compstrReturn);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evtCompareStrings, 3)

    s32 migraineShortenCurseTimers(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        mario::MarioWork *mario = mario::marioGetPtr();
        for (mario::MarioStatus *yes = mario->firstStatus; yes != 0; yes = yes->next)
        {
            if (yes->type == STATUS_FLIPPED_CONTROLS || yes->type == STATUS_NO_SKILLS || yes->type == STATUS_NO_JUMP || yes->type == STATUS_SLOW)
            {
                // WAIT_MSEC(500) precedes this user func, so it starts with .5s
                // Thus, status conditions may last anywhere from 1-10s
                s32 yesRand = system::rand() % 540 + 30;
                yes->timer = (f32)yesRand;
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(migraineShortenCurseTimers, 0)

    s32 evt_eff_spm_spindash(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        f32 x = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        f32 y = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        f32 z = evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        f32 scale = evtmgr_cmd::evtGetValue(evtEntry, args[3]);
        f32 uwVal = evtmgr_cmd::evtGetValue(evtEntry, args[4]);
        eff_spm_spindash::effSpmSpindashEntry(x, y, z, scale, uwVal);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_eff_spm_spindash, 5)

    s32 spectreFloor(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        double hp = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 floor = swdrv::swByteGet(1);
        s32 hpReturn = 0;
        if (hp <= 1)
        {
            hpReturn = 0;
            evtmgr_cmd::evtSetValue(evtEntry, args[1], hpReturn);
            return 2;
        }
        if (floor <= 24) // Lv1
        {
            hp = (hp / 10);
        }
        else if (floor <= 48) // Lv2
        {
            hp = (hp / 6.667);
        }
        else if (floor <= 174) // Lv3
        {
            hp = (hp / 5);
        }
        else // Lv4
        {
            hp = (hp / 4);
        }
        hpReturn = msl::math::floor(hp);
        if (hpReturn == 0)
        {
            hpReturn = 1;
        }
        evtmgr_cmd::evtSetValue(evtEntry, args[1], hpReturn);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(spectreFloor, 2)

    s32 paramitaFloor(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        if (pouch->hp >= 1)
        {
            f32 hp = (f32)(pouch->hp * 0.2);
            s32 hpReturn = msl::math::floor(hp);
            evtmgr_cmd::evtSetValue(evtEntry, args[0], hpReturn);
        }
        else
        {
            evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(paramitaFloor, 1)

    s32 generate_mover_prices(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // Change Mover prices based on Pit Level
        s32 floor = swdrv::swByteGet(1);
        if (floor <= 24) // Lv1
        {
            moverDown2Price = 20;
            moverDown5Price = 60;
        }
        else if (floor <= 48) // Lv2
        {
            moverDown2Price = 25;
            moverDown5Price = 75;
        }
        else if (floor <= 174) // Lv3
        {
            moverDown2Price = 30;
            moverDown5Price = 90;
        }
        else // Lv4
        {
            moverDown2Price = 40;
            moverDown5Price = 120;
        }
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], moverDown2Price);
        evtmgr_cmd::evtSetValue(evtEntry, args[1], moverDown5Price);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(generate_mover_prices, 2)

    s32 mover_down_2(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        s32 floor = swdrv::swByteGet(1);
        floor = floor + 1;
        swdrv::swByteSet(1, floor);
        const char *destMap = getNextDanMapnameNew(floor);
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], destMap);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(mover_down_2, 1)

    s32 mover_down_5(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        s32 floor = swdrv::swByteGet(1);
        floor = floor + 4;
        // floor = floor + 198; // DEBUG
        swdrv::swByteSet(1, floor);
        const char *destMap = getNextDanMapnameNew(floor);
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], destMap);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(mover_down_5, 1)

    s32 start_from_21(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        mario_pouch::pouchSetMaxHp(20);
        mario_pouch::pouchSetHp(20);
        mario_pouch::pouchAddXp(60000);
        mario_pouch::pouchSetLevel(5);
        mario_pouch::pouchSetAttack(3);
        mario_pouch::pouchSetCoin(40);
        mario_pouch::pouchAddItem(50);
        mario_pouch::pouchAddItem(217);
        mario_pouch::pouchAddItem(221);
        mario_pouch::pouchAddItem(222);
        mario_pouch::pouchAddItem(80);
        mario_pouch::pouchAddItem(80);
        mario_pouch::pouchAddItem(70);
        mario_pouch::pouchAddItem(118);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(start_from_21, 0)

    s32 start_from_61(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        mario_pouch::pouchSetMaxHp(30);
        mario_pouch::pouchSetHp(30);
        mario_pouch::pouchAddXp(250000);
        mario_pouch::pouchSetLevel(9);
        mario_pouch::pouchSetAttack(4);
        mario_pouch::pouchSetCoin(120);
        mario_pouch::pouchAddItem(36);
        mario_pouch::pouchAddItem(50);
        mario_pouch::pouchAddItem(217);
        mario_pouch::pouchAddItem(218);
        mario_pouch::pouchAddItem(221);
        mario_pouch::pouchAddItem(222);
        mario_pouch::pouchAddItem(223);
        mario_pouch::pouchAddItem(224);
        mario_pouch::pouchAddItem(225);
        mario_pouch::pouchAddItem(226);
        mario_pouch::pouchAddItem(227);
        mario_pouch::pouchAddItem(228);
        mario_pouch::pouchAddItem(80);
        mario_pouch::pouchAddItem(118);
        mario_pouch::pouchAddItem(102);
        swdrv::swSet(515);
        swdrv::swSet(511);
        swdrv::swSet(514);
        swdrv::swSet(513);
        swdrv::swSet(510);
        swdrv::swSet(508);
        swdrv::swSet(534);
        swdrv::swSet(1037);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(start_from_61, 0)

    s32 start_from_eg(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        mario_pouch::MarioPouchWork *pouch = mario_pouch::pouchGetPtr();
        mario_pouch::pouchSetMaxHp(40);
        mario_pouch::pouchSetHp(40);
        mario_pouch::pouchAddXp(360000);
        mario_pouch::pouchSetLevel(12);
        mario_pouch::pouchSetAttack(6);
        mario_pouch::pouchSetCoin(200);
        mario_pouch::pouchAddItem(36);
        mario_pouch::pouchAddItem(50);
        mario_pouch::pouchAddItem(217);
        mario_pouch::pouchAddItem(218);
        mario_pouch::pouchAddItem(219);
        mario_pouch::pouchAddItem(221);
        mario_pouch::pouchAddItem(222);
        mario_pouch::pouchAddItem(223);
        mario_pouch::pouchAddItem(224);
        mario_pouch::pouchAddItem(225);
        mario_pouch::pouchAddItem(226);
        mario_pouch::pouchAddItem(227);
        mario_pouch::pouchAddItem(228);
        mario_pouch::pouchAddItem(229);
        mario_pouch::pouchAddItem(230);
        mario_pouch::pouchAddItem(81);
        mario_pouch::pouchAddItem(118);
        swdrv::swSet(515);
        swdrv::swSet(511);
        swdrv::swSet(514);
        swdrv::swSet(513);
        swdrv::swSet(510);
        swdrv::swSet(508);
        swdrv::swSet(418);
        swdrv::swSet(533);
        swdrv::swSet(534);
        swdrv::swSet(535);
        swdrv::swSet(536);
        swdrv::swSet(1037);
        swdrv::swSet(1234);
        swdrv::swSet(1235);
        swdrv::swSet(1236);
        swdrv::swSet(1237);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(start_from_eg, 0)

    s32 evt_npc_drop_item_new(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 itemType = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 coinCount = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        // If Disorder: Dread is active, disable all item and coin drops except for the Pit Key.
        disorderNum = swdrv::swByteGet(1630);
        if (disorderNum == DisorderId::DISORDER_ORANGE && itemType != 48)
        {
            return 2;
        }
        npcdrv::NPCEntry *npc = evt_npc::evtNpcNameToPtr(evtEntry, "me");
        if (npc->unkShellSfx == "holo")
        {
            coinCount = coinCount * 5;
            if (coinCount < 10)
            {
                coinCount = 10;
            }
            else if (coinCount > 30)
            {
                coinCount = 30;
            }
        }
        // If Disorder: Prejudice is active, multiply coinCount and have a chance to set a Gold Bar x3 as the itemType.
        if (disorderNum == DisorderId::DISORDER_YELLOW)
        {
            difficulty = swdrv::swByteGet(1620);
            switch (difficulty)
            {
                case 0:
                coinCount = coinCount * 2.5;
                break;
                case 1:
                coinCount = coinCount * 2;
                break;
                case 2:
                coinCount = coinCount * 1.5;
                break;
            }
            if (coinCount < 5)
            {
                coinCount = 5;
            }
            s32 goldBarChance = system::rand() % 100;
            if (goldBarChance < 7 && itemType == 0)
            {
                itemType = item_data::ItemType::ITEM_ID_USE_KINKAI_100;
            }
        }
        temp_unk::npcDropItem(npc, itemType, coinCount);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_npc_drop_item_new, 2)

    s32 create_holographic_enemy(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[1], 0);
        npcdrv::NPCEntry *npc = npcdrv::npcNameToPtr_NoAssert(evtmgr_cmd::evtGetValue(evtEntry, args[0]));
        s32 sup = system::rand() % 100;
        currentFloor = swdrv::swByteGet(1);
        s32 tribe = npc->tribeId;
        npcdrv::NPCDropItem *dropItems = npcdrv::npcTribes[tribe].dropItemList;
        // While we're here, let's nerf all item drops!
        difficulty = swdrv::swByteGet(1620);
        switch (difficulty)
        {
        case 0:
            if (sup > 80 && npc->dropItemId != 48)
            {
                npc->dropItemId = 0;
            }
            break;
        case 1:
            if (sup > 60 && npc->dropItemId != 48)
            {
                npc->dropItemId = 0;
            }
            break;
        case 2:
            if (sup > 10 && npc->dropItemId != 48)
            {
                npc->dropItemId = 0;
            }
            break;
        }
        // Why not, let's also handle some Disorder stuff here!
        s32 disorderId = swdrv::swByteGet(1630);
        switch (disorderId)
        {
            case DisorderId::DISORDER_RED:
            switch (difficulty)
            {
                case 0:
                npc->hp = (u32)msl::math::floor(npc->hp * 0.85);
                npc->maxHp = (u32)msl::math::floor(npc->maxHp * 0.85);
                break;
                case 1:
                npc->hp = (u32)msl::math::floor(npc->hp * 0.75);
                npc->maxHp = (u32)msl::math::floor(npc->maxHp * 0.75);
                break;
                case 2:
                npc->hp = (u32)msl::math::floor(npc->hp * 0.67);
                npc->maxHp = (u32)msl::math::floor(npc->maxHp * 0.67);
                break;
            }
            break;
        }
        sup = system::rand() % 100;
        // OK, now for holo logic
        // DEBUG: sup > -1 && currentFloor > -1 && (npc->maxHp >= 1 || npcdrv::npcTribes[npc->tribeId].attackStrength >= 0)
        // NORMAL: sup > 95 && currentFloor > 149 && (npc->maxHp >= 10 || npcdrv::npcTribes[npc->tribeId].attackStrength >= 3)
        if (sup > 95 && currentFloor > 149 && (npc->maxHp >= 10 || npcdrv::npcTribes[npc->tribeId].attackStrength >= 3))
        {
            if ((s32)npc != 0 && npc->templateUnkScript9 == 0 && npc->tribeId != 200 && npc->tribeId != 201 && npc->tribeId != 32 && npc->tribeId != 142 && npc->tribeId != 144 && npc->tribeId != 146 && npc->tribeId != 504 && npc->tribeId != 156 && npc->tribeId != 157 && npc->tribeId != 188 && npc->tribeId != 189 && npc->tribeId != 184 && npc->tribeId != 185)
            {
                npc->maxHp = (npc->maxHp * 2);
                npc->hp = (npc->hp * 2);
                npc->unkShellSfx = "holo"; // Used as an identifier for holographic enemies in the npcDamageMario and npcHandleHitXp patches
                sup = system::rand() % 100;
                if (sup > 25)
                {
                    if (npc->dropItemId == 0 && dropItems[0].itemId != 0) // If it doesn't already have an item, continue
                    {
                        sup = 1;
                        for (i = 0; sup != 0; ++i)
                        {
                            sup = dropItems[i].itemId;
                        }
                        do
                        {
                            sup = system::rand() % i;
                            npc->dropItemId = dropItems[sup].itemId;
                            if (npc->dropItemId == 0x57)
                            {
                                s32 sup2 = system::rand() % 100;
                                if (sup2 > 25)
                                {
                                    npc->dropItemId = 0x53; // 75% chance to replace Catch Card drops with Dried Shrooms
                                }
                            }
                        } while (dropItems[sup].itemId == 0);
                    }
                }
                evtmgr_cmd::evtSetValue(evtEntry, args[1], 1);
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(create_holographic_enemy, 2)

    s32 loadCustomSelectTex(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        customSelectType = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 icon = 0;
        if (customSelectType == CustomSelects::SELECT_MUSIC)
        {
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_HONOO_SAKURETU].iconId = 0x85;  // Normal Pit Music; Power Plus icon
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KOORI_NO_IBUKI].iconId = 0x84;  // Tater-Tot Tunes; HP Plus icon
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KAMINARI_DOKKAN].iconId = 0x84; // Plazzap; HP Plus icon
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KIRAKIRA_OTOSHI].iconId = 0x87; // No music; Catch Card SP icon
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_POW_BLOCK].iconId = 0x84;       // Jdaster64; HP Plus icon
        }
        if (customSelectType >= CustomSelects::SELECT_FEATURES)
        {
            msgdrv::msgdrv_msgIcon[3].iconid = 0x86;
            msgdrv::msgdrv_msgIcon[4].iconid = 0x87;
        }
        if (customSelectType == CustomSelects::SELECT_FEATURES)
        {
            icon = (0x86 + swdrv::swGet(1610));
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_HONOO_SAKURETU].iconId = icon; // Movers
            icon = (0x86 + swdrv::swGet(1611));
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KOORI_NO_IBUKI].iconId = icon; // Merluna
            icon = (0x86 + swdrv::swGet(1612));
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KAMINARI_DOKKAN].iconId = icon; // Mario
        }
        if (customSelectType == CustomSelects::SELECT_PATCHES)
        {
            icon = (0x86 + swdrv::swGet(1620));
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_HONOO_SAKURETU].iconId = icon; // Quicker Locks
            icon = (0x86 + swdrv::swGet(1621));
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KOORI_NO_IBUKI].iconId = icon; // Quicker Locks
        }
        if (customSelectType == CustomSelects::SELECT_ACCESSIBILITY)
        {
            icon = (0x86 + swdrv::swGet(1630));
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_HONOO_SAKURETU].iconId = icon; // Explosions Shake Camera
        }
        if (customSelectType == 0)
        {
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_HONOO_SAKURETU].iconId = 0x6C;
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KOORI_NO_IBUKI].iconId = 0x6D;
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KAMINARI_DOKKAN].iconId = 0x6E;
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_KIRAKIRA_OTOSHI].iconId = 0x6F;
            item_data::itemDataTable[item_data::ItemType::ITEM_ID_USE_POW_BLOCK].iconId = 0x70;
            msgdrv::msgdrv_msgIcon[3].iconid = 0xF;
            msgdrv::msgdrv_msgIcon[4].iconid = 0x10;
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(loadCustomSelectTex, 1)

    s32 dan_boodin_setup_cards(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        s32 cardNum = system::rand() % 9 + 5; // 5-13
        boodinBalls.cardCount = cardNum;
        s32 poolCard = 0;
        for (i = 1; i <= cardNum; ++i)
        {
            s32 size = (sizeof(boodinShopItemPool) / 8);
            poolCard = system::rand() % size;
            boodinBalls.cardArray[i] = boodinShopItemPool[poolCard];
            if (i == cardNum)
            {
                boodinBalls.cardArray[i] = -1;
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(dan_boodin_setup_cards, 0)

    s32 dan_boodin_card_select(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // Check if there are any cards remaining
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        // Format array to eliminate zeroes before opening the shop
        for (i = 1; i <= 14; ++i)
        {
            finalCardArray[i] = -1;
        }
        s32 n = 1;
        for (i = 1; i <= 14; ++i)
        {
            if (boodinBalls.cardArray[i] != 0)
            {
                finalCardArray[n] = boodinBalls.cardArray[i];
                n = n + 1;
            }
        }
        // Open shop
        if (firstRun)
        {
            spmario_snd::spsndSFXOn("SFX_SYS_MENU_OPEN1");
            winmgr::WinmgrSelect *shop = winmgr::winMgrSelectEntry(&finalCardArray, 0, 9);
            evtEntry->tempU[0] = (u32)shop;
        }
        s32 selectedCard = winmgr::winMgrSelect((winmgr::WinmgrSelect *)evtEntry->tempU[0]);
        s32 ret = 0;
        if (selectedCard == 0)
        {
            ret = 0;
        }
        else
        {
            winmgr::WinmgrSelect *shop = (winmgr::WinmgrSelect *)evtEntry->tempU[0];
            finalCardSelectedIdx = shop->selectedItemIdx;
            winmgr::winMgrSelectDelete((winmgr::WinmgrSelect *)evtEntry->tempU[0]);
            if (selectedCard == -1)
            {
                evtmgr_cmd::evtSetValue(evtEntry, args[0], -1);
            }
            else
            {
                evtmgr_cmd::evtSetValue(evtEntry, args[0], selectedCard);
                evtmgr_cmd::evtSetValue(evtEntry, args[1], msgdrv::msgSearch(item_data::itemDataTable[selectedCard].nameMsg));
                evtmgr_cmd::evtSetValue(evtEntry, args[2], (u32)item_data::itemDataTable[selectedCard].buyPrice);
                u32 halvedCardPrice = 0;
                if (((u32)item_data::itemDataTable[selectedCard].buyPrice) % 2 == 1) // Variable is odd, so round up.
                {
                    halvedCardPrice = ((f32)item_data::itemDataTable[selectedCard].buyPrice / 2) + 0.5;
                }
                else
                {
                    halvedCardPrice = (u32)item_data::itemDataTable[selectedCard].buyPrice / 2;
                }
                evtmgr_cmd::evtSetValue(evtEntry, args[3], halvedCardPrice);
            }
            ret = 2;
        }
        return ret;
    }
    EVT_DECLARE_USER_FUNC(dan_boodin_card_select, 4) // Return item ID, item name, and its original/new sell value

    s32 dan_boodin_card_remove(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        if (finalCardSelectedIdx != 0)
        {
            finalCardArray[finalCardSelectedIdx] = 0;
            s32 n = 1;
            for (i = 1; i <= 14; ++i)
            {
                if (finalCardArray[i] != 0)
                {
                    boodinBalls.cardArray[n] = finalCardArray[i];
                    n = n + 1;
                }
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(dan_boodin_card_remove, 0)

    s32 chestBakudaaaan(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
        difficulty = swdrv::swByteGet(1620);
        s32 explosionChance = system::rand() % 100;
        s32 explodeDmg = 0;
        switch (difficulty)
        {
        case 0:
            if (explosionChance < 10)
            {
                explodeDmg = (system::rand() % 3) + 4; // 4-6
            }
            break;
        case 1:
            if (explosionChance < 20)
            {
                explodeDmg = (system::rand() % 3) + 6; // 6-8
            }
            break;
        case 2:
            if (explosionChance < 33)
            {
                explodeDmg = (system::rand() % 3) + 8; // 8-10
            }
            break;
        }
        if (explodeDmg != 0)
        {
            evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
            mobjdrv::MobjEntry *chest = mobjdrv::mobjNameToPtr("box");
            mario::marioKeyOn();
            npcdrv::npcDamageMario(0, 0, &chest->pos, 0, explodeDmg, 4);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(chestBakudaaaan, 1)

    s32 merlunaCutsceneSetMaterialFlags(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        mario::MarioWork *mario = mario::marioGetPtr();
        animdrv::animPoseSetMaterialFlagOn(mario->animPoseIds[0], 0x40000000);
        animdrv::animPoseSetMaterialFlagOn(npcdrv::npcNameToPtr("rebear")->m_Anim.m_nPoseId, 0x40000000);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(merlunaCutsceneSetMaterialFlags, 0)

    s32 render_judgement(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        // Blessing
        s32 blessing = system::rand() % 3 + 1;
        blessStorage = blessing;
        char *blessName;
        switch (blessing)
        {
        case BlessId::MERLUNA_SPECTRE:
            blessName = "Spectre";
            break;
        case BlessId::MERLUNA_HOURAI:
            blessName = "Hourai Doll";
            break;
        case BlessId::MERLUNA_PARAMITA:
            blessName = "Paramita";
            break;
        }
        // Curse
        s32 curse = system::rand() % 3 + 1;
        curseStorage = curse;
        char *curseName;
        switch (curse)
        {
        case CurseId::MERLUNA_SHION:
            curseName = "Tatarian Aster";
            break;

        case CurseId::MERLUNA_HEX:
            curseName = "Vulnerability Hex";
            break;

        case CurseId::MERLUNA_MIGRAINE:
            curseName = "Migraine";
            break;
        }

        // Determine which one actually happens
        difficulty = swdrv::swByteGet(1620);
        s32 blessThreshold = 0;
        switch (difficulty)
        {
        case 0:
            blessThreshold = 80;
            break;
        case 1:
            blessThreshold = 70;
            break;
        case 2:
            blessThreshold = 50;
            break;
        }
        s32 judgement = system::rand() % 100;
        if (judgement < blessThreshold)
        {
            judgement = 0; // Is Blessing
        }
        else
        {
            judgement = 1; // Is Curse
        }
        //    wii::os::OSReport("merluna bless name: %s.\n", blessName);
        //    wii::os::OSReport("merluna curse name: %s.\n", curseName);
        evtmgr_cmd::evtSetValue(evtEntry, args[0], blessing);
        evtmgr_cmd::evtSetValue(evtEntry, args[1], blessName);
        evtmgr_cmd::evtSetValue(evtEntry, args[2], curse);
        evtmgr_cmd::evtSetValue(evtEntry, args[3], curseName);
        evtmgr_cmd::evtSetValue(evtEntry, args[4], judgement);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(render_judgement, 5)

    s32 rerender_judgement(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        // Blessing
        char *blessName;
        switch (blessStorage)
        {
        case BlessId::MERLUNA_SPECTRE:
            blessName = "Spectre";
            break;
        case BlessId::MERLUNA_HOURAI:
            blessName = "Hourai Doll";
            break;
        case BlessId::MERLUNA_PARAMITA:
            blessName = "Paramita";
            break;
        }
        // Curse
        char *curseName;
        switch (curseStorage)
        {
        case CurseId::MERLUNA_SHION:
            curseName = "Tatarian Aster";
            break;

        case CurseId::MERLUNA_HEX:
            curseName = "Vulnerability Hex";
            break;

        case CurseId::MERLUNA_MIGRAINE:
            curseName = "Migraine";
            break;
        }

        // Determine which one actually happens
        difficulty = swdrv::swByteGet(1620);
        s32 blessThreshold = 0;
        switch (difficulty)
        {
        case 0:
            blessThreshold = 80;
            break;
        case 1:
            blessThreshold = 70;
            break;
        case 2:
            blessThreshold = 50;
            break;
        }
        s32 judgement = system::rand() % 100;
        if (judgement < blessThreshold)
        {
            judgement = 0; // Is Blessing
        }
        else
        {
            judgement = 1; // Is Curse
        }
        //   wii::os::OSReport("RERENDER merluna bless name: %s.\n", blessName);
        //   wii::os::OSReport("RERENDER merluna curse name: %s.\n", curseName);
        //   wii::os::OSReport("RERENDER merluna judgement: %d.\n", judgement);
        evtmgr_cmd::evtSetValue(evtEntry, args[0], blessStorage);
        evtmgr_cmd::evtSetValue(evtEntry, args[1], blessName);
        evtmgr_cmd::evtSetValue(evtEntry, args[2], curseStorage);
        evtmgr_cmd::evtSetValue(evtEntry, args[3], curseName);
        evtmgr_cmd::evtSetValue(evtEntry, args[4], judgement);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(rerender_judgement, 5)

    s32 render_judgement_2(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 bless = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 curse = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        s32 blessingOrCurse = evtmgr_cmd::evtGetValue(evtEntry, args[2]);

        // Set target GSW to the judgement ID
        if (blessingOrCurse == 0)
        {
            swdrv::swByteSet(1600, bless);
            swdrv::swByteSet(1602, 20);
        }
        else
        {
            swdrv::swByteSet(1601, curse);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(render_judgement_2, 3)

    s32 explain_judgement(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 bless = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 curse = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        s32 judgementType = evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        s32 speaker = evtmgr_cmd::evtGetValue(evtEntry, args[3]);

        // Blessing
        if (judgementType == 0)
        {
            switch (bless)
            {
            case BlessId::MERLUNA_SPECTRE:
                return evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaSpectre, 0, speaker);
                break;
            case BlessId::MERLUNA_HOURAI:
                return evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaHourai, 0, speaker);
                break;
            case BlessId::MERLUNA_PARAMITA:
                return evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaParamita, 0, speaker);
                break;
            }
        }
        else // Curse
        {
            switch (curse)
            {
            case CurseId::MERLUNA_SHION:
                return evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaAster, 0, speaker);
                break;
            case CurseId::MERLUNA_HEX:
                return evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaHex, 0, speaker);
                break;
            case CurseId::MERLUNA_MIGRAINE:
                return evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaMigraine, 0, speaker);
                break;
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(explain_judgement, 4)

    static evt_door::DokanDesc new_dan_70_dokan_desc = {
        0, 0, 0, "dokan", "dan_70", "A2D_dokan_1", "A3D_dokan_1", "mac_05", "dokan_1"};

    npcdrv::NPCTribeAnimDef merlunaAnims[] = {
        {0, "n_stg2_mistS_1"},      // Idle
        {1, "n_stg2_mistW_1"},      // Walking
        {2, "n_stg2_mistR_1"},      // Running
        {3, "n_stg2_mistT_1"},      // Talking (Idle)
        {10, "n_stg2_mistM_1a"},    // Raise your glowsticks~
        {11, "n_stg2_mistM_1b"},    // Wave them around~
        {12, "n_stg2_mistM_1c"},    // Bakudaaaan~
        {13, "n_stg2_mistM_1d"},    // Phew!
        {25, "n_stg2_mistA_1"},     // ):<
        {26, "n_stg2_mistA_2"},     // D:<
        {27, "n_stg2_mistO_1"},     // O_O
        {28, "n_stg2_mistTW_1_a"},  // KEK
        {29, "n_stg2_mistTW_1_b"},  // LMAOOOOOO
        {30, "n_stg2_mistTW_1_c"},  // lol
        {31, "n_stg2_mistTOB_1_a"}, // >_<
        {32, "n_stg2_mistTOB_1_b"}, // @_@
        {33, "n_stg2_mistTOB_1_c"}, // Back to normal
        {34, "n_stg2_mistHi_1"},    // :3
        {35, "n_stg2_mistS_2"},     // Idle (demure)
        {36, "n_stg2_mistJ_1_a"},   // Jump
        {37, "n_stg2_mistJ_1_b"},   // Land
        {-1, "n_stg2_mistZ_1"}};

    npcdrv::NPCTribeAnimDef moverAnims[] = {
        {0, "stg2_syuuzin_b_S_1"}, // Idle
        {1, "stg2_syuuzin_b_W_1"}, // Walking
        {2, "stg2_syuuzin_b_R_1"}, // Running
        {3, "stg2_syuuzin_b_T_1"}, // Talking (Idle)
        {-1, "stg2_syuuzin_b_Z_1"}};

    npcdrv::NPCTribeAnimDef boodinAnims[] = {
        {0, "S_1"},  // Idle
        {1, "W_1"},  // Walking
        {2, "R_1"},  // Running
        {3, "T_1"},  // Talking
        {24, "T_2"}, // One bow goodbye
        {25, "T_3"}, // Three small bows goodbye (unused)
        {26, "O_1"}, // WHAT
        {-1, "Z_1"}};

    /* static evt_door::DokanDesc temp_mac_04_2_dokan_desc = {
         0, 0, 0, "dokan", "mac_04", "A2D_dokan_1", "A3D_dokan_1", "dan_70", "dokan_1"}; */

    EVT_BEGIN(handle_dj_misc_behavior)
    // Search for dialogue that needs to be run
    IF_EQUAL(GSWF(1671), 1)
    WAIT_MSEC(300)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(houraiWearOff), 0, 0)
    END_IF()
    USER_FUNC(handleBlessingWearOff)
    RETURN()
    EVT_END()

    EVT_BEGIN(fix_seq_transition)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(10), LW(11), LW(12))
    USER_FUNC(evt_mario::evt_mario_get_height, LW(13))
    DIVF(LW(13), FLOAT(2.0))
    ADDF(LW(11), LW(13))
    USER_FUNC(evt_fade::evt_fade_set_center_pos, LW(10), LW(11), LW(12))
    RETURN()
    EVT_END()

    EVT_BEGIN(mover_speech)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    USER_FUNC(generate_mover_prices, LW(3), LW(4))
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(moverIntro), 0, PTR("me"))
    SWITCH(LW(3))
    CASE_EQUAL(20)
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(moverChoicesLv1))
    CASE_EQUAL(25)
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(moverChoicesLv2))
    CASE_EQUAL(30)
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(moverChoicesLv3))
    CASE_ETC()
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(moverChoicesLv4))
    END_SWITCH()
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 2) // "I'm good, thanks"
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(moverNo), 0, PTR("me"))
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_IF()
    USER_FUNC(evt_pouch::evt_pouch_get_coins, LW(1))
    IF_EQUAL(LW(0), 1)     // Down 5 Floors
    IF_SMALL(LW(1), LW(4)) // Checks if Mario is poor...
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(moverScam), 0, PTR("me"))
    USER_FUNC(evt_mario::evt_mario_key_on)
    ELSE() // ...but if he's not, Proceed tm
    USER_FUNC(evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    MUL(LW(4), -1)
    USER_FUNC(evt_pouch::evt_pouch_add_coins, LW(4))
    USER_FUNC(evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(evt_sub::evt_sub_hud_configure, 2)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(moverYes), 0, PTR("me"))
    USER_FUNC(mover_down_5, LW(2))
    USER_FUNC(evt_pouch::evt_pouch_check_have_item, 48, LW(5)) // Removes Pit Key if Mario has one
    IF_EQUAL(LW(5), 1)
    USER_FUNC(evt_pouch::evt_pouch_remove_item, 48)
    END_IF()
    RUN_CHILD_EVT(fix_seq_transition)
    WAIT_MSEC(500)
    USER_FUNC(evt_seq::evt_seq_set_seq, seqdrv::SEQ_MAPCHANGE, LW(2), PTR(0))
    END_IF()
    END_IF()
    IF_EQUAL(LW(0), 0)     // Down 2 Floors
    IF_SMALL(LW(1), LW(3)) // Checks if Mario is poor...
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(moverScam), 0, PTR("me"))
    USER_FUNC(evt_mario::evt_mario_key_on)
    ELSE() // ...but if he's not, Proceed tm
    USER_FUNC(evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    MUL(LW(3), -1)
    USER_FUNC(evt_pouch::evt_pouch_add_coins, LW(3))
    USER_FUNC(evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(evt_sub::evt_sub_hud_configure, 2)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(moverYes), 0, PTR("me"))
    USER_FUNC(mover_down_2, LW(2))
    USER_FUNC(evt_pouch::evt_pouch_check_have_item, 48, LW(5)) // Removes Pit Key if Mario has one
    IF_EQUAL(LW(5), 1)
    USER_FUNC(evt_pouch::evt_pouch_remove_item, 48)
    END_IF()
    RUN_CHILD_EVT(fix_seq_transition)
    WAIT_MSEC(500)
    USER_FUNC(evt_seq::evt_seq_set_seq, seqdrv::SEQ_MAPCHANGE, LW(2), PTR(0))
    END_IF()
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_mover_speech)
    RUN_EVT(mover_speech)
    RETURN()
    EVT_END()

    // Overwrite Pit music
    EVT_BEGIN(custom_pit_music)
    SWITCH(GSW(1621))
    CASE_EQUAL(0)
    USER_FUNC(evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_MAP_100F"), 500)
    CASE_EQUAL(1)
    USER_FUNC(evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_EVT_RELAXATION1"), 500)
    CASE_EQUAL(2)
    USER_FUNC(evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_MAP_100FSYNTH"), 500)
    CASE_EQUAL(3)
    USER_FUNC(evt_snd::evt_snd_bgmoff, 0)
    CASE_EQUAL(4)
    USER_FUNC(evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_MAP_100FPIANO"), 500)
    END_SWITCH()
    USER_FUNC(evt_snd::evt_snd_set_sfx_reverb_mode, 0)
    RETURN()
    EVT_END()

    EVT_BEGIN(new_shadoo_evt)
    SET(GSW(24), 1) // just in case
    DO(0)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    IF_LARGE(LW(0), -50)
    DO_BREAK()
    END_IF()
    WAIT_FRM(1)
    WHILE()
    USER_FUNC(evt_mario::evt_mario_key_off, 0)
    USER_FUNC(evt_mario::evt_mario_fairy_reset)
    USER_FUNC(evt_cam::evt_cam_look_at_door, 1, 0)
    INLINE_EVT()
    WAIT_MSEC(200)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
    END_INLINE()
    IF_LARGE(GSW(1622), 0) // If you've already fought Shadoo, new dialogue
    IF_LARGE(GSW(1622), GSW(1620))
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(shadooIntroMock), 0, 0)
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(shadooIntroRamp), 0, 0)
    END_IF()
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(shadooIntro), 0, 0)
    END_IF()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("E_3"), 0)
    USER_FUNC(evt_snd::evt_snd_sfxon, PTR("SFX_EVT_QUAKE1L"))
    USER_FUNC(evt_snd::evt_snd_get_last_sfx_id, LW(0))
    USER_FUNC(evt_cam::evt_cam_shake, 5, FLOAT(1.5), FLOAT(1.5), FLOAT(0.0), 2000, 0)
    USER_FUNC(evt_snd::evt_snd_sfxoff, LW(0))
    USER_FUNC(evt_mario::evt_mario_get_character, LW(1))
    IF_EQUAL(LW(1), 1)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1B"), 0)
    ELSE()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    END_IF()
    USER_FUNC(evt_cam::evt_cam3d_evt_zoom_in, 1, 50, 150, 334, 50, 150, -16, 500, 11)
    WAIT_MSEC(500)
    WAIT_MSEC(1000)
    USER_FUNC(evt_snd::evt_snd_bgmon, 0, PTR("BGM_BTL_BOSS_MIDDLE1"))
    USER_FUNC(evt_npc::evt_npc_tribe_agb_async, 286)
    USER_FUNC(evt_npc::evt_npc_entry_from_template, 0, 286, 0, -100, 0, LW(10), EVT_NULLPTR)
    USER_FUNC(evt_npc::evt_npc_set_anim, LW(10), 0, 1)
    USER_FUNC(evt_npc::func_80107c38, LW(10), 0)
    USER_FUNC(evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
    USER_FUNC(evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
    USER_FUNC(declare_shadoo_stats, LW(10))
    USER_FUNC(evt_npc::evt_npc_flip_to, LW(10), 1)
    USER_FUNC(evt_npc::evt_npc_finish_flip_instant, LW(10))
    USER_FUNC(evt_npc::evt_npc_set_position, LW(10), 50, 110, 20)
    USER_FUNC(evt_npc::func_800ff8f8, LW(10), 50, 110, 20)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
    USER_FUNC(evt_npc::evt_npc_flip, LW(10))
    USER_FUNC(evt_npc::evt_npc_wait_flip_finished, LW(10))
    WAIT_MSEC(1000)
    USER_FUNC(evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
    WAIT_MSEC(500)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_P_MARIO_LAND1"), LW(10))
    USER_FUNC(evt_cam::evt_cam3d_evt_zoom_in, 1, 0, 75, 484, 0, 75, -16, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d_player_character, PTR("SFX_P_V_MARIO_BIKKURI1"), PTR("SFX_P_V_PEACH_ATTACK4"), PTR("SFX_P_V_KOOPA_BIKKURI1"), PTR("SFX_P_V_LUIGI_BIKKURI1"))
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("E_5"), 0)
    USER_FUNC(evt_mario::evt_mario_wait_anim)
    USER_FUNC(evt_mario::evt_mario_get_character, LW(0))
    IF_EQUAL(LW(0), 1)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1B"), 0)
    ELSE()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    END_IF()
    WAIT_MSEC(300)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(shadooDoBattle), 0, LW(10))
    USER_FUNC(evt_npc::evt_npc_restart_evt_id, LW(10))
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_mario::evt_mario_set_anim_change_handler, 0)
    USER_FUNC(evt_mario::evt_mario_key_on)
    RUN_CHILD_EVT(dan::dan_shadoo_fight_evt)
    RETURN()
    EVT_END()

    EVT_BEGIN(easyChestRewards)
    USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("me"), 0)
    USER_FUNC(evt_mobj::evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item::evt_item_entry, PTR("i1"), 0xE7, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i1"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i1"))
    ADD(GSW(1622), 1)
    RETURN()
    EVT_END()

    EVT_BEGIN(normalChestRewards)
    USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("me"), 0)
    USER_FUNC(evt_mobj::evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item::evt_item_entry, PTR("i1"), 0x59, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i1"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i1"))
    USER_FUNC(evt_item::evt_item_entry, PTR("i2"), 0x59, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i2"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i2"))
    USER_FUNC(evt_item::evt_item_entry, PTR("i3"), 0x5A, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i3"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i3"))
    USER_FUNC(evt_item::evt_item_entry, PTR("i4"), 0x5A, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i4"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i4"))
    ADD(GSW(1622), 1)
    RETURN()
    EVT_END()

    EVT_BEGIN(hardChestRewards)
    USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("me"), 0)
    USER_FUNC(evt_mobj::evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item::evt_item_entry, PTR("i1"), 0x20A, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i1"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i1"))
    USER_FUNC(evt_item::evt_item_entry, PTR("i2"), 0x20B, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i2"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i2"))
    USER_FUNC(evt_item::evt_item_entry, PTR("i3"), 0x1CA, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i3"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i3"))
    USER_FUNC(evt_item::evt_item_entry, PTR("i4"), 0x209, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i4"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i4"))
    USER_FUNC(evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    INLINE_EVT()
    USER_FUNC(evt_pouch::evt_pouch_get_max_hp, LW(0))
    USER_FUNC(evt_pouch::evt_pouch_set_hp, LW(0))
    END_INLINE()
    USER_FUNC(evt_pouch::evt_pouch_set_coins, 998)
    USER_FUNC(evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(evt_sub::evt_sub_hud_configure, 2)
    WAIT_MSEC(700)
    ADD(GSW(1622), 1)
    RETURN()
    EVT_END()

    EVT_BEGIN(dan_70_rewards_cont)
    USER_FUNC(evt_mario::evt_mario_key_off, 0)
    USER_FUNC(evt_mario::evt_mario_fairy_reset)
    USER_FUNC(evt_cam::evt_cam_look_at_door, 1, 0)
    USER_FUNC(evt_map::evt_mapobj_get_position, PTR("dokan"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_cam::func_800e01f8)
    USER_FUNC(evt_cam::evt_cam3d_evt_zoom_in, 1, LW(0), EVT_NULLPTR, EVT_NULLPTR, LW(0), EVT_NULLPTR, EVT_NULLPTR, 500, 11)
    WAIT_MSEC(500)
    WAIT_MSEC(1000)
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("dokan"), 1)
    USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A2D_dokan"), 1, 1)
    USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A3D_dokan"), 1, 1)
    USER_FUNC(evt_hit::evt_hit_bind_mapobj, PTR("A2D_dokan"), PTR("dokan"))
    USER_FUNC(evt_hit::evt_hit_bind_mapobj, PTR("A3D_dokan"), PTR("dokan"))
    USER_FUNC(evt_map::evt_mapobj_get_position, PTR("dokan"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_MOBJ_DOKAN_BORN1"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, -3000, 0, 1000)
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    DIVF(LW(0), FLOAT(100.0))
    USER_FUNC(evt_map::evt_mapobj_trans, PTR("dokan"), 0, LW(0), 0)
    USER_FUNC(evt_hit::evt_hit_bind_update, PTR("A2D_dokan"))
    USER_FUNC(evt_hit::evt_hit_bind_update, PTR("A3D_dokan"))
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    WAIT_MSEC(1000)
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_door::evt_door_enable_disable_dokan_desc, 1, PTR("dokan"))
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_new_shadoo_evt)
    RUN_EVT(new_shadoo_evt)
    RETURN_FROM_CALL()

    EVT_BEGIN(dan_70_new_rewards)
    WAIT_MSEC(500)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(evt_cam::evt_cam3d_evt_zoom_in, 1, LW(0), 60, 384, LW(0), 60, -16, 500, 11)
    WAIT_MSEC(1000)
    IF_EQUAL(GSW(1622), 0)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
    USER_FUNC(evt_mario::evt_mario_get_character, LW(0))
    SWITCH(LW(0))
    CASE_EQUAL(0)
    USER_FUNC(evt_guide::evt_guide_get_can_search, LW(0))
    IF_EQUAL(LW(0), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tippiDS), 0, PTR("__guide__"))
    END_IF()
    CASE_EQUAL(1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(peachDS), 0, PTR("__mario__"))
    CASE_EQUAL(2)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(bowserDS), 0, PTR("__mario__"))
    CASE_EQUAL(3)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(luigiDS), 0, PTR("__mario__"))
    END_SWITCH()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    ELSE()
    USER_FUNC(evt_guide::evt_guide_get_can_search, LW(0))
    IF_EQUAL(LW(0), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tippiDSAgain), 0, PTR("__guide__"))
    END_IF()
    END_IF()
    IF_EQUAL(GSWF(1600), 1)
    IF_EQUAL(GSWF(1602), 0)
    SET(GSWF(1602), 1)
    WAIT_MSEC(1500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tippiDS_Sus), 0, PTR("__guide__"))
    END_IF()
    END_IF()
    USER_FUNC(evt_snd::evt_snd_bgmoff_f_d, 0, 2000)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_mario::evt_mario_face_coords, 200, 0)
    USER_FUNC(evt_cam::evt_cam3d_evt_zoom_in, 1, 0, 160, 1034, 0, 160, -16, 500, 11)
    WAIT_MSEC(500)
    INLINE_EVT()
    USER_FUNC(evt_snd::evt_snd_bgmon, 2, PTR("BGM_FF_CORRECT1"))
    USER_FUNC(evt_snd::evt_snd_get_bgm_wait_time, 2, LW(0))
    WAIT_MSEC(LW(0))
    USER_FUNC(evt_snd::evt_snd_bgmoff_f_d, 2, 1000)
    RUN_EVT(custom_pit_music)
    END_INLINE()
    SET(LW(1), GSW(1622))
    SET(LW(2), 0)
    IF_LARGE_EQUAL(GSW(1620), 0)
    IF_LARGE(LW(1), 0)
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("easy"), -250, -100, -75, 0, 0, 0, 1)
    USER_FUNC(evt_mobj::evt_mobj_flag_onoff, 1, 0, PTR("easy"), 0x40)
    USER_FUNC(evt_mobj::evt_mobj_flag_onoff, 1, 1, PTR("easy"), 0x10000)
    ELSE()
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("easy"), -250, -100, -75, 0, PTR(easyChestRewards), 0, 0)
    USER_FUNC(evt_mobj::evt_mobj_flag_onoff, 1, 0, PTR("easy"), 0x40)
    USER_FUNC(evt_mobj::evt_mobj_flag_onoff, 1, 1, PTR("easy"), 0x10000)
    END_IF()
    END_IF()
    IF_LARGE_EQUAL(GSW(1620), 1)
    IF_LARGE(LW(1), 1)
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("normal"), 250, -100, -75, 0, 0, 0, 1)
    ELSE()
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("normal"), 250, -100, -75, 0, PTR(normalChestRewards), 0, 0)
    END_IF()
    END_IF()
    IF_LARGE_EQUAL(GSW(1620), 2)
    IF_LARGE(LW(1), 2)
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("hard"), 0, -100, -75, 0, 0, 0, 1)
    ELSE()
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("hard"), 0, -100, -75, 0, PTR(hardChestRewards), 0, 0)
    END_IF()
    END_IF()
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 1000, 0, 2000)
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    DIVF(LW(0), FLOAT(1000.0))
    SETF(LW(2), LW(0))
    MULF(LW(2), FLOAT(-100.0))
    SETF(LW(3), LW(0))
    MULF(LW(3), FLOAT(1440.0))
    IF_LARGE_EQUAL(GSW(1620), 0)
    USER_FUNC(evt_mobj::evt_mobj_get_position, PTR("easy"), LW(5), LW(6), LW(7))
    USER_FUNC(evt_mobj::evt_mobj_set_position, PTR("easy"), LW(5), LW(2), LW(7))
    END_IF()
    IF_LARGE_EQUAL(GSW(1620), 1)
    USER_FUNC(evt_mobj::evt_mobj_get_position, PTR("normal"), LW(5), LW(6), LW(7))
    USER_FUNC(evt_mobj::evt_mobj_set_position, PTR("normal"), LW(5), LW(2), LW(7))
    END_IF()
    IF_LARGE_EQUAL(GSW(1620), 2)
    USER_FUNC(evt_mobj::evt_mobj_get_position, PTR("hard"), LW(5), LW(6), LW(7))
    USER_FUNC(evt_mobj::evt_mobj_set_position, PTR("hard"), LW(5), LW(2), LW(7))
    END_IF()
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    IF_LARGE_EQUAL(GSW(1620), 0)
    USER_FUNC(evt_mobj::evt_mobj_hit_onoff, 1, PTR("easy"))
    END_IF()
    IF_LARGE_EQUAL(GSW(1620), 1)
    USER_FUNC(evt_mobj::evt_mobj_hit_onoff, 1, PTR("normal"))
    END_IF()
    IF_LARGE_EQUAL(GSW(1620), 2)
    USER_FUNC(evt_mobj::evt_mobj_hit_onoff, 1, PTR("hard"))
    END_IF()
    USER_FUNC(evt_mario::evt_mario_set_anim_change_handler, 0)
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 500, 11)
    USER_FUNC(evt_mario::evt_mario_key_on)
    SET(LW(0), GSW(1620))
    ADD(LW(0), 1)
    DO(0)
    IF_SMALL_EQUAL(LW(0), GSW(1622))
    RUN_EVT(dan_70_rewards_cont)
    RETURN()
    END_IF()
    WAIT_FRM(1)
    WHILE()
    RETURN()
    EVT_END()

    EVT_BEGIN(shadoo_defeat_msg)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(shadooDefeated), 0, PTR("me"))
    RUN_EVT(dan_70_new_rewards)
    RETURN_FROM_CALL()

    EVT_BEGIN(merluna_walmart)
    WAIT_FRM(1)
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_rehighlight)
    INLINE_EVT()
    USER_FUNC(evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
    WAIT_MSEC(500)
    RUN_CHILD_EVT(custom_pit_music)
    END_INLINE()
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 128, 255, 1000)
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    USER_FUNC(evt_map::evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_speech_2)
    // Screen darkens on first evt run
    IF_EQUAL(LW(14), 0)
    SET(LW(14), 1)
    USER_FUNC(merlunaCutsceneSetMaterialFlags)
    INLINE_EVT()
    USER_FUNC(evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
    WAIT_MSEC(500)
    USER_FUNC(evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_MINI_KOURA1"), 500)
    END_INLINE()
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 255, 128, 1200)
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    USER_FUNC(evt_map::evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    END_IF()
    // Merluna prepares judgement
    IF_EQUAL(GSWF(1669), 0)
    SET(GSWF(1669), 1)
    USER_FUNC(render_judgement, GSW(1603), LW(2), GSW(1604), LW(4), LW(5))
    WAIT_MSEC(500)
    END_IF()
    USER_FUNC(evt_msg::evt_msg_print_insert, 1, PTR(merlunaJudgement), 0, PTR("me"), LW(2), LW(4))
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(merlunaOptions))
    USER_FUNC(evt_msg::evt_msg_continue)
    SWITCH(LW(0))
    CASE_EQUAL(0) // Chooses to receive Judgement
    USER_FUNC(evt_pouch::evt_pouch_get_coins, LW(10))
    IF_SMALL(LW(10), 10) // Checks if Mario is poor...
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaScam), 0, PTR("me"))
    INLINE_EVT()
    RUN_CHILD_EVT(merluna_rehighlight)
    END_INLINE()
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 1000, 11)
    WAIT_MSEC(1000)
    USER_FUNC(evt_mario::evt_mario_key_on)
    ELSE() // ...but if he's not, Proceed tm
    USER_FUNC(evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    USER_FUNC(evt_pouch::evt_pouch_add_coins, -10)
    USER_FUNC(evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(evt_sub::evt_sub_hud_configure, 2)
    WAIT_MSEC(500)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(13), LW(14), LW(15))
    USER_FUNC(evt_mario::evt_mario_get_height, LW(14))
    DIVF(LW(14), 2)
    ADDF(LW(15), 10)
    SUBF(LW(13), 3)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 12, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(render_judgement_2, GSW(1603), GSW(1604), LW(5))
    IF_EQUAL(LW(5), 0) // Receives Blessing
    USER_FUNC(evt_snd::evt_snd_sfxon_3d_player, PTR("SFX_EVT_MAJINAI_EFFECT1"))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(13), LW(14), LW(15), FLOAT(5.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_EVT_QUIZ_7_SEIKAI1"), LW(13), LW(14), LW(15))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 13, 1)
    WAIT_MSEC(1000)
    INLINE_EVT()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    RUN_CHILD_EVT(merluna_rehighlight)
    END_INLINE()
    WAIT_MSEC(1300)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaBless), 0, PTR("me"))
    ELSE() // Receives Curse
    USER_FUNC(evt_snd::evt_snd_sfxon_3d_player, PTR("SFX_EVT_MAJINAI_EFFECT1"))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 9, LW(13), LW(14), LW(15), FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("D_2"), 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_EVT_QUIZ_FAULT1"), LW(13), LW(14), LW(15))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 13, 1)
    WAIT_MSEC(1000)
    INLINE_EVT()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    RUN_CHILD_EVT(merluna_rehighlight)
    END_INLINE()
    WAIT_MSEC(1300)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaCurse), 0, PTR("me"))
    END_IF()
    SWITCH_BREAK()
    CASE_EQUAL(3) // No thanks!
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaDecline), 0, PTR("me"))
    INLINE_EVT()
    RUN_CHILD_EVT(merluna_rehighlight)
    END_INLINE()
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 1000, 11)
    WAIT_MSEC(1000)
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
    SWITCH_BREAK()
    CASE_EQUAL(1) // About Blessing...
    SET(LW(6), PTR("me"))
    USER_FUNC(explain_judgement, GSW(1603), GSW(1604), 0, LW(6))
    WAIT_MSEC(250)
    RUN_CHILD_EVT(merluna_speech_2)
    RETURN()
    SWITCH_BREAK() // lol
    CASE_EQUAL(2)  // About Curse...
    SET(LW(6), PTR("me"))
    USER_FUNC(explain_judgement, GSW(1603), GSW(1604), 1, LW(6))
    WAIT_MSEC(250)
    RUN_CHILD_EVT(merluna_speech_2)
    RETURN()
    SWITCH_BREAK() // lol
    END_SWITCH()
    // Merluna disappears after judgement is dispensed
    INLINE_EVT()
    SET(GSWF(1669), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("rebear"), 9, PTR(merluna_walmart))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 28, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 29, 1)
    END_INLINE()
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(evt_mario::evt_mario_key_on)
    WAIT_MSEC(500)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_delete, PTR("me"))
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), 398, 0, 0)
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, 398, 0, 0, FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_curse_speech)
    INLINE_EVT()
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 28, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 29, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 30, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    END_INLINE()
    WAIT_MSEC(2000)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaRelieveCurse), 0, PTR("me"))
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(merlunaRelieveOptions))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 0)
    USER_FUNC(evt_pouch::evt_pouch_get_coins, LW(1))
    IF_SMALL(LW(1), 100) // Checks if Mario is poor...
    WAIT_MSEC(800)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaRelieveScam), 0, PTR("me"))
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(evt_mario::evt_mario_key_on)
    ELSE() // ...but if he's not, Proceed tm
    USER_FUNC(evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    USER_FUNC(evt_pouch::evt_pouch_add_coins, -100)
    USER_FUNC(evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(evt_sub::evt_sub_hud_configure, 2)
    // CINEMATIC
    USER_FUNC(merlunaCutsceneSetMaterialFlags)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 255, 128, 1200)
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    USER_FUNC(evt_map::evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    WAIT_MSEC(500)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(13), LW(14), LW(15))
    USER_FUNC(evt_mario::evt_mario_get_height, LW(14))
    DIVF(LW(14), 2)
    ADDF(LW(15), 10)
    SUBF(LW(13), 3)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 12, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d_player, PTR("SFX_EVT_MAJINAI_EFFECT1"))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(13), LW(14), LW(15), FLOAT(5.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("I_2"), 0)
    SET(GSW(1601), 0)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 13, 1)
    WAIT_MSEC(1000)
    INLINE_EVT()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 128, 255, 1000)
    DO(0)
    USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
    USER_FUNC(evt_map::evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    END_INLINE()
    WAIT_MSEC(1300)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaRelieveReal), 0, PTR("me"))
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(merlunaRetryOptions))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 0)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaRetryAccept), 0, PTR("me"))
    SET(LW(14), 0)
    RUN_CHILD_EVT(merluna_speech_2)
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaRetryDecline), 0, PTR("me"))
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_IF()
    END_IF()
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaRelieveDecline), 0, PTR("me"))
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_speech)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    USER_FUNC(evt_cam::evt_cam3d_evt_zoom_in, 1, FLOAT(237), FLOAT(101), FLOAT(644), FLOAT(340), FLOAT(50), FLOAT(200), 1000, 11)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(4), LW(5), LW(6))
    USER_FUNC(evt_mario::evt_mario_walk_to, 350, 0, 800)
    USER_FUNC(evt_mario::evt_mario_face_npc, PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), -1)
    // Merluna first cutscene
    IF_EQUAL(GSWF(1600), 0)
    SET(GSWF(1600), 1)
    INLINE_EVT()
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 28, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 29, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 30, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    END_INLINE()
    WAIT_MSEC(2000)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(merlunaIntro), 0, PTR("me"))
    WAIT_MSEC(250)
    MARIO_SPAWN_QUESTION_MARK()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_10"), 0)
    WAIT_MSEC(1000)
    INLINE_EVT()
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 34, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    END_INLINE()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    WAIT_MSEC(600)
    SWITCH(GSW(1620))
    CASE_EQUAL(0)
    SET(LW(6), 90)
    CASE_EQUAL(1)
    SET(LW(6), 70)
    CASE_EQUAL(2)
    SET(LW(6), 50)
    END_SWITCH()
    SET(LW(7), 100)
    SUB(LW(7), LW(6))
    USER_FUNC(evt_msg::evt_msg_print_insert, 1, PTR(merlunaIntro2), 0, PTR("me"), LW(6), LW(7))
    ELSE() // Not first cutscene
    WAIT_MSEC(1000)
    IF_LARGE(GSW(1601), 0)
    RUN_CHILD_EVT(merluna_curse_speech)
    RETURN()
    ELSE()
    SWITCH(GSW(1620))
    CASE_EQUAL(0)
    SET(LW(6), 90)
    CASE_EQUAL(1)
    SET(LW(6), 70)
    CASE_EQUAL(2)
    SET(LW(6), 50)
    END_SWITCH()
    SET(LW(7), 100)
    SUB(LW(7), LW(6))
    USER_FUNC(evt_msg::evt_msg_print_insert, 1, PTR(merlunaIntro3), 0, PTR("me"), LW(6), LW(7))
    IF_EQUAL(GSWF(1669), 1)
    USER_FUNC(rerender_judgement, LW(1), LW(2), LW(3), LW(4), LW(5))
    END_IF()
    WAIT_MSEC(600)
    END_IF()
    END_IF()
    RUN_CHILD_EVT(merluna_speech_2)
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_merluna_speech)
    RUN_EVT(merluna_speech)
    RETURN()
    EVT_END()

    EVT_BEGIN(boodin_speech)
    IF_EQUAL(LW(7), 0)
    USER_FUNC(evt_mario::evt_mario_key_off, 0)
    SET(LW(6), 0)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("dan_card"), 0, 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(boodinIntro), 0, PTR("dan_card"))
    END_IF()
    USER_FUNC(evt_sub::evt_sub_hud_configure, 0)
    USER_FUNC(dan_boodin_card_select, LW(4), LW(5), LW(1), LW(2)) // Returns item ID, item name, buy price, and buy price div 2
    USER_FUNC(evt_sub::evt_sub_hud_configure, 2)
    IF_NOT_EQUAL(LW(4), -1)
    USER_FUNC(evt_msg::evt_msg_print_insert, 1, PTR(boodinItemSelected), 0, PTR("dan_card"), LW(5), LW(1), LW(2))
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(boodinSelect))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 0)
    USER_FUNC(evt_pouch::evt_pouch_get_coins, LW(3))
    IF_SMALL(LW(3), LW(2))
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(boodinClassism), 0, PTR("dan_card"))
    ELSE()
    USER_FUNC(evt_pouch::evt_pouch_check_free_use_item, LW(3))
    IF_EQUAL(LW(3), 0)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(boodinNoSpace), 0, PTR("dan_card"))
    ELSE()
    USER_FUNC(evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    MUL(LW(2), -1)
    USER_FUNC(evt_pouch::evt_pouch_add_coins, LW(2))
    USER_FUNC(evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(dan_boodin_card_remove)
    USER_FUNC(evt_item::evt_item_entry, PTR("card_item"), LW(4), 0, 0, -1000, 0, 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("card_item"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("card_item"))
    USER_FUNC(evt_sub::evt_sub_hud_configure, 2)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    // BUY ANOTHER?
    SET(LW(6), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(boodinWantMore), 0, PTR("dan_card"))
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(boodinSelect))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 0)
    SET(LW(7), 1)
    RUN_CHILD_EVT(boodin_speech)
    RETURN()
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(boodinSatisfied), 0, PTR("dan_card"))
    END_IF()
    END_IF()
    END_IF()
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(boodinDecline), 0, PTR("dan_card"))
    END_IF()
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(boodinDecline), 0, PTR("dan_card"))
    END_IF()
    USER_FUNC(evt_mario::evt_mario_key_on)
    INLINE_EVT()
    IF_EQUAL(LW(6), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("dan_card"), 25, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("dan_card"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("dan_card"), 0, 1)
    ELSE()
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("dan_card"), 24, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("dan_card"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("dan_card"), 0, 1)
    END_IF()
    END_INLINE()
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_boodin_speech)
    RUN_EVT(boodin_speech)
    RETURN()
    EVT_END()

    s32 evt_dan_patch_dokan(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        dan::dan_chestRoomDokanDesc.destMapName = "mac_05";
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_dan_patch_dokan, 0)

    EVT_BEGIN(new_dan_chest_interact_evt)
    SWITCH(GSW(1620))
    CASE_EQUAL(0)
    SET(LW(2), 10)
    CASE_EQUAL(1)
    SET(LW(2), 20)
    CASE_EQUAL(2)
    SET(LW(2), 33)
    END_SWITCH()
    USER_FUNC(evt_msg::evt_msg_print_insert, 1, PTR(chestText), 0, 0, LW(2))
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(yesNoSelect_YesByDefault))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 1)
    SET(GSWF(1605), 1)
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(new_dan_chest_open_evt)
    USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("box"), 0)
    USER_FUNC(chestBakudaaaan, LW(1))
    IF_EQUAL(LW(1), 1)
    USER_FUNC(evt_mobj::evt_mobj_get_position, PTR("box"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_NINJA_BOMB_FIRE1"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, LW(0), LW(1), LW(2), FLOAT(1.5), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_mobj::evt_mobj_delete, PTR("box"))
    USER_FUNC(evt_npc::evt_npc_unfreeze_all)
    RETURN()
    END_IF()
    USER_FUNC(evt_sub::evt_sub_item_select_menu, 0, PTR(&rfcItems), LW(0), 0)
    IF_SMALL_EQUAL(LW(0), 0) // Select menu cancelled
    USER_FUNC(evt_mobj::evt_mobj_set_anim, PTR("box"), PTR("A_3"))
    USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("box"), 0)
    USER_FUNC(evt_mobj::evt_mobj_delete, PTR("box"))
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), PTR(new_dan_chest_interact_evt), PTR(new_dan_chest_open_evt), 0, 0)
    USER_FUNC(evt_npc::evt_npc_unfreeze_all)
    USER_FUNC(evt_mario::evt_mario_key_on)
    ELSE()
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
    USER_FUNC(evt_item::evt_item_entry, PTR("item"), LW(0), 0, LW(1), LW(2), LW(3), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("item"))
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(whacka_hit)
    USER_FUNC(evt_npc::func_80108194, PTR("dan_koburon"), 0)
    USER_FUNC(evt_npc::evt_npc_get_damage_type, PTR("me"), LW(0))
    SWITCH(LW(0))
    CASE_EQUAL(1)
    CASE_EQUAL(2)
    CASE_EQUAL(10)
    CASE_EQUAL(4)
    CASE_EQUAL(5)
    CASE_EQUAL(17)
    CASE_ETC()
    USER_FUNC(evt_npc::evt_npc_restart_evt_id, PTR("dan_koburon"))
    RETURN()
    END_SWITCH()
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    IF_EQUAL(GSWF(1640), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("me"), 2, 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("me"), 9, 0)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 4, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    ADD(LW(2), 10)
    USER_FUNC(evt_item::evt_item_entry, PTR("bump"), 106, 0, LW(0), LW(1), LW(2), 1, 0, 0, 0)
    WAIT_MSEC(500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaOww), 0, PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    SET(GSWF(1640), 1)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KOBURON_IN1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_koburon"), 1, 0)
    ELSE()
    WAIT_MSEC(500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaCantHurt), 0, PTR("me")) // You can't hurt me, I'm bluuuue!
    WAIT_MSEC(500)
    USER_FUNC(evt_mario::evt_mario_get_character, LW(0))
    USER_FUNC(evt_guide::evt_guide_get_can_search, LW(1))
    SWITCH(LW(0))
    CASE_EQUAL(0)
    IF_EQUAL(LW(1), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tippiHitWhacka), 0, PTR("__guide__")) // I don't think being extra blue makes you invincible...
    END_IF()
    CASE_EQUAL(1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(peachHitWhacka), 0, PTR("__mario__"))
    CASE_EQUAL(2)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(bowserHitWhacka), 0, PTR("__mario__"))
    CASE_EQUAL(3)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(luigiHitWhacka), 0, PTR("__mario__"))
    END_SWITCH()
    IF_EQUAL(LW(1), 1)
    WAIT_MSEC(500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaCantHurt2), 0, PTR("me")) // Oh, really?
    END_IF()
    WAIT_MSEC(200)
    END_IF()
    USER_FUNC(evt_mario::evt_mario_key_on)
    USER_FUNC(evt_npc::evt_npc_restart_evt_id, PTR("dan_koburon"))
    RETURN()
    EVT_END()

    EVT_BEGIN(whacka_move)
    USER_FUNC(evt_npc::func_80108194, PTR("me"), 0)
    USER_FUNC(evt_npc::evt_npc_set_move_mode, PTR("me"), 1)
    DO(0)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    SET(LW(3), 75)
    SET(LW(4), 25)
    SET(LW(5), FLOAT(-87.5))
    USER_FUNC(evt_sub::evt_sub_get_dist, LW(0), 0, LW(3), 0, LW(1))
    IF_LARGE(LW(1), 100)
    IF_EQUAL(GSWF(384), 1)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KOBURON_IN1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
    SET(GSWF(384), 0)
    END_IF()
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), LW(3), -1000, LW(5))
    ELSE()
    IF_EQUAL(GSWF(384), 0)
    IF_EQUAL(GSWF(1640), 1)
    USER_FUNC(evt_npc::evt_npc_set_color, PTR("dan_koburon"), 60, 100, 255, 255)
    END_IF()
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KOBURON_OUT1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 25, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    SET(GSWF(384), 1)
    END_IF()
    END_IF()
    WAIT_FRM(5)
    WHILE()
    RETURN()
    EVT_END()

    EVT_BEGIN(whacka_speech)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    IF_EQUAL(GSWF(1640), 0) // Checks if you've hit Whacka in the Pit
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaHi), 0, PTR("me"))
    ELSE()
    IF_EQUAL(GSWF(1641), 0) // Whacka hasn't given you the Mysterious Bump
    IF_EQUAL(GSWF(1642), 0) // Checks if you've talked to him after hitting him once already
    SET(GSWF(1642), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaHiBlueLong1), 0, PTR("me")) // Long
    USER_FUNC(evt_mario::evt_mario_get_character, LW(0))
    USER_FUNC(evt_guide::evt_guide_get_can_search, LW(1))
    SWITCH(LW(0))
    CASE_EQUAL(0)
    IF_EQUAL(LW(1), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tippiBlueWhacka), 0, PTR("__guide__"))
    END_IF()
    CASE_EQUAL(1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(peachBlueWhacka), 0, PTR("__mario__"))
    CASE_EQUAL(2)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(bowserBlueWhacka), 0, PTR("__mario__"))
    CASE_EQUAL(3)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(luigiBlueWhacka), 0, PTR("__mario__"))
    END_SWITCH()
    IF_EQUAL(LW(1), 1)
    WAIT_MSEC(500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaHiBlueLong2), 0, PTR("me"))
    END_IF()
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaHiBlueShort), 0, PTR("me")) // Short
    END_IF()
    USER_FUNC(evt_pouch::evt_pouch_check_have_item, 106, LW(0)) // Checks if Mario has a Whacka Bump
    IF_EQUAL(LW(0), 1)
    USER_FUNC(evt_sub::evt_sub_item_select_menu, 0, PTR(&whackaItems), LW(0), 0)
    IF_EQUAL(LW(0), 106)
    USER_FUNC(evt_pouch::evt_pouch_remove_item, 106)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaThanks), 0, PTR("me"))
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KOBURON_IN1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 75, -100, -90)
    USER_FUNC(evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
    WAIT_MSEC(3000)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_DOKAN_MAKE1"), PTR("me"))
    WAIT_MSEC(1000)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_10"), 0)
    WAIT_MSEC(1000)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_P_V_PEACH_PANIC1"), PTR("me"))
    WAIT_MSEC(250)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
    WAIT_MSEC(750)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_BS_DMN_FLAME_BOMB1"), PTR("me"))
    WAIT_MSEC(400)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("E_5"), 0)
    USER_FUNC(evt_mario::evt_mario_wait_anim)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_7"), 0)
    WAIT_MSEC(1000)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_BS_DMN_FLAME_BOMB1"), PTR("me"))
    WAIT_MSEC(4000)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KOBURON_OUT1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 25, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    WAIT_MSEC(500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaGiveBump1), 0, PTR("me"))
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(evt_item::evt_item_entry, PTR("i1"), 45, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("i1"), 8)
    USER_FUNC(evt_item::evt_item_wait_collected, PTR("i1"))
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    RUN_EVT(custom_pit_music)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaGiveBump2), 0, PTR("me"))
    SET(GSWF(1641), 1)
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaHmph), 0, PTR("me"))
    END_IF()
    ELSE()
    WAIT_MSEC(1500)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaDontHaveBump), 0, PTR("me"))
    END_IF()
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(whackaHiBluePost), 0, PTR("me"))
    END_IF()
    END_IF()
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
    EVT_END()

    EVT_BEGIN(spawn_whacka)
    SET(GSWF(384), 0)
    USER_FUNC(evt_npc::evt_npc_entry, PTR("dan_koburon"), PTR("n_koburon"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_koburon"), 14, PTR(&temp_unk::whacka_anim_defs))
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_koburon"), 9, PTR(whacka_speech))
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_koburon"), 2, PTR(whacka_hit))
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_koburon"), 1, PTR(whacka_move))
    USER_FUNC(evt_npc::evt_npc_set_move_mode, PTR("dan_koburon"), 1)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("dan_koburon"), 75, -1000, FLOAT(-87.5))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("dan_koburon"), 26, 1)
    USER_FUNC(evt_npc::evt_npc_flag8_onoff, PTR("dan_koburon"), 1, 0x4410004)
    USER_FUNC(evt_npc::evt_npc_animflag_onoff, PTR("dan_koburon"), 1, 0x20)
    USER_FUNC(evt_npc::evt_npc_add_flip_part, PTR("dan_koburon"))
    USER_FUNC(evt_npc::func_80108194, PTR("dan_koburon"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_koburon"), 11, 20)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_koburon"), 10, 20)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_koburon"), 12, 20)
    USER_FUNC(evt_npc::evt_npc_modify_part, PTR("dan_koburon"), -1, 15, PTR(&temp_unk::whacka_defenses))
    USER_FUNC(evt_npc::evt_npc_modify_part, PTR("dan_koburon"), 1, 11, 20)
    USER_FUNC(evt_npc::evt_npc_modify_part, PTR("dan_koburon"), 1, 10, 20)
    USER_FUNC(evt_npc::evt_npc_modify_part, PTR("dan_koburon"), 1, 12, 20)
    USER_FUNC(evt_npc::evt_npc_restart_evt_id, PTR("dan_koburon"))
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_setup)
    // Handle music
    RUN_EVT(custom_pit_music)
    // Overwrite vanilla chests
    USER_FUNC(evt_mobj::evt_mobj_delete, PTR("box"))
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), PTR(new_dan_chest_interact_evt), PTR(new_dan_chest_open_evt), 0, 0)
    // Handle Whacka replacing the chest
    IF_NOT_EQUAL(GSW(22), 8) // If Whacka has not been brutally murdered
    USER_FUNC(evt_sub::evt_sub_random, 100, LW(0))
    IF_SMALL_EQUAL(LW(0), 10)
    USER_FUNC(evt_mobj::evt_mobj_delete, PTR("box"))
    RUN_EVT(spawn_whacka)
    END_IF()
    END_IF()
    // Handle blessing wear-off effects in rest floors
    IF_SMALL(GSW(1602), 1)
    USER_FUNC(handleBlessingWearOff)
    END_IF()
    // Initialize NPC draw logic with Merluna overrides.
    SET(LW(1), 0)          // 0 is no NPC, 1 is Flimm, 2 is Merluna, 3 is Boodin
    SET(LW(2), 0)          // 0 is Merluna-neutral, 1 is forcibly active, 2 is forcibly inactive
    IF_LARGE(GSW(1600), 0) // If ANY blessing is active, Merluna will never show up.
    SET(LW(1), 0)
    SET(LW(2), 2)
    END_IF()
    IF_LARGE(GSW(1601), 0) // If ANY curse is active, Merluna will always show up.
    SET(LW(1), 2)
    SET(LW(2), 1)
    END_IF()
    IF_EQUAL(GSWF(1611), 1) // If Merluna is disabled in the Jimbo Settings, she will never show up.
    SET(LW(1), 0)
    SET(LW(2), 2)
    END_IF()
    // Run normal NPC draws with Merluna overrides in consideration
    USER_FUNC(evt_sub::evt_sub_random, 100, LW(0))
    IF_EQUAL(LW(2), 0) // Assuming neither a blessing or curse is active
    IF_SMALL(LW(0), 40)
    SET(LW(1), 1)
    ELSE()
    IF_SMALL(LW(0), 80)
    SET(LW(1), 2)
    ELSE()
    SET(LW(1), 3)
    END_IF()
    END_IF()
    END_IF()
    IF_EQUAL(LW(2), 2) // Assuming Merluna is forcibly inactive
    IF_SMALL(LW(0), 70)
    SET(LW(1), 1)
    ELSE()
    SET(LW(1), 3)
    END_IF()
    IF_EQUAL(GSW(1620), 2)
    SET(LW(1), 3)
    END_IF()
    END_IF()
    // Handle NPC draws
    IF_EQUAL(LW(1), 1)         // Flimm
    IF_NOT_EQUAL(GSW(1620), 2) // If difficulty is Hard, do not spawn Flimm at all
    USER_FUNC(evt_npc::evt_npc_entry, PTR("roten"), PTR("n_machi_roten"), 0)
    USER_FUNC(evt_npc::evt_npc_set_axis_movement_unit, PTR("roten"), -1)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("roten"), 14, PTR(&dan::dan_rotenTribeAnimDefs))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("roten"), 0, 1)
    USER_FUNC(evt_npc::evt_npc_flag8_onoff, PTR("roten"), 1, 0x4400004)
    USER_FUNC(evt_npc::evt_npc_animflag_onoff, PTR("roten"), 1, 32)
    USER_FUNC(evt_npc::evt_npc_add_flip_part, PTR("roten"))
    USER_FUNC(evt_npc::evt_npc_set_move_mode, PTR("roten"), 1)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("roten"), 398, 0, 0)
    USER_FUNC(evt_npc::func_80108194, PTR("roten"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("roten"), 11, 40)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("roten"), 10, 60)
    USER_FUNC(evt_npc::evt_npc_modify_part, PTR("roten"), -1, 11, 40)
    USER_FUNC(evt_npc::evt_npc_modify_part, PTR("roten"), -1, 10, 60)
    USER_FUNC(evt_shop::evt_shop_set_defs, PTR(&dan::dan_rotenShopDef), 1)
    END_IF()
    END_IF()
    IF_EQUAL(LW(1), 2) // Merluna
    USER_FUNC(evt_npc::evt_npc_entry, PTR("rebear"), PTR("n_rebear"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("rebear"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(merlunaAnims))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("rebear"), 0, true)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("rebear"), 398, 0, 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("rebear"), 9, PTR(fwd_merluna_speech))
    USER_FUNC(evt_npc::evt_npc_add_flip_part, PTR("rebear"))
    END_IF()
    IF_EQUAL(LW(1), 3) // Boodin
    USER_FUNC(evt_npc::evt_npc_entry, PTR("dan_card"), PTR("n_machi_c_card"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_card"), 14, PTR(boodinAnims))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("dan_card"), 0, 1)
    USER_FUNC(evt_npc::evt_npc_flag8_onoff, PTR("dan_card"), 1, 0x4400004)
    USER_FUNC(evt_npc::evt_npc_animflag_onoff, PTR("dan_card"), 1, 32)
    USER_FUNC(evt_npc::evt_npc_add_flip_part, PTR("dan_card"))
    USER_FUNC(evt_npc::evt_npc_set_move_mode, PTR("dan_card"), 1)
    USER_FUNC(evt_npc::func_80108194, PTR("dan_card"), 0)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("dan_card"), 398, 0, 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("dan_card"), 9, PTR(fwd_boodin_speech))
    USER_FUNC(dan_boodin_setup_cards)
    END_IF()
    // Initialize Mover spawn logic, independent of all other NPCs
    USER_FUNC(get_mover_rng, LW(0))
    IF_SMALL_EQUAL(LW(0), 24) // Boosted chance to spawn on rest floors
    USER_FUNC(evt_npc::evt_npc_entry, PTR("mover"), PTR("n_stg2_syuuzin_b"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("mover"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(moverAnims))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("mover"), 0, true)
    USER_FUNC(evt_npc::evt_npc_add_flip_part, PTR("mover"))
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("mover"), 200, 0, 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("mover"), 9, PTR(fwd_mover_speech))
    END_IF()
    USER_FUNC(evt_dan_patch_dokan)
    RETURN_FROM_CALL()

    EVT_BEGIN(determine_custom_music)
    // Difficulty
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(difficultyText), 0, 0)
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(difficultyOptions))
    USER_FUNC(evt_msg::evt_msg_continue)
    SET(GSW(1620), LW(0))
    // Music
    USER_FUNC(loadCustomSelectTex, 1)
    USER_FUNC(evt_sub::evt_sub_item_select_menu, 0, PTR(&musicItems), LW(0), 0)
    SWITCH(LW(0))
    CASE_EQUAL(65) // Vanilla
    SET(GSW(1621), 0)
    CASE_EQUAL(66) // 8Bit
    SET(GSW(1621), 1)
    CASE_EQUAL(67) // Synth
    SET(GSW(1621), 2)
    CASE_EQUAL(68) // No Music
    SET(GSW(1621), 3)
    CASE_EQUAL(69)
    SET(GSW(1621), 4)
    END_SWITCH()
    USER_FUNC(loadCustomSelectTex, 0)
    SET(GW(5), 0) // Fixes the Dark Prognosticus breaking holographic enemies. What a funny ass bug
    RETURN_FROM_CALL()

    EVT_BEGIN(custom_music_sign)
    IF_EQUAL(GSWF(1601), 0)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(musicSignRFText), 0, 0)
    SET(GSWF(1601), 1)
    END_IF()
    USER_FUNC(loadCustomSelectTex, 1)
    USER_FUNC(evt_sub::evt_sub_item_select_menu, 0, PTR(&musicItems), LW(0), 0)
    SWITCH(LW(0))
    CASE_EQUAL(65) // Vanilla
    SET(GSW(1621), 0)
    CASE_EQUAL(66) // 8Bit
    SET(GSW(1621), 1)
    CASE_EQUAL(67) // Synth
    SET(GSW(1621), 2)
    CASE_EQUAL(68) // No Music
    SET(GSW(1621), 3)
    CASE_EQUAL(69) // Piano
    SET(GSW(1621), 4)
    END_SWITCH()
    USER_FUNC(loadCustomSelectTex, 0)
    IF_NOT_EQUAL(LW(0), -1)
    USER_FUNC(evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
    WAIT_MSEC(1200)
    RUN_EVT(custom_pit_music)
    END_IF()
    RETURN_FROM_CALL()

    EVT_BEGIN(spectre_logic)
    IF_EQUAL(GSW(1600), 1)
    USER_FUNC(rand100, LW(1))
    IF_SMALL(LW(1), 20)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_NOT_EQUAL(LW(1), 156)
    IF_NOT_EQUAL(LW(1), 157)
    USER_FUNC(evt_npc::evt_npc_get_max_hp, PTR("me"), LW(1)) // Set to LW(1) after test
    USER_FUNC(spectreFloor, LW(1), LW(2))
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(3), LW(4), LW(5))
    USER_FUNC(evt_mario::evt_mario_get_height, LW(7))
    ADD(LW(4), LW(7))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("spm_recovery"), LW(3), LW(4), LW(5), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_pouch::evt_pouch_add_hp, LW(2))
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    RETURN_FROM_CALL()

    EVT_BEGIN(npc_drop_item_patch)
    USER_FUNC(evt_npc_drop_item_new, LW(10), LW(11))
    RETURN_FROM_CALL()

    s32 danPitLock[] = {48, -1};

    // Like so many other functions used in this mod, this was adapted heavily from decomp dan.c
    // Thank you Seeky! This mod and many others would not exist without your work.
    // You are greatly appreciated by all of us in the SPM Community.
    s32 evt_dan_handle_key_failsafe_new(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)isFirstCall;
        // Check whether the key exists anywhere
        if (
            !dan::danCheckEnemyInMapBbox() && !dan::danCheckKeyInMapBbox() &&
            !mario_pouch::pouchCheckHaveItem(48) &&
            !itemdrv::itemCheckForId(48))
        {
            // Spawn the key at Mario's position if not
            mario::MarioWork *mario = mario::marioGetPtr();
            itemdrv::itemEntry(NULL, 48, 1, mario->position.x, mario->position.y, mario->position.z, NULL, 0);
            return 2;
        }
        else
        {
            return 0;
        }
    }
    EVT_DECLARE_USER_FUNC(evt_dan_handle_key_failsafe_new, 0)

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

    s32 indifferenceItems(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        u8 difficulty2 = swdrv::swByteGet(1620);
        u8 loops = 1;
        if (difficulty2 == 2)
        {
            loops = 2;
        }
        u8 thresh = 0;
        u8 j = 0;
        s32 indiffItems[] = {83, 95, 98, 160, 174, 175, 176, 178};
        const char *indiffINames[] = {"indiff_i1", "indiff_i2"};
        s32 indiffItemIdx = 0;
        u8 itemsRmd = 0;
        u8 itemsAdded = 0;
        while (loops != j)
        {
            switch (difficulty2)
            {
            case 0:
                thresh = 25;
                break;
            case 1:
                thresh = 50;
                break;
            case 2:
                thresh = 50;
                break;
            }
            s32 odds = system::rand() % 100;
            if (thresh > odds)
            {
                indiffItemIdx = system::rand() % 8;
                mario::MarioWork * mario = mario::marioGetPtr();
                if ((mario_pouch::pouchCountUseItems() + itemsAdded) < 10)
                {
                    itemdrv::ItemEntry * item = itemdrv::itemEntry(indiffINames[itemsAdded], indiffItems[indiffItemIdx], 0, mario->position.x, mario->position.y, mario->position.z, NULL, 0);
                    item->flags = (item->flags | 0x800);
                    itemsAdded = itemsAdded + 1;
                }
                else
                {
                    s32 invIdx = system::rand() % 10;
                    mario_pouch::MarioPouchWork * pouch = mario_pouch::pouchGetPtr();
                    itemsRmd = itemsRmd + 1;
                    evtmgr_cmd::evtSetValue(evtEntry, args[itemsRmd], msgdrv::msgSearch(item_data::itemDataTable[pouch->useItem[invIdx]].nameMsg));
                    mario_pouch::pouchRemoveItemIdx(pouch->useItem[invIdx], invIdx);
                    itemdrv::ItemEntry * item = itemdrv::itemEntry(indiffINames[itemsAdded], indiffItems[indiffItemIdx], 0, mario->position.x, mario->position.y, mario->position.z, NULL, 0);
                    item->flags = (item->flags | 0x800);
                    itemsAdded = itemsAdded + 1;
                }
            }
            j = j + 1;
        }
        evtmgr_cmd::evtSetValue(evtEntry, args[0], itemsRmd);
        evtmgr_cmd::evtSetValue(evtEntry, args[3], itemsAdded);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(indifferenceItems, 4)

    EVT_BEGIN(handle_indifference)
    USER_FUNC(evt_mario::evt_mario_key_off, 0)
    USER_FUNC(indifferenceItems, LW(0), LW(1), LW(2), LW(3))
    IF_EQUAL(LW(0), 1)
    USER_FUNC(evt_msg::evt_msg_print_insert, 1, PTR(disorderIndifferenceItemNotif), 0, 0, LW(1))
    ELSE()
    IF_EQUAL(LW(0), 2)
    USER_FUNC(evt_msg::evt_msg_print_insert, 1, PTR(disorderIndifferenceItemNotif2), 0, 0, LW(1), LW(2))
    END_IF()
    END_IF()
    USER_FUNC(evt_mario::evt_mario_key_on)
    IF_LARGE_EQUAL(LW(3), 1)
    USER_FUNC(evt_item::evt_item_flag_onoff, 0, PTR("indiff_i1"), 0x800)
    END_IF()
    IF_EQUAL(LW(3), 2)
    USER_FUNC(evt_item::evt_item_flag_onoff, 0, PTR("indiff_i2"), 0x800)
    END_IF()
    RETURN()
    EVT_END()

    s32 IntplUltra(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 x1 = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 y1 = evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        s32 x2 = evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        s32 y2 = evtmgr_cmd::evtGetValue(evtEntry, args[3]);
        s32 in = evtmgr_cmd::evtGetValue(evtEntry, args[4]);
        // Prepare the linear function's slope and y-intercept.
        f32 m = (((f32)y1 - (f32)y2) / ((f32)x1 - (f32)x2));
        f32 b = ((f32)y1 / m);
        b = (((f32)x1 - b) * (m * -1));
        // Feed the input through this linear function as x.
        s32 out = msl::math::floor((m * (f32)in) + b);
        evtmgr_cmd::evtSetValue(evtEntry, args[5], out);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(IntplUltra, 6)

    EVT_BEGIN(disorder_pulse)
     // Thank you Kora for teaching me how to do linear functions for secondary interpolations, I haven't done these since 10th grade lol
     // This aided immensely in creating my own function to do all the work for me so I never have to remember rise over run again! I hope.
    IF_EQUAL(GSW(1630), 0)
    RETURN()
    END_IF()
    SET(LW(2), 255)
    SET(LW(3), 255)
    SET(LW(4), 1700)
    SET(LW(5), 255)
    DO(0)
    WAIT_MSEC(2000)
    SWITCH(GSW(1630))
        CASE_EQUAL(1) // APATHY/RED; 255/200/200 <-> 255/225/225
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, LW(2), 200, LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(evt_map::evt_map_set_blend, 0, 255, LW(0), LW(0), 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
            WAIT_MSEC(500)
            SET(LW(2), 225)
            SET(LW(4), 1000)
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 200, LW(2), LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(evt_map::evt_map_set_blend, 0, 255, LW(0), LW(0), 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
        CASE_EQUAL(2) // DREAD/ORANGE; 255/210/150 <-> 255/235/180
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, LW(2), 210, LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(IntplUltra, 210, 150, LW(2), LW(5), LW(0), LW(3))
                USER_FUNC(evt_map::evt_map_set_blend, 0, 255, LW(0), LW(3), 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
            WAIT_MSEC(500)
            SET(LW(2), 235)
            SET(LW(5), 180)
            SET(LW(4), 1000)
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 210, LW(2), LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(IntplUltra, 210, 150, LW(2), LW(5), LW(0), LW(3))
                USER_FUNC(evt_map::evt_map_set_blend, 0, 255, LW(0), LW(3), 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
        CASE_EQUAL(3) // PREJUDICE/YELLOW; 255/255/115 <-> 255/255/200
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, LW(2), 115, LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(evt_map::evt_map_set_blend, 0, 255, 255, LW(0), 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
            WAIT_MSEC(500)
            SET(LW(2), 200)
            SET(LW(4), 1000)
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 115, LW(2), LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(evt_map::evt_map_set_blend, 0, 255, 255, LW(0), 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
        CASE_EQUAL(4) // INDIFFERENCE/GREEN; 215/255/215 <-> 235/255/235
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, LW(2), 215, LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(evt_map::evt_map_set_blend, 0, LW(0), 255, LW(0), 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
            WAIT_MSEC(500)
            SET(LW(2), 235)
            SET(LW(4), 1000)
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 215, LW(2), LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(evt_map::evt_map_set_blend, 0, LW(0), 255, LW(0), 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
        CASE_EQUAL(5) // RECALCITRANCE/CYAN; 225/255/255 <-> 210/255/210
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, LW(2), 210, LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(evt_map::evt_map_set_blend, 0, LW(0), 255, 255, 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
            WAIT_MSEC(500)
            SET(LW(2), 225)
            SET(LW(4), 1000)
            USER_FUNC(evt_sub::evt_sub_intpl_msec_init, 11, 210, LW(2), LW(4))
            DO(0)
                USER_FUNC(evt_sub::evt_sub_intpl_msec_get_value)
                USER_FUNC(evt_map::evt_map_set_blend, 0, LW(0), 255, 255, 255)
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
    END_SWITCH()
    WHILE()
    RETURN()
    EVT_END()

    EVT_BEGIN(dan_enemy_room_init_evt_new)
    SET(LW(0), GSW(1))
    USER_FUNC(get_mover_rng, LW(1)) // Movers
    USER_FUNC(evt_dan_read_mover_rng, LW(1))
    IF_SMALL_EQUAL(LW(1), 14)
    USER_FUNC(evt_npc::evt_npc_entry, PTR("mover"), PTR("n_stg2_syuuzin_b"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("mover"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(moverAnims))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("mover"), 0, true)
    USER_FUNC(evt_npc::evt_npc_add_flip_part, PTR("mover"))
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("mover"), -40, 0, 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("mover"), 9, PTR(fwd_mover_speech))
    ELSE()
    IF_LARGE(GSW(1630), 0)
    RUN_EVT(disorder_pulse)
    END_IF()
    END_IF()
    USER_FUNC(dan::evt_dan_read_data)
    USER_FUNC(dan::evt_dan_handle_map_parts, LW(0))
    USER_FUNC(dan::evt_dan_handle_dokans, LW(0))
    USER_FUNC(evt_door::evt_door_set_dokan_descs, PTR(&dan::dan_dokanDescs), 8)
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
    USER_FUNC(create_holographic_enemy, LW(5), LW(6))
    IF_EQUAL(LW(6), 1)
    USER_FUNC(evt_npc::evt_npc_set_animpose_disp_callback, LW(5), PTR(mi4::mi4MimiHolographicEffect), 0)
    END_IF()
    WHILE()
    END_IF()
    ADD(LW(10), 1)
    WHILE()
    IF_EQUAL(GSW(1601), 1) // Tatarian Aster
    USER_FUNC(rand100, LW(8))
    IF_SMALL(LW(8), 70)
    USER_FUNC(dan::evt_dan_decide_key_enemy, 48)
    END_IF()
    ELSE()
    USER_FUNC(dan::evt_dan_decide_key_enemy, 48)
    END_IF()
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
    IF_EQUAL(GSW(1620), 2)
    IF_EQUAL(GSWF(1603), 0)
    SET(GSWF(1603), 1)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(hardDifficultyFirst), 0, 0)
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_IF()
    END_IF()
    IF_EQUAL(GSW(1630), 4) // GREEN/INDIFFERENCE
    RUN_CHILD_EVT(handle_indifference)
    END_IF()
    USER_FUNC(evt_npc::evt_npc_unfreeze_all)
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

    // Dialogue to determine quickstart or no
    EVT_BEGIN(determine_quickstart)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(quickstartText), 0, 0)
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(quickstartOptions))
    USER_FUNC(evt_msg::evt_msg_continue)
    SWITCH(LW(0))
    CASE_EQUAL(1) // 2-1
    SET(GSW(137), 1)
    SET(GSW(0), 78)
    USER_FUNC(start_from_21)
    CASE_EQUAL(2) // 6-1
    SET(GSW(137), 2)
    SET(GSW(0), 226)
    USER_FUNC(start_from_61)
    CASE_EQUAL(3) // Postgame
    SET(GSW(137), 3)
    SET(GSW(0), 424)
    USER_FUNC(start_from_eg)
    CASE_ETC() // Normal pills
    SET(GSW(137), 0)
    END_SWITCH()
    IF_NOT_EQUAL(GSW(137), 0)
    SET(GSWF(2), 1)
    SET(GSWF(386), 1)
    SET(GSWF(387), 1)
    SET(GSWF(431), 1)
    SET(GSWF(501), 1)
    SET(GSWF(512), 1)
    SET(GSWF(517), 1)
    SET(GSWF(612), 1)
    SET(GSWF(614), 1)
    SET(GSWF(615), 1)
    SET(GSWF(616), 1)
    USER_FUNC(evt_seq::evt_seq_set_seq, seqdrv::SEQ_MAPCHANGE, PTR("mac_05"), PTR("elv1"))
    END_IF()
    RETURN_FROM_CALL()

    EVT_BEGIN(disable_flopside_pit_entrance)
    USER_FUNC(evt_door::evt_door_enable_disable_dokan_desc, 0, PTR("dokan_1"))
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("dokan_1"), 1)
    USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A2d_dokan_1"), 1, 0)
    USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A3d_dokan_1"), 1, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(patch_pit_exit)
    SET(GSW(1601), 0)
    SET(GSW(1600), 0)
    SET(GSW(1602), 0)
    USER_FUNC(evt_door::evt_door_set_dokan_descs, PTR(&new_dan_70_dokan_desc), 1)
    RETURN_FROM_CALL()

    EVT_BEGIN(patch_shadoo_health)
    USER_FUNC(declare_shadoo_stats, LW(10))
    RETURN_FROM_CALL()

    s32 ToggleGSWF(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        s32 gswfId = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 gswfVal = swdrv::swGet(gswfId);
        if (gswfVal == 0)
        {
            swdrv::swSet(gswfId);
        }
        else
        {
            swdrv::swClear(gswfId);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(ToggleGSWF, 1)

    EVT_BEGIN(jimbo_real)
    SWITCH(GW(0))
    // FEATURES
    CASE_EQUAL(0)
    IF_EQUAL(GSWF(1604), 0)
    IF_EQUAL(GSW(1622), 0) // Shadoo not defeated, but Features menu opened
    SET(GSWF(1604), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(jimboWarn), 0, PTR("me"))
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(yesNoSelect_NoByDefault))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 1)
    SET(GW(0), -1)
    SET(LW(1), -1)
    END_IF()
    END_IF()
    END_IF()
    IF_NOT_EQUAL(GW(0), -1)
    USER_FUNC(loadCustomSelectTex, 2)
    USER_FUNC(evt_sub::evt_sub_item_select_menu, 0, PTR(&featuresItems), LW(1), 0)
    SWITCH(LW(1))
    CASE_EQUAL(65) // MOVERS
    USER_FUNC(ToggleGSWF, 1610)
    CASE_EQUAL(66) // MERLUNA
    USER_FUNC(ToggleGSWF, 1611)
    CASE_EQUAL(67) // MARIO
    USER_FUNC(ToggleGSWF, 1612)
    END_SWITCH()
    END_IF()
    // PATCHES
    CASE_EQUAL(1)
    USER_FUNC(loadCustomSelectTex, 3)
    USER_FUNC(evt_sub::evt_sub_item_select_menu, 0, PTR(&patchesItems), LW(1), 0)
    SWITCH(LW(1))
    CASE_EQUAL(65) // LOCKS
    USER_FUNC(ToggleGSWF, 1620)
    CASE_EQUAL(66) // HP SOUND
    USER_FUNC(ToggleGSWF, 1621)
    END_SWITCH()
    // ACCESSIBILITY
    CASE_EQUAL(2)
    USER_FUNC(loadCustomSelectTex, 4)
    USER_FUNC(evt_sub::evt_sub_item_select_menu, 0, PTR(&accessibilityItems), LW(1), 0)
    SWITCH(LW(1))
    CASE_EQUAL(65) // EXPLOSION SHAKES
    USER_FUNC(ToggleGSWF, 1630)
    END_SWITCH()
    // LOOP OR END SEQUENCE
    END_SWITCH()
    IF_NOT_EQUAL(LW(1), -1)
    WAIT_MSEC(500)
    RUN_EVT(jimbo_real)
    RETURN()
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(jimboAnythingElse), 0, PTR("me"))
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(jimboSettings))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_NOT_EQUAL(LW(0), 3)
    SET(GW(0), LW(0))
    RUN_EVT(jimbo_real)
    RETURN()
    ELSE()
    // END JIMBO CUTSCENE
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(jimboBye), 0, PTR("me"))
    USER_FUNC(loadCustomSelectTex, 0)
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 1000, 11)
    WAIT_MSEC(1000)
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_IF()
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(jimbo_speech)
    USER_FUNC(evt_mario::evt_mario_key_off, 1)
    USER_FUNC(evt_cam::evt_cam_get_pos, 5, LW(0), LW(1), LW(2))
    USER_FUNC(evt_cam::evt_cam3d_evt_zoom_in, 1, LW(0), 60, LW(2), EVT_NULLPTR, 60, -1100, 1000, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(3), LW(4), LW(5))
    SUB(LW(3), 45)
    USER_FUNC(evt_mario::evt_mario_walk_to, LW(3), LW(5), 500)
    USER_FUNC(evt_mario::evt_mario_face_npc, PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), LW(9))
    WAIT_MSEC(1000)
    // Jimbo first cutscene
    IF_EQUAL(GSWF(586), 0)
    SET(GSWF(586), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(jimboIntro), 0, PTR("me"))
    ELSE()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(jimboIntro2), 0, PTR("me"))
    END_IF()
    USER_FUNC(evt_msg::evt_msg_select, 1, PTR(jimboSettings))
    USER_FUNC(evt_msg::evt_msg_continue)
    IF_NOT_EQUAL(LW(0), 3)
    SET(GW(0), LW(0))
    RUN_CHILD_EVT(jimbo_real)
    RETURN()
    END_IF()
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(jimboBye), 0, PTR("me"))
    // END JIMBO CUTSCENE
    USER_FUNC(loadCustomSelectTex, 0)
    USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 1000, 11)
    WAIT_MSEC(1000)
    USER_FUNC(evt_mario::evt_mario_key_on)
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_jimbo_speech)
    RUN_EVT(jimbo_speech)
    RETURN()
    EVT_END()

    EVT_BEGIN(patch_mac_05)
    SET(GSW(1), 0)
    SET(GSW(1600), 0)
    SET(GSW(1601), 0)
    SET(GSW(1602), 0)
    SET(GSW(1610), 0)
    SET(GSW(1611), 0)
    SET(GSW(1620), 0)
    USER_FUNC(evt_pouch::evt_pouch_check_have_item, 48, LW(15)) // Removes Pit Key if Mario has one
    IF_EQUAL(LW(15), 1)
    USER_FUNC(evt_pouch::evt_pouch_remove_item, 48)
    END_IF()
    RUN_CHILD_EVT(handle_dj_misc_behavior)
    USER_FUNC(evt_npc::evt_npc_entry, PTR("jimbo"), PTR("e_antho"), 0)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("jimbo"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(heihoAnims))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("jimbo"), 0, true)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR("jimbo"), -50, 0, -1250)
    USER_FUNC(evt_npc::evt_npc_set_property, PTR("jimbo"), 9, PTR(fwd_jimbo_speech))
    USER_FUNC(evt_npc::evt_npc_add_flip_part, PTR("jimbo"))
    USER_FUNC(evt_npc::evt_npc_set_scale, PTR("jimbo"), FLOAT(1.1287), FLOAT(1.1287), FLOAT(1.1287))
    RETURN_FROM_CALL()

    EVT_BEGIN(global_operations_non_dan)
    USER_FUNC(update_bump_tex)
    DO(0)
    USER_FUNC(animPoseSetMaterialEvtColorWrapper, GSWF(1643))
    // Also handle debug frame color while we're at it
    USER_FUNC(updateDebugFrameColor)
    WAIT_FRM(1)
    WHILE()
    RETURN()
    EVT_END()

    EVT_BEGIN(global_operations)
    RUN_EVT(global_operations_non_dan)
    USER_FUNC(evt_sub::evt_sub_get_mapname, 0, LW(14)) // Pit-Only runtime
    USER_FUNC(evtCompareStrings, LW(14), PTR("dan"), LW(15))
    IF_NOT_EQUAL(LW(15), 1)
    RETURN()
    ELSE()
    DO(0)                   // Start global operations loop
    IF_EQUAL(GSWF(1605), 1) // Used for RFC removal after saying "No" to open the chest
    SET(GSWF(1605), 0)
    USER_FUNC(evt_mobj::evt_mobj_delete, PTR("box"))
    USER_FUNC(evt_mobj::evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), PTR(new_dan_chest_interact_evt), PTR(new_dan_chest_open_evt), 0, 0)
    USER_FUNC(evt_npc::evt_npc_unfreeze_all)
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_IF()
    IF_EQUAL(GSWF(1670), 1) // If Migraine has just been activated
    SET(GSWF(1670), 0)
    INLINE_EVT()
    WAIT_MSEC(500)
    USER_FUNC(migraineShortenCurseTimers)
    END_INLINE()
    END_IF()                // End Migraine check
    IF_EQUAL(GSWF(1672), 1) // Hourai Doll cutscene
    SET(GSWF(1672), 0)
    INLINE_EVT()
    WAIT_MSEC(1000)
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(3), LW(4), LW(5))
    USER_FUNC(evt_mario::evt_mario_get_height, LW(7))
    ADD(LW(4), LW(7))
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
    WAIT_MSEC(700)
    SUB(LW(4), 7)
    USER_FUNC(evt_eff::evt_eff, 0, PTR("spm_recovery"), LW(3), LW(4), LW(5), -1, 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(2)
    ADD(LW(3), 10)
    ADD(LW(4), 10)
    ADD(LW(5), 10)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(2)
    ADD(LW(3), -20)
    ADD(LW(4), -20)
    ADD(LW(5), -20)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(2)
    ADD(LW(4), 20)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(2)
    ADD(LW(3), 20)
    ADD(LW(4), -20)
    ADD(LW(5), 20)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MSEC(200)
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("I_2"), 0)
    SET(GSW(1602), 19)
    WAIT_MSEC(1800)
    IF_EQUAL(GSWF(1673), 0)
    SET(GSWF(1673), 1)
    USER_FUNC(evt_msg::evt_msg_print, 1, PTR(houraiFirstUse), 0, 0)
    ELSE()
    WAIT_MSEC(700)
    END_IF()
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_mario_motion_chg_mot, 0)
    USER_FUNC(evt_mario::evt_mario_key_on)
    END_INLINE()
    END_IF()               // End Hourai Doll activation check
    IF_EQUAL(GSW(1600), 3) // Begin Paramita active check
    // Handle cooldown timer first
    IF_LARGE(GSW(1610), 0)  // Seconds timer above 0
    IF_LARGE(GSW(1611), 0)  // Frame timer above 0
    SUB(GSW(1611), 1)       // Reduce frame timer by 1
    ELSE()                  // Frame timer hits 0, underflow
    SET(GSW(1611), 59)      // Reset frames to 60
    SUB(GSW(1610), 1)       // Subtract 1 second from the primary Paramita timer
    IF_EQUAL(GSW(1610), 20) // Play sfx when cooldown begins
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(4), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(0), LW(1), LW(2))
    END_IF()
    END_IF()
    ELSE()
    IF_LARGE(GSW(1611), 0) // If sec = 0, set frame timer to 0
    SET(GSW(1611), 0)
    END_IF()
    END_IF()
    // Handle Paramita activation detection
    IF_EQUAL(GSW(1610), 0)
    IF_EQUAL(GSW(1611), 0)              // Both timers must be zeroed
    USER_FUNC(evt_mario_chk_key, LW(0)) // Mario must be keyed on
    IF_EQUAL(LW(0), 1)
    USER_FUNC(evt_sub::evt_key_get_button, 0, LW(0))
    IF_FLAG(LW(0), 0x400) // B must be a button held
    SET(GSW(1610), 50)
    SET(GSW(1611), 60) // Sets Paramita timers
    USER_FUNC(evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(4), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(0), LW(1), LW(2))
    USER_FUNC(paramitaFloor, LW(0))
    USER_FUNC(evt_mario::evt_mario_take_damage, 2, 0, 0, 0, 0, LW(0))
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    WAIT_FRM(1)
    WHILE()
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(run_global_operations)
    RUN_EVT(global_operations)
    RETURN_FROM_CALL()

    EVT_BEGIN(dan_enter_pipe_wait)
    WAIT_MSEC(0)
    RETURN_FROM_CALL()

    // TEMP
    /*  EVT_BEGIN(temp_pipe)
      USER_FUNC(evt_door::evt_door_set_dokan_descs, PTR(&temp_mac_04_2_dokan_desc), 1)
      RETURN_FROM_CALL() */

    // Dark Puff contact ATK patch
    EVT_BEGIN(d_puff_dir_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 1)
    RETURN_FROM_CALL()

    // Dark Puff projectile ATK patch
    EVT_BEGIN(d_puff_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 2)
    RETURN_FROM_CALL()

    // Ice Bro projectile ATK patch
    EVT_BEGIN(i_bro_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Ice Bro contact ATK patch
    EVT_BEGIN(i_bro_dir_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    RETURN_FROM_CALL()

    // Hyper Goomba contact ATK patch
    EVT_BEGIN(h_goomba_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Hyper Paragoomba contact ATK patch
    EVT_BEGIN(h_pgoomba_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Red Magikoopa projectile ATK patch
    EVT_BEGIN(r_magi_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Tileoid PU ATK patch
    EVT_BEGIN(p_tile_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Tileoid PU speed patch
    EVT_BEGIN(p_tile_speed)
    USER_FUNC(patch_tileoid_pu)
    RETURN_FROM_CALL()

    // Kilo Muth ATK/DEF patch
    EVT_BEGIN(k_muth_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    RETURN_FROM_CALL()

    // Spiked Gloomba ATK patch
    EVT_BEGIN(s_gloomba_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 3)
    RETURN_FROM_CALL()

    // Shady Koopa/Flip Shady Koopa ATK patch
    EVT_BEGIN(uranoko_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 4)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 2, 4)
    RETURN_FROM_CALL()

    // Bleepboxer projectile ATK patch
    EVT_BEGIN(bleep_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 3)
    RETURN_FROM_CALL()

    // White Clubba ATK patch
    EVT_BEGIN(w_clubba_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 4)
    RETURN_FROM_CALL()

    // Bawbus ATK patch
    EVT_BEGIN(bawb_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Shady Boomerang Bro contact ATK patch
    EVT_BEGIN(sbb_dir_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    RETURN_FROM_CALL()

    // Shady Boomerang Bro proj ATK patch
    EVT_BEGIN(sbb_proj_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 5)
    RETURN_FROM_CALL()

    // Shady Magikoopa projectile speedup
    EVT_BEGIN(s_magi_proj_speed)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(9))
    IF_EQUAL(LW(9), 68)
    USER_FUNC(evt_npc::evt_npc_glide_to, PTR("me"), LW(0), LW(1), LW(2), 5000, FLOAT(420.0), 0, 0, 0, 0)
    ELSE()
    USER_FUNC(evt_npc::evt_npc_glide_to, PTR("me"), LW(0), LW(1), LW(2), 5000, FLOAT(120.0), 0, 0, 0, 0)
    END_IF()
    RETURN_FROM_CALL()

    // Shady Magikoopa projectile ATK
    EVT_BEGIN(s_magi_proj_atk)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Shady Magikoopa animation delay
    EVT_BEGIN(s_magi_delay)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(9))
    SWITCH(LW(9))
    CASE_EQUAL(66)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KAMEKU_MAGIC_SING1"), PTR("me"))
    WAIT_MSEC(1200)
    SWITCH_BREAK()
    CASE_EQUAL(67)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KAMEKU_MAGIC_SING1"), PTR("me"))
    WAIT_MSEC(1200)
    SWITCH_BREAK()
    CASE_ETC()
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KAMEKU_MAGIC_SING1"), PTR("me"))
    END_SWITCH()
    RETURN_FROM_CALL()

    // White Clubba movement speed
    EVT_BEGIN(w_clubba_mvmt)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(14))
    IF_EQUAL(LW(14), 95)
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(3), LW(5), 0, 120, 4, 0, 0)
    ELSE()
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(3), LW(5), 0, 60, 4, 0, 0)
    END_IF()
    RETURN_FROM_CALL()

    // White Clubba triple attack
    EVT_BEGIN(w_clubba_triple)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(14))
    IF_EQUAL(LW(14), 95)
    IF_SMALL(LW(6), 128)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_GABON_HURU1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 27, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 1, 1)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_GABON_HURU1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 27, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 1, 1)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_GABON_HURU1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 27, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 1, 1)
    END_IF()
    ELSE()
    IF_SMALL(LW(6), 60)
    USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_GABON_HURU1"), PTR("me"))
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 27, 1)
    USER_FUNC(evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), 1, 1)
    END_IF()
    END_IF()
    RETURN_FROM_CALL()

    // Shady Koopa AI rework
    EVT_BEGIN(uranoko_brain)
    USER_FUNC(evt_npc::evt_npc_get_unitwork, PTR("me"), 9, LW(8))
    IF_EQUAL(LW(8), 1)
    USER_FUNC(evt_npc::evt_npc_set_unitwork, PTR("me"), 9, 0)
    USER_FUNC(evt_npc::evt_npc_get_axis_movement_unit, PTR("me"), LW(9))
    MUL(LW(9), -1)
    USER_FUNC(evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), LW(9))
    END_IF()
    RETURN_FROM_CALL()

    // Shady Koopa movement speed
    EVT_BEGIN(uranoko_mvmt)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(4), LW(5), LW(6))
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("target"), LW(7), LW(8), LW(9))
    SUB(LW(7), LW(4))
    IF_SMALL(LW(7), 0)
    SUB(LW(1), 250)
    ELSE()
    ADD(LW(1), 250)
    END_IF()
    USER_FUNC(evt_npc::evt_npc_set_unitwork, PTR("me"), 9, 1)
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 300, 4, 0, 0)
    USER_FUNC(evt_npc::evt_npc_set_unitwork, PTR("me"), 9, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(uranoko_dendrite)
    USER_FUNC(evt_npc::evt_npc_set_unitwork, PTR("me"), 9, 0)
    RETURN_FROM_CALL()

    s32 kami_bomb(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        npcdrv::NPCEntry *npc = evtEntry->ownerNPC;
        npcdrv::NPCPart *part = npcdrv::npcGetPartById(npc, 1);
        mario::MarioWork *mario = mario::marioGetPtr();
        f32 dist = PSVECSquareDistance(&npc->position, &mario->position);
        u8 marioIn3D = mario::marioCheck3d();
        u8 npcIn3D = npc->flippedTo3d;
        u8 failsIfOne = marioIn3D + npcIn3D;
        f32 distThreshold = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        if (dist < distThreshold && failsIfOne != 1)
        {
            if (mario->invincibilityTimer == 0 && mario->motionId != MOT_DAMAGE && mario->motionId != MOT_FLIP && mario->motionId != MOT_FLIP_AIR && mario->motionId != MOT_BOTTOMLESS && mario->motionId != MOT_CHAR_CHANGE && mario->motionId != MOT_FAIRY_CHANGE)
            {
                npcdrv::npcDamageMario(npc, part, &part->position, 0, part->attackPower, 4);
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(kami_bomb, 1)

    // Kamikaze Goomba explosion visuals & second attack
    EVT_BEGIN(kami_no_dokkan)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(11))
    IF_EQUAL(LW(11), 10)
    // Explosion #1
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(8), LW(9))
    USER_FUNC(kami_bomb, 5200)
    USER_FUNC(evt_eff::evt_eff, 0, PTR("spm_explosion"), 2, LW(11), LW(8), LW(9), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(11), LW(8), LW(9))
    INLINE_EVT()
    IF_EQUAL(GSWF(1630), 0)
    USER_FUNC(evt_cam::evt_cam_shake, 5, FLOAT(0.8), FLOAT(0.8), FLOAT(0.0), 173, 0)
    END_IF()
    END_INLINE()
    // Explosion #2 if difficulty is Normal
    IF_LARGE_EQUAL(GSW(1620), 1)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("target"), LW(11), LW(8), LW(9))
    ADD(LW(8), 50)
    USER_FUNC(evt_npc::evt_npc_arc_to, PTR("me"), LW(11), LW(8), LW(9), 800, 0, 45, 0, 16, 0)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(8), LW(9))
    USER_FUNC(kami_bomb, 5200)
    USER_FUNC(evt_eff::evt_eff, 0, PTR("spm_explosion"), 2, LW(11), LW(8), LW(9), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(11), LW(8), LW(9))
    INLINE_EVT()
    IF_EQUAL(GSWF(1630), 0)
    USER_FUNC(evt_cam::evt_cam_shake, 5, FLOAT(0.8), FLOAT(0.8), FLOAT(0.0), 173, 0)
    END_IF()
    END_INLINE()
    END_IF()
    // Explode twice more if difficulty is Hard
    // Explosion #2 if difficulty is Normal
    IF_EQUAL(GSW(1620), 2)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("target"), LW(11), LW(8), LW(9))
    ADD(LW(8), 50)
    USER_FUNC(evt_npc::evt_npc_arc_to, PTR("me"), LW(11), LW(8), LW(9), 800, 0, 45, 0, 16, 0)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(8), LW(9))
    USER_FUNC(kami_bomb, 5200)
    USER_FUNC(evt_eff::evt_eff, 0, PTR("spm_explosion"), 2, LW(11), LW(8), LW(9), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(11), LW(8), LW(9))
    INLINE_EVT()
    IF_EQUAL(GSWF(1630), 0)
    USER_FUNC(evt_cam::evt_cam_shake, 5, FLOAT(0.8), FLOAT(0.8), FLOAT(0.0), 173, 0)
    END_IF()
    END_INLINE()
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("target"), LW(11), LW(8), LW(9))
    ADD(LW(8), 50)
    USER_FUNC(evt_npc::evt_npc_arc_to, PTR("me"), LW(11), LW(8), LW(9), 800, 0, 45, 0, 16, 0)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(8), LW(9))
    USER_FUNC(kami_bomb, 5200)
    USER_FUNC(evt_eff::evt_eff, 0, PTR("spm_explosion"), 2, LW(11), LW(8), LW(9), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(11), LW(8), LW(9))
    INLINE_EVT()
    IF_EQUAL(GSWF(1630), 0)
    USER_FUNC(evt_cam::evt_cam_shake, 5, FLOAT(0.8), FLOAT(0.8), FLOAT(0.0), 173, 0)
    END_IF()
    END_INLINE()
    END_IF()
    // Bounce
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("target"), LW(11), LW(8), LW(9))
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(13), LW(14), LW(15))
    USER_FUNC(evt_npc::evt_npc_arc_to, PTR("me"), LW(11), LW(14), LW(9), 500, 0, 30, 0, 16, 0)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(8), LW(9))
    END_IF()
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_KURIBOO_LANDING1"), LW(11), LW(8), LW(9))
    RETURN_FROM_CALL()

    // Bomb Boo explosion damage
    EVT_BEGIN(bomb_boo_atk)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(13))
    IF_EQUAL(LW(13), 86)
    USER_FUNC(kami_bomb, 6900)
    USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(13), LW(14), LW(15))
    USER_FUNC(evt_eff::evt_eff, 0, PTR("spm_explosion"), 2, LW(13), LW(14), LW(15), FLOAT(1.2), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(13), LW(14), LW(15))
    INLINE_EVT()
    IF_EQUAL(GSWF(1630), 0)
    USER_FUNC(evt_cam::evt_cam_shake, 5, FLOAT(1.3), FLOAT(1.3), FLOAT(0.0), 173, 0)
    END_IF()
    END_INLINE()
    END_IF()
    RETURN_FROM_CALL()

    // Skellobyte delay shorten
    EVT_BEGIN(skello_ai)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(6))
    SWITCH(LW(6))
    CASE_EQUAL(225)
    WAIT_MSEC(0)
    SWITCH_BREAK()
    CASE_EQUAL(228)
    WAIT_MSEC(0)
    SWITCH_BREAK()
    CASE_ETC()
    USER_FUNC(evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 200, 0, FLOAT(10.0), 0, 0, 0)
    SWITCH_BREAK()
    END_SWITCH()
    RETURN_FROM_CALL()

    // Skellobyte faster attacks
    EVT_BEGIN(skello_ai2)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(6))
    SWITCH(LW(6))
    CASE_EQUAL(225)
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(0), LW(2), 0, FLOAT(255.0), 28, 0, 0)
    SWITCH_BREAK()
    CASE_EQUAL(228)
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(0), LW(2), 0, FLOAT(255.0), 28, 0, 0)
    SWITCH_BREAK()
    CASE_ETC()
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(0), LW(2), 0, FLOAT(100.0), 28, 0, 0)
    SWITCH_BREAK()
    END_SWITCH()
    RETURN_FROM_CALL()

    // Skellobyte faster movement in general
    EVT_BEGIN(skello_ai3)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(6))
    SWITCH(LW(6))
    CASE_EQUAL(225)
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, FLOAT(60.0), 28, 0, 0)
    SWITCH_BREAK()
    CASE_EQUAL(228)
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, FLOAT(60.0), 28, 0, 0)
    SWITCH_BREAK()
    CASE_ETC()
    USER_FUNC(evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, FLOAT(40.0), 28, 0, 0)
    SWITCH_BREAK()
    END_SWITCH()
    RETURN_FROM_CALL()

    /*
        EVT_BEGIN(uranoko_mvmtcheck)
        LBL(73)
        USER_FUNC(evt_npc::evt_npc_get_position, PTR("target"), LW(4), LW(5), LW(6))
        USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(7), LW(8), LW(9))
        SUB(LW(8), LW(5))
        IF_LARGE(LW(8), 50)
        WAIT_FRM(1)
        GOTO(73)
        END_IF()
        IF_SMALL(LW(8), -50)
        WAIT_FRM(1)
        GOTO(73)
        END_IF()
        RETURN_FROM_CALL() */

    // IF_SMALL(LW(7), 0)
    // USER_FUNC(evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), 1)
    // ELSE()
    // USER_FUNC(evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), -1)
    // END_IF()

    // Dark Lakitu stats
    EVT_BEGIN(dlak_stats)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 34)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 1)
    ELSE()
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    USER_FUNC(setHitFlags, 0)
    USER_FUNC(setHitboxSize, 0, FLOAT(24), FLOAT(32), FLOAT(10))
    END_IF()
    RETURN_FROM_CALL()

    // Dark Lakitu proj limit patch
    EVT_BEGIN(dlak_limit)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 34)
    USER_FUNC(temp_unk::lakitu_count_spinies, 36, LW(0))
    ELSE()
    USER_FUNC(temp_unk::lakitu_count_spinies, 38, LW(0))
    END_IF()
    RETURN_FROM_CALL()

    /*   // Replace Sky Blue Spiny anim defs to fix Dark Lakitu bullshit
       EVT_BEGIN(skyblue_anim_defs)
       USER_FUNC(evt_npc::evt_npc_set_property, PTR("me"), 14, PTR(&skyblueAnims))
       RETURN_FROM_CALL() */

    // Dark Lakitu proj patch 2
    EVT_BEGIN(dlak_skyblue_2)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 34)
    USER_FUNC(temp_unk::lakitu_spawn_spiny, 59)
    ELSE()
    USER_FUNC(temp_unk::lakitu_spawn_spiny, 336)
    END_IF()
    RETURN_FROM_CALL()

    // Dark Lakitu proj patch
    EVT_BEGIN(dlak_skyblue)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 34)
    USER_FUNC(evt_npc::evt_npc_agb_async, PTR("e_togezo"), LW(0))
    ELSE()
    USER_FUNC(evt_npc::evt_npc_agb_async, PTR("e_togezb"), LW(0))
    END_IF()
    RETURN_FROM_CALL()

    /*   // KP Koopa stats
       EVT_BEGIN(kp_stats)
       USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
       IF_EQUAL(LW(1), 14)
       USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 1)
       ELSE()
       USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
       //   USER_FUNC(setHitFlags, 1)
       //   USER_FUNC(setFlag2c, 0, 0x304000)
       //   USER_FUNC(setFlag2c, 1, 0x224000)
       //   USER_FUNC(setHitboxSize, 0, FLOAT(20), FLOAT(22), FLOAT(20))
       //   USER_FUNC(setHitboxSize, 1, FLOAT(14), FLOAT(10), FLOAT(20))
       END_IF()
       RETURN_FROM_CALL() */

    // Shy Guy stats
    EVT_BEGIN(heiho_stats)
    USER_FUNC(evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 0)
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 1)
    ELSE()
    USER_FUNC(evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    IF_LARGE(LW(1), 500)
    //  USER_FUNC(setHitboxSize, 0, FLOAT(27), FLOAT(27), FLOAT(27))
    USER_FUNC(evt_npc::evt_npc_set_scale, PTR("me"), FLOAT(1.1287), FLOAT(1.1287), FLOAT(1.1287))
    END_IF()
    END_IF()
    RETURN_FROM_CALL()

    static void evtPatches()
    {
        // Initialize the patches to the EVT interpreter to add custom opcodes
        evtpatch::evtmgrExtensionInit();

        /* // Get Flipside Tower's init EVT script
         evtmgr_cmd::EvtScriptCode *flipsideInitEvt = map_data::mapDataPtr("mac_02")->initScript;

         // Add a hook at the beginning of Flipside's init EVT script that will play our custom script first
         evtpatch::hookEvt(flipsideInitEvt, 1, flipside_nastasia_funny); */

        // Render Flopside Pit pipe useless and patch Pit exit pipe
        evtmgr_cmd::EvtScriptCode *disableFlopsidePitEntrance = map_data::mapDataPtr("mac_15")->initScript;
        evtpatch::hookEvt(disableFlopsidePitEntrance, 11, disable_flopside_pit_entrance);
        evtpatch::hookEvtReplace(dan::dan_70_init_evt, 8, patch_pit_exit);

        // Enemy room init evt complete rewrite
        evtpatch::hookEvtReplace(dan::dan_enemy_room_init_evt, 1, dan_enemy_room_init_evt_new);

        // Get enemy onSpawnScripts from templates
        evtmgr_cmd::EvtScriptCode *dPuffDirAtk = npcdrv::npcEnemyTemplates[357].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *dPuffAtk = npcdrv::npcEnemyTemplates[358].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *iBroDirAtk = npcdrv::npcEnemyTemplates[341].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *iBroAtk = npcdrv::npcEnemyTemplates[344].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *hGoombaAtk = npcdrv::npcEnemyTemplates[329].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *hpGoombaAtk = npcdrv::npcEnemyTemplates[328].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *rMagiAtk = npcdrv::npcEnemyTemplates[324].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *pTileAtk = npcdrv::npcEnemyTemplates[379].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *pTileSpeed = npcdrv::npcEnemyTemplates[379].moveScript;
        evtmgr_cmd::EvtScriptCode *kMuthAtk = npcdrv::npcEnemyTemplates[279].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *sGloombaAtk = npcdrv::npcEnemyTemplates[6].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *uranokoAtk = npcdrv::npcEnemyTemplates[10].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *fUranokoAtk = npcdrv::npcEnemyTemplates[13].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *bleepAtk = npcdrv::npcEnemyTemplates[277].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *wClubbaAtk = npcdrv::npcEnemyTemplates[350].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *sbbDirAtk = npcdrv::npcEnemyTemplates[340].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *sbbProjAtk = npcdrv::npcEnemyTemplates[343].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *sMagiProjSpeed = npcdrv::npcEnemyTemplates[347].moveScript;
        evtmgr_cmd::EvtScriptCode *sMagiProjAtk = npcdrv::npcEnemyTemplates[347].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *wClubbaTriple = npcdrv::npcEnemyTemplates[350].atkScript;
        evtmgr_cmd::EvtScriptCode *uranokoMvmt = npcdrv::npcEnemyTemplates[10].atkScript;
        evtmgr_cmd::EvtScriptCode *uranokoBrain = npcdrv::npcEnemyTemplates[10].moveScript;
        evtmgr_cmd::EvtScriptCode *uranokoOnDmgTaken = npcdrv::npcEnemyTemplates[10].onHitScript;
        evtmgr_cmd::EvtScriptCode *kamiKuriAtk = npcdrv::npcEnemyTemplates[330].atkScript;
        evtmgr_cmd::EvtScriptCode *bombBooAtk = npcdrv::npcEnemyTemplates[366].atkScript;
        evtmgr_cmd::EvtScriptCode *dLakAtk = npcdrv::npcEnemyTemplates[30].atkScript;
        evtmgr_cmd::EvtScriptCode *dLakStats = npcdrv::npcEnemyTemplates[30].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *bawbAtk = npcdrv::npcEnemyTemplates[375].onSpawnScript;
        //    evtmgr_cmd::EvtScriptCode *kpStats = npcdrv::npcEnemyTemplates[7].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *goombaStats = npcdrv::npcEnemyTemplates[2].onSpawnScript;
        //    evtmgr_cmd::EvtScriptCode *skyblueAnimDefs = npcdrv::npcEnemyTemplates[336].onSpawnScript;
        evtmgr_cmd::EvtScriptCode *commonDeathScript = npcdrv::npcEnemyTemplates[350].deathScript;

        // Enemy stat overwrite
        evtpatch::hookEvtReplace(dPuffDirAtk, 2, d_puff_dir_atk);
        evtpatch::hookEvtReplace(dPuffAtk, 11, d_puff_atk);
        evtpatch::hookEvtReplace(iBroDirAtk, 1, i_bro_dir_atk);
        evtpatch::hookEvtReplace(iBroAtk, 1, i_bro_atk);
        evtpatch::hookEvtReplace(hGoombaAtk, 2, h_goomba_atk);
        evtpatch::hookEvtReplace(hpGoombaAtk, 2, h_pgoomba_atk);
        evtpatch::hookEvtReplace(rMagiAtk, 1, r_magi_atk);
        evtpatch::hookEvtReplace(pTileAtk, 1, p_tile_atk);
        evtpatch::hookEvt(pTileSpeed, 16, p_tile_speed);
        evtpatch::hookEvt(npc_tile::npc_tile_common_onspawn_evt, 6, p_tile_speed);
        evtpatch::hookEvtReplace(kMuthAtk, 1, k_muth_atk);
        evtpatch::hookEvtReplace(sGloombaAtk, 2, s_gloomba_atk);
        evtpatch::hookEvtReplace(uranokoAtk, 5, uranoko_atk);
        evtpatch::hookEvtReplace(fUranokoAtk, 2, uranoko_atk);
        evtpatch::hookEvtReplace(bleepAtk, 1, bleep_atk);
        evtpatch::hookEvtReplace(wClubbaAtk, 1, w_clubba_atk);
        evtpatch::hookEvtReplace(sbbDirAtk, 1, sbb_dir_atk);
        evtpatch::hookEvtReplace(sbbProjAtk, 1, sbb_proj_atk);
        evtpatch::hookEvtReplace(sMagiProjSpeed, 24, s_magi_proj_speed);
        evtpatch::hookEvtReplace(sMagiProjAtk, 1, s_magi_proj_atk);
        evtpatch::hookEvtReplace(wClubbaTriple, 36, w_clubba_mvmt);
        evtpatch::hookEvtReplace(wClubbaTriple, 50, w_clubba_mvmt);
        evtpatch::hookEvtReplaceBlock(wClubbaTriple, 52, w_clubba_triple, 59);
        evtpatch::hookEvtReplace(uranokoMvmt, 16, uranoko_mvmt);
        //    evtpatch::hookEvt(uranokoMvmt, 1, uranoko_mvmtcheck);
        evtpatch::hookEvt(uranokoBrain, 1, uranoko_brain);
        evtpatch::hookEvt(uranokoOnDmgTaken, 1, uranoko_dendrite);
        evtpatch::hookEvtReplace(kamiKuriAtk, 51, kami_no_dokkan);
        evtpatch::hookEvt(bombBooAtk, 34, bomb_boo_atk);
        evtpatch::hookEvtReplace(temp_unk::skellobits_unk7, 7, skello_ai);
        evtpatch::hookEvtReplace(temp_unk::skellobits_unk7, 25, skello_ai2);
        evtpatch::hookEvtReplace(temp_unk::skellobits_unk2, 16, skello_ai3);
        evtpatch::hookEvtReplace(temp_unk::skellobits_unk8, 16, skello_ai2);
        evtpatch::hookEvt(temp_unk::kameks_unk7, 18, s_magi_delay);
        evtpatch::hookEvtReplace(temp_unk::dark_broom_kamek_unk7, 17, s_magi_delay);
        evtpatch::hookEvtReplace(dLakAtk, 31, dlak_skyblue_2);
        evtpatch::hookEvtReplace(dLakAtk, 12, dlak_skyblue);
        evtpatch::hookEvtReplace(dLakAtk, 7, dlak_limit);
        evtpatch::hookEvtReplace(temp_unk::lakitu_onspawn_child, 8, dlak_stats);
        evtpatch::hookEvtReplace(bawbAtk, 1, bawb_atk);
        //    evtpatch::hookEvtReplace(kpStats, 4, kp_stats);
        evtpatch::hookEvtReplace(goombaStats, 2, heiho_stats);
        evtpatch::hookEvtReplaceBlock(temp_unk::goomba_unk2_child, 12, heiho_g_panic, 16);

        // Ninjoe patched to only bomb 10% of the time, thanks Lily!
        writeWord(&npc_ninja::ninjoe_bomb_calc_chance, 0xDC, 0x2C03000A);

        // Shadoo
        evtpatch::hookEvtReplace(dan::dan_start_shadoo_evt, 1, fwd_new_shadoo_evt);
        evtpatch::hookEvtReplaceBlock(dan::dan_shadoo_defeat_evt, 13, shadoo_defeat_msg, 15);
        evtpatch::hookEvt(dan::dan_shadoo_fight_evt, 97, patch_shadoo_health);
        evtpatch::hookEvt(dan::dan_shadoo_fight_evt, 56, patch_shadoo_health);
        evtpatch::hookEvt(dan::dan_shadoo_fight_evt, 15, patch_shadoo_health);

        // Pit music replacement
        evtpatch::hookEvt(evt_door::evt_door_dan_dokan, 79, determine_custom_music);
        evtpatch::hookEvtReplace(dan::dan_exit_pipe_sign_interact_evt, 2, custom_music_sign);

        // Pit room pipe speedup
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_left_one, 108, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_left_two, 92, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_left_three, 85, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_left_four, 71, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_left_one, 99, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_left_two, 83, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_left_three, 76, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_left_four, 62, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_right_one, 119, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_right_two, 92, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_right_three, 85, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_right_four, 71, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_right_one, 110, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_right_two, 83, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_right_three, 76, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(evt_door::evt_door_dan_dokan_right_four, 62, dan_enter_pipe_wait);

        // Quickstart
        evtpatch::hookEvt(aa1_01::aa1_01_mario_house_transition_evt, 10, determine_quickstart);

        // Custom NPCs
        evtpatch::hookEvtReplaceBlock(dan::dan_chest_room_init_evt, 65, merluna_setup, 84);
        evtpatch::hookEvtReplace(map_data::mapDataPtr("mac_05")->initScript, 79, patch_mac_05);
        writeWord(&evt_shop::evt_shop_build_flimm_pit_item_tables, 0xB8, 0x2C000001); // Patch Pit Flimm's double pricing in the Flopside Pit
        evtpatch::hookEvt(temp_unk::default_item_use_evt, 4, bump_item_use_hook);

        // Blessings/Curses
        evtpatch::hookEvt(commonDeathScript, 4, spectre_logic);
        evtpatch::hookEvt(evt_door::door_init_evt, 1, run_global_operations);

        // Flipside Pit 3D Thoreau Lock patch
        writeWord(&mario_motion::func_80144908, 0x164, 0x60000000);

        // Holo coin patch
        evtpatch::hookEvtReplace(temp_unk::npc_drop_item_evt, 3, npc_drop_item_patch);
    }

    void main()
    {
        wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
        tplpatch::iconPatch("wicon2");
        titleScreenCustomTextPatch();
        messagePatch();
        danOverwrite();
        pluswinWhackaBump();
        danYouSuck();
        danDontFuckingCrash();
        patchNpcRgbaFuncs();
        danPatchPitEnemies();
        evtPatches();
        patchMarioDamage();
        dimenPatch();
    }
}