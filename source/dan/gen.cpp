#pragma once
#include <common.h>
#include <evt_cmd.h>
#include <gen.h>
#include <util.h>
#include <cutscene_helpers.h>
#include <evtpatch.h>
#include <tplpatch.h>
#include <sndpatch.h>
#include <mod.h>
#include <lunatic/npcdata.h>

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

    static double boobies = 0;
    static s32 enemyArrayOffset = 0;

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
    
    s32 enemyConfigArray[2001] = {169};

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

    // Pit Flimm item pool to iterate from
    s32 rotenShopItemPool[] = {81, 82, 84, 89, 90, 95, 99, 106, 106, 106, 106, 118, 123, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
                               141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160, 161, 162, 163, 164, 166, 168, 169, 170,
                               171, 173, 174, 177, 178, 180, 181, 182, 183, 184, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 198, 199, 200,
                               201, 202, 203, 204, 205, 207, 208, 209, 211, 212, 214, 215};

    s32 rotenShopLowerClassItemPool[] = {65, 66, 67, 68, 69, 70, 73, 74, 75, 76, 77, 78, 79, 83, 86, 98, 104, 109, 113};

    s32 rfcItems[] = {0, 0, 0, -1};

    /*
        HIGH PRIORITY TODO (I BROKE HELLA SHIT):
        add types/enums like DisorderId to a special file
        extern rfcItems or add all relevant evtpatches/functions to gen.cpp if they are generation-related (I'm not checking)
        add room generation-related evtpatches here
        update mod.cpp to exclude these functions' and global variables' instantiations. I'm preserving them for now in case I actually broke hella double triple quadruple shit

        LOW PRIORITY TODO:
        clean up the first half of evt_dan_read_data_new to run some things only on pit entry or otherwise conditionally
        clean up actual code in many different sections
        set template field unknown_0x8 for unused enemies on mod start rather than on every pit npc entry, that shoooould work?
    */ 


    void DanGen_Items(s32 currentFloor)
    {
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
        for (s32 i = 0; i < 3; ++i)
        {
            itemRarity = 0;
            n = 0;
            // Determine item rarity. One success makes it Uncommon, two successes makes it Rare, none makes it Common.
            do
            {
                n = n + 1;
                s32 rand100Num = system::rand() % 100;
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
                s32 rand100Num = system::rand() % 4;
                if (itemId != rfcItems[0] && itemId != rfcItems[1] && itemId != rfcItems[2] && itemSubrarity > rand100Num)
                {
                    rfcItems[i] = itemId; // Assigns the item to the rfcItems array
                    wii::os::OSReport("Item %d was assigned to rfcItems. itemId is %d.\n", i, itemId);
                    itemAssigned = true;
                }
            } while (!itemAssigned); // Loops until the rand is below the subrarity value AND item isn't identical to previously assigned items
        }
        return;
    }

    void DanGen_SegmentsAndDoors(s32 currentFloor)
    {
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
                segmentCount = 5;
            s32 disorderId = swdrv::swByteGet(1630);
            if (disorderId == DisorderId::DISORDER_GREEN) // Guarantee a maximally complex layout while Indifference is active
            {
                segmentCount = 16;
            }
            wii::os::OSReport("Rolled segmentRNG = %d, queueing %d segment generations for this room.\n", segmentRNG, segmentCount);

            /*
                ENCLOSED STRUCTURE GENERATION
                (If more than 2 segments are generated at any time, the remainder is subtracted from segmentCount)
            */
            s32 i = 0;
            s32 structureRNG = system::rand() % 180;
            s32 structureMiscRNG = system::rand() % 100;
            if (disorderId == DisorderId::DISORDER_GREEN && structureRNG > 30) // Guarantee an enclosed structure while Indifference is active
                structureRNG = system::rand() % 31;

            // SPLIT
            if (structureRNG >= 0 && structureRNG < 8)
            {
                wii::os::OSReport("!!! SPLIT Structure is generating !!!\n");
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
                wii::os::OSReport("!!! QUARTER Structure is generating !!!\n");
                if (structureMiscRNG < 31)
                {
                    segment100 = true;
                    segment8 = true;
                    if (segment40000)
                    {
                        if (boobies < 50)
                            segment80000 = true;
                    }
                    else
                        segment80000 = true;
                }
                else if (structureMiscRNG < 54)
                {
                    segment40 = true;
                    segment8 = true;
                    if (segment40000)
                    {
                        if (boobies < 50)
                            segment80000 = true;
                    }
                    else
                        segment80000 = true;
                }
                else if (structureMiscRNG < 77)
                {
                    segment100 = true;
                    segment2000 = true;
                    if (segment10000)
                    {
                        if (boobies < 50)
                            segment20000 = true;
                    }
                    else
                        segment20000 = true;
                }
                else
                {
                    segment40 = true;
                    segment2000 = true;
                    if (segment10000)
                    {
                        if (boobies < 50)
                            segment20000 = true;
                    }
                    else
                        segment20000 = true;
                }
            }

            // Cube
            else if (structureRNG >= 19 && structureRNG < 21)
            {
                wii::os::OSReport("!!! CUBE Structure is generating !!!\n");
                segmentCount = segmentCount - 2;
                if (structureMiscRNG < 50)
                {
                    segment400 = true;
                    segment80 = true;
                    segment1 = true;
                    segment20 = true;
                    segment20000 = true;
                    segment40000 = true;
                }
                else
                {
                    segment8000 = true;
                    segment200 = true;
                    segment20 = true;
                    segment1 = true;
                    segment20000 = true;
                    segment40000 = true;
                }
            }

            // Pillar
            else if (structureRNG >= 21 && structureRNG < 23)
            {
                wii::os::OSReport("!!! PILLAR Structure is generating !!!\n");
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
                    segment80000 = true;
            }

            // Small
            else if (structureRNG >= 23 && structureRNG < 26)
            {
                wii::os::OSReport("!!! SMALL Structure is generating !!!\n");
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
                wii::os::OSReport("!!! LOBATE Structure is generating !!!\n");
                segmentCount = segmentCount - 2;
                if (structureMiscRNG <= 66)
                {
                    if (boobies < 25)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment200 = true;
                        segment8000 = true;
                        segment20000 = true;
                    }
                    else if (boobies < 50)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment80 = true;
                        segment400 = true;
                        segment20000 = true;
                    }
                    else if (boobies < 75)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment200 = true;
                        segment20 = true;
                        segment20000 = true;
                    }
                    else
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment80 = true;
                        segment1 = true;
                        segment20000 = true;
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
                    }
                    else if (boobies < 25)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment200 = true;
                        segment4000 = true;
                    }
                    else if (boobies < 37.5)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment80 = true;
                        segment2 = true;
                    }
                    else if (boobies < 50)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment200 = true;
                        segment10 = true;
                    }
                    else if (boobies < 62.5)
                    {
                        segment2 = true;
                        segment4 = true;
                        segment200 = true;
                        segment4000 = true;
                    }
                    else if (boobies < 75)
                    {
                        segment10 = true;
                        segment200 = true;
                        segment800 = true;
                        segment1000 = true;
                    }
                    else if (boobies < 87.5)
                    {
                        segment4 = true;
                        segment10 = true;
                        segment80 = true;
                        segment800 = true;
                    }
                    else
                    {
                        segment2 = true;
                        segment80 = true;
                        segment1000 = true;
                        segment4000 = true;
                    }
                }
            }
            // Structure does not generate
            else
                structureGenerated = false;

            wii::os::OSReport("Structure check passed with RNG val %d, %d segments to generate remaining.\n", structureRNG, segmentCount);

            /*
                SEGMENT GENERATION PROCEDURE & EVERY NECESSARY CHECK
                (This made me want to kill myself. Badly. It took weeks back in summer 2024 but it was worth)
            */
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

            /*
                FINISH ROOM LAYOUT
            */

            // Pipes
            if (!structureGenerated)
            {
                s32 pipeRNG = system::rand() % 133;
                if (pipeRNG >= 0 && pipeRNG < 39)
                    segment20000 = true;
                else if (pipeRNG >= 39 && pipeRNG < 76)
                    segment80000 = true;
                else if (pipeRNG >= 76 && pipeRNG < 80 && segment1 && segment20)
                    segment40000 = true;
                else if (pipeRNG >= 80 && pipeRNG < 82 && segment400 && segment8000)
                    segment10000 = true;
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
                roomDecCode = roomDecCode + 0x400;
            if (segment800)
                roomDecCode = roomDecCode + 0x800;
            if (segment1000)
                roomDecCode = roomDecCode + 0x1000;
            if (segment2000)
                roomDecCode = roomDecCode + 0x2000;
            if (segment4000)
                roomDecCode = roomDecCode + 0x4000;
            if (segment8000)
                roomDecCode = roomDecCode + 0x8000;
            if (segment40)
                roomDecCode = roomDecCode + 0x40;
            if (segment80)
                roomDecCode = roomDecCode + 0x80;
            if (segment100)
                roomDecCode = roomDecCode + 0x100;
            if (segment200)
                roomDecCode = roomDecCode + 0x200;
            if (segment1)
                roomDecCode = roomDecCode + 0x1;
            if (segment2)
                roomDecCode = roomDecCode + 0x2;
            if (segment4)
                roomDecCode = roomDecCode + 0x4;
            if (segment8)
                roomDecCode = roomDecCode + 0x8;
            if (segment10)
                roomDecCode = roomDecCode + 0x10;
            if (segment20)
                roomDecCode = roomDecCode + 0x20;

            wii::os::OSReport("Finished generating. Roomcode w/o pipes is %d.\n", roomDecCode);

            if (segment10000)
                roomDecCode = roomDecCode + 0x10000;
            if (segment20000)
                roomDecCode = roomDecCode + 0x20000;
            if (segment40000)
                roomDecCode = roomDecCode + 0x40000;
            if (segment80000)
                roomDecCode = roomDecCode + 0x80000;

            if (currentFloor == 198) // Literally never gets ran, maybe doesn't matter? Revisit this if Room 99 generation is ever buggy.
            {
                roomDecCode = 0;
                activeDoorCount = 19;
            }
        }

        /*
            DOORS
            (I would make this a separate function, but it relies on previously established vars a lot so it can stay here)
        */
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

        for (s32 i = 0; i < 200; ++i)
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
        return;
    }

    s32 evt_dan_read_data_new(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)entry;

        /*
            NO CLUE IF THIS IS STILL NECESSARY, BUT FUCK IT WE BALL
        */
        if (isFirstCall)
        {
            dan::dan_wp = (dan::DanWork *)memory::__memAlloc(memory::Heap::HEAP_MAP, sizeof(dan::DanWork));
            msl::string::memset(dan::dan_wp, 0, sizeof(*dan::dan_wp));
            dan::dan_wp->dungeons = (dan::DanDungeon *)memory::__memAlloc(memory::Heap::HEAP_MAP, sizeof(dan::DanDungeon[DUNGEON_MAX]));
            msl::string::memset(dan::dan_wp->dungeons, 0, sizeof(dan::DanDungeon[DUNGEON_MAX]));
        }

        /*
            SET UP VARIABLES AND DO RANDOM PIT/ROOM ENTRY THINGS
            (Some of this CAN be thrown into a new entry-only function later!)
        */

        // Reset Pit chests
        for (u8 ThakoGswf = 433; ThakoGswf <= 450; ++ThakoGswf) // This loop is untested, I hope it works lol
        {
            swdrv::swClear(ThakoGswf);
        }

        // Clear Merluna judgement flag
        swdrv::swClear(1669);

        s32 i = 0;
        s32 currentFloor = swdrv::swByteGet(1);
        s32 shadooEntries = swdrv::swByteGet(24);

        // Global room generation RNG value (1-100, 5 decimal places)
        // Hi, it's Yme from November 2025. I should hang myself for doing this. I'm not changing it though
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

        // Check if room 49, then shove you into Flopside Pit (Doesn't really update until Floor 50)
        if (currentFloor == 48)
        {
            swdrv::swByteSet(1, 148);
        }

        // Ensure Shadoo is fought on first room entry, but never again
        if (shadooEntries == 0)
        {
            swdrv::swByteSet(24, 1);
        }

        /*
            INIT PIT ROOM RANDOMIZATION PROCESS
        */

        DanGen_SegmentsAndDoors(currentFloor);

        DanGen_Items(currentFloor);

        // Fuck this I'm not gonna separate this into DanGen_Enemies yet lol
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
                s32 difficulty = swdrv::swByteGet(1620);
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
                    s32 difficulty = swdrv::swByteGet(1620);
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
                        arrayRNG = system::rand() % (sizeof(mod::getlv1Tribes()) / 4);
                        tribeArray = 1;
                        enemyTribe = mod::getlv1Tribes()[arrayRNG];
                        wii::os::OSReport("Room #%d: tribeArray %d selected; (enemyGenRNG = %d) < (lv1Lim + limMod = %d). limMod = %d, lim = %d.\n", roomGens, tribeArray, enemyGenRNG, ((danLevelData->lv1Limiter) + limiterMod), limiterMod, danLevelData->lv1Limiter);
                    }
                    else if (enemyGenRNG < ((danLevelData->lv2Limiter) + limiterMod))
                    {
                        arrayRNG = system::rand() % (sizeof(mod::getlv2Tribes()) / 4);
                        tribeArray = 2;
                        enemyTribe = mod::getlv2Tribes()[arrayRNG];
                        wii::os::OSReport("Room #%d: tribeArray %d selected; (enemyGenRNG = %d) < (lv2Lim + limMod = %d). limMod = %d, lim = %d.\n", roomGens, tribeArray, enemyGenRNG, ((danLevelData->lv2Limiter) + limiterMod), limiterMod, danLevelData->lv2Limiter);
                    }
                    else if (enemyGenRNG < ((danLevelData->lv3Limiter) + limiterMod))
                    {
                        arrayRNG = system::rand() % (sizeof(mod::getlv3Tribes()) / 4);
                        tribeArray = 3;
                        enemyTribe = mod::getlv3Tribes()[arrayRNG];
                        wii::os::OSReport("Room #%d: tribeArray %d selected; (enemyGenRNG = %d) < (lv3Lim + limMod = %d). limMod = %d, lim = %d.\n", roomGens, tribeArray, enemyGenRNG, ((danLevelData->lv3Limiter) + limiterMod), limiterMod, danLevelData->lv3Limiter);
                    }
                    else
                    {
                        arrayRNG = system::rand() % (sizeof(mod::getlv4Tribes()) / 4);
                        tribeArray = 4;
                        enemyTribe = mod::getlv4Tribes()[arrayRNG];
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
                            arrayRNG = system::rand() % (sizeof(mod::getlv1Tribes()) / 4);
                            enemyTribe = mod::getlv1Tribes()[arrayRNG];
                        }
                        else if (tribeArray == 2)
                        {
                            arrayRNG = system::rand() % (sizeof(mod::getlv2Tribes()) / 4);
                            enemyTribe = mod::getlv2Tribes()[arrayRNG];
                        }
                        else if (tribeArray == 3)
                        {
                            arrayRNG = system::rand() % (sizeof(mod::getlv3Tribes()) / 4);
                            enemyTribe = mod::getlv3Tribes()[arrayRNG];
                        }
                        else
                        {
                            arrayRNG = system::rand() % (sizeof(mod::getlv4Tribes()) / 4);
                            enemyTribe = mod::getlv4Tribes()[arrayRNG];
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
        if (returnMoverRng() > 14)
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
        s32 difficulty = swdrv::swByteGet(1620);
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
        if (returnMoverRng() <= 14)
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

        s32 blessingRoomCounter = swdrv::swByteGet(1602);
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

    // Force unused enemies to not crash the fucking game when spawned in the Pit
    s32 evt_dan_get_enemy_info_new(evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)isFirstCall;

        // Get dungeon and enemy index
        evtmgr::EvtScriptCode *args = entry->pCurData;
        s32 enemyTribex = evtmgr_cmd::evtGetValue(entry, args[1]);
        s32 currentFloor = swdrv::swByteGet(1);
        dan::DanDungeon *dungeon = dan::dan_wp->dungeons + currentFloor;

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
    s32 * getRfcItems()
    {
      return rfcItems;
    }
}