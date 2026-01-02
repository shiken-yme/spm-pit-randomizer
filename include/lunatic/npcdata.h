#pragma once
#include <common.h>
#include <spm/npcdrv.h>

namespace mod
{
    // npcdata.h is a custom header to help streamline the custom Pit's enemy generation process.
    // It contains data on NPCs, the scaling "levels," and difficulty settings.

    using namespace spm::npcdrv;

    // Enemies are selected from these arrays based on the current floor, which determines which DanLevelData to use
    NPCTribeId lv1Tribes[] = {
        NPC_GOOMBA,
        NPC_FLIP_GOOMBA,
        NPC_PARAGOOMBA,
        NPC_KOOPA_TROOPA,
        NPC_KOOPA_TROOPA_STG3,
        NPC_RED_KOOPA,
        NPC_FLIP_RED_KOOPA,
        NPC_PARATROOPA,
        NPC_PARATROOPA_STG3,
        NPC_BUZZY_BEETLE,
        NPC_STONE_BUZZY,
        NPC_BOO,
        NPC_GREEN_CHEEP_CHEEP,
        NPC_RED_CHEEP_CHEEP,
        NPC_FLIP_GREEN_CHEEP_CHEEP,
        NPC_BALD_CLEFT,
        NPC_SQUIGLET,
        NPC_SWAMPIRE, // Red Shy Guy
        NPC_SQUIG,
        NPC_SPROING_OING,
        NPC_BOING_OING,
        NPC_BOOMBOXER,
        NPC_GROWMEBA,
        NPC_JELLIEN,
        NPC_CURSYA,
        NPC_CHERBIL,
        NPC_POISON_CHERBIL,
        NPC_FRACKLE,
        NPC_SPINIA,
        NPC_FLIP_BUZZY_BEETLE,
        NPC_DARK_RUFF_PUFF,
        NPC_JAWBUS};

    NPCTribeId lv2Tribes[] = {
        NPC_GLOOMBA,
        NPC_SPIKED_GOOMBA,
        NPC_UNUSED_SPIKED_GOOMBA,
        NPC_RED_PARATROOPA,
        NPC_FLIP_RED_PARATROOPA,
        NPC_SPIKE_TOP,
        NPC_PARABUZZY,
        NPC_SPINY,
        NPC_FLIP_SPINY,
        NPC_HAMMER_BRO,
        NPC_MAGIKOOPA,
        NPC_FUZZY,
        NPC_POKEY,
        NPC_SPANIA,
        NPC_CHAIN_CHOMP,
        NPC_SQUOG,
        NPC_BEEPBOXER,
        NPC_MISTER_I,
        NPC_SHLURP,
        NPC_TILEOID_G,
        NPC_TILEOID_B,
        NPC_GREEN_FUZZY, // Blue Shy Guy
        NPC_RAWBUS,
        NPC_PATROLMEOW,
        NPC_LONGATOR,
        NPC_PIGARITHM,
        NPC_FLORO_SAPIEN_YELLOW,
        NPC_FLORO_SAPIEN_PURPLE,
        NPC_FLORO_SAPIEN_RED,
        NPC_NINJOE,
        NPC_TECH_CURSYA,
        NPC_HEAVY_CURSYA,
        NPC_REVERSYA_CURSYA,
        NPC_ICE_CHERBIL,
        NPC_FLIP_SPIKE_TOP,
        NPC_MOON_CLEFT,
        NPC_PURPLE_MUTH,
        NPC_GREEN_BOOMBOXER};

    NPCTribeId lv3Tribes[] = {
        NPC_HEADBONK_GOOMBA,
        NPC_KOOPATROL,
        NPC_SPIKY_PARABUZZY,
        NPC_DRY_BONES,
        NPC_FLIP_HAMMER_BRO,
        NPC_BOOMERANG_BRO,
        NPC_FIRE_BRO,
        NPC_BROOM_MAGIKOOPA,
        NPC_KOOPA_STRIKER,
        NPC_BILL_BLASTER,
        NPC_DARK_BOO,
        NPC_CLUBBA,
        NPC_PINK_FUZZY,
        NPC_DARK_CLEFT,
        NPC_RUFF_PUFF,
        NPC_CRAZEE_DAYZEE,
        NPC_ZOING_OING,
        NPC_BLOMEBA,
        NPC_TILEOID_R,
        NPC_TILEOID_Y,
        NPC_MEOWBOMB_SURPRISEMEOW,
        NPC_FOTON,
        NPC_HOOLIGON,
        NPC_GOLD_FUZZY, // Green Shy Guy
        NPC_COPTA,
        NPC_HOGARITHM,
        NPC_NINJOHN,
        NPC_FLIP_SKELLOBIT,
        NPC_FLIP_SKELLOBOMBER_HEAD,
        NPC_MUTH,
        NPC_SPUNIA,
        NPC_DARK_GOOMBA,
        NPC_DARK_SPIKED_GOOMBA,
        NPC_DARK_PARAGOOMBA,
        NPC_DARK_JAWBUS,
        NPC_DARK_KOOPA,
        NPC_DARK_PARATROOPA,
        NPC_FLIP_BOOMERANG_BRO,
        NPC_FLIP_FIRE_BRO,
        NPC_SOOPA_STRIKER,
        NPC_DARK_FUZZY,
        NPC_DARK_FIRE_BRO,
        NPC_RED_MAGIKOOPA,
        NPC_RED_BROOM_MAGIKOOPA,
        NPC_WHITE_MAGIKOOPA,
        NPC_WHITE_BROOM_MAGIKOOPA,
        NPC_GREEN_MAGIKOOPA,
        NPC_LAKITU,
        NPC_FLIP_LAKITU,
        NPC_GREEN_BROOM_MAGIKOOPA};

    NPCTribeId lv4Tribes[] = {
        NPC_DULL_BONES,
        NPC_PHANTOM_EMBER, // Dark Lakitu
        NPC_POISON_POKEY,
        NPC_AMAZY_DAYZEE,
        NPC_RED_CHOMP,
        NPC_DARK_CHOMP,
        NPC_SQUOINKER,
        NPC_BLASTBOXER,
        NPC_RED_I,
        NPC_SHLORP,
        NPC_CHROMEBA,
        NPC_GAWBUS,
        NPC_LONGADILE,
        NPC_NINJERRY,
        NPC_FLIP_SPIKY_SKELLOBIT,
        NPC_FLIP_SKELLOBOMBER_WITH_SKELLOBAIT,
        NPC_MAGIBLOT_B,
        NPC_MAGIBLOT_R,
        NPC_MAGIBLOT_Y,
        NPC_BACK_CURSYA,
        NPC_GIGABITE,
        NPC_MEGA_MUTH,
        NPC_SHADY_KOOPA,
        NPC_FLIP_SHADY_KOOPA,
        NPC_DARK_CHERBIL,
        NPC_DARK_SPINY,
        NPC_DARK_SPIKE_TOP,
        NPC_DARK_DULL_BONES,
        NPC_DARK_CLUBBA,
        NPC_DARK_DARK_BOO,
        NPC_DARK_MAGIKOOPA,
        NPC_DARK_BROOM_MAGIKOOPA,
        NPC_DARK_TILEOID,
        NPC_DARK_NINJOE,
        NPC_DARK_KOOPATROL,
        NPC_DARK_HEADBONK_GOOMBA,
        NPC_DARK_SKELLOBIT,
        NPC_DARK_SPIKY_SKELLOBIT,
        NPC_DARK_HAMMER_BRO,
        NPC_DARK_BOOMERANG_BRO,
        NPC_DARK_STRIKER};

    // Obtained from
    struct DanNPCData
    {
        s32 name;
        s32 odds;
        s32 spr;
        s32 min;
        s32 max;
    };

    // All npcStructs are named after their "name" field, 1 plus their tribe ID
    DanNPCData npcStruct1 = {1, 70, 0, 4, 7};
    DanNPCData npcStruct2 = {2, 65, 0, 3, 6};
    DanNPCData npcStruct3 = {3, 40, 0, 3, 6};
    DanNPCData npcStruct4 = {4, 85, 0, 4, 6};
    DanNPCData npcStruct5 = {5, 60, 0, 3, 6};
    DanNPCData npcStruct6 = {6, 50, 0, 3, 5};
    DanNPCData npcStruct7 = {7, 30, 0, 3, 6};
    DanNPCData npcStruct8 = {8, 60, 0, 3, 5};
    DanNPCData npcStruct9 = {9, 30, 0, 3, 6};
    DanNPCData npcStruct10 = {10, 80, 0, 4, 6};
    DanNPCData npcStruct11 = {11, 70, 0, 4, 6};
    DanNPCData npcStruct12 = {12, 25, 0, 3, 5};
    DanNPCData npcStruct13 = {13, 30, 0, 3, 5};
    DanNPCData npcStruct15 = {15, 35, 0, 3, 5};
    DanNPCData npcStruct16 = {16, 25, 0, 3, 5};
    DanNPCData npcStruct17 = {17, 20, 0, 2, 5};
    DanNPCData npcStruct19 = {19, 35, 0, 2, 5};
    DanNPCData npcStruct20 = {20, 30, 0, 1, 4};
    DanNPCData npcStruct21 = {21, 25, 0, 3, 4};
    DanNPCData npcStruct22 = {22, 25, 0, 3, 4};
    DanNPCData npcStruct23 = {23, 40, 0, 3, 5};
    DanNPCData npcStruct24 = {24, 40, 0, 3, 4};
    DanNPCData npcStruct25 = {25, 25, 0, 2, 5};
    DanNPCData npcStruct26 = {26, 20, 0, 3, 5};
    DanNPCData npcStruct28 = {28, 40, 0, 3, 5};
    DanNPCData npcStruct30 = {30, 70, 0, 1, 2};
    DanNPCData npcStruct31 = {31, 25, 2, 3, 4};
    DanNPCData npcStruct32 = {32, 30, 2, 2, 4};
    DanNPCData npcStruct33 = {33, 45, 0, 3, 6};
    DanNPCData npcStruct35 = {35, 60, 0, 1, 2};
    DanNPCData npcStruct36 = {36, 50, 0, 1, 2};
    DanNPCData npcStruct37 = {37, 45, 0, 3, 6};
    DanNPCData npcStruct38 = {38, 35, 0, 3, 6};
    DanNPCData npcStruct39 = {39, 75, 0, 2, 6};
    DanNPCData npcStruct40 = {40, 30, 0, 3, 5};
    DanNPCData npcStruct42 = {42, 65, 0, 2, 5};
    DanNPCData npcStruct44 = {44, 60, 0, 1, 3};
    DanNPCData npcStruct46 = {46, 70, 0, 2, 4};
    DanNPCData npcStruct48 = {48, 45, 0, 1, 3};
    DanNPCData npcStruct50 = {50, 50, 0, 1, 2};
    DanNPCData npcStruct52 = {52, 50, 0, 3, 4};
    DanNPCData npcStruct54 = {54, 30, 0, 2, 5};
    DanNPCData npcStruct56 = {56, 50, 0, 1, 2};
    DanNPCData npcStruct58 = {58, 50, 0, 3, 4};
    DanNPCData npcStruct60 = {60, 30, 0, 2, 5};
    DanNPCData npcStruct62 = {62, 50, 0, 2, 5};
    DanNPCData npcStruct64 = {64, 70, 0, 2, 5};
    DanNPCData npcStruct65 = {65, 50, 0, 2, 4};
    DanNPCData npcStruct67 = {67, 66, 0, 1, 3};
    DanNPCData npcStruct68 = {68, 44, 0, 1, 3};
    DanNPCData npcStruct70 = {70, 65, 0, 2, 5};
    DanNPCData npcStruct74 = {74, 50, 0, 2, 4};
    DanNPCData npcStruct76 = {76, 50, 0, 2, 5};
    DanNPCData npcStruct84 = {84, 30, 0, 2, 4};
    DanNPCData npcStruct85 = {85, 50, 0, 3, 6};
    DanNPCData npcStruct86 = {86, 60, 0, 3, 6};
    DanNPCData npcStruct87 = {87, 60, 0, 2, 4};
    DanNPCData npcStruct89 = {89, 50, 0, 2, 4};
    DanNPCData npcStruct90 = {90, 40, 0, 2, 4};
    DanNPCData npcStruct91 = {91, 20, 0, 2, 3};
    DanNPCData npcStruct95 = {95, 75, 0, 3, 5};
    DanNPCData npcStruct96 = {96, 70, 0, 1, 3};
    DanNPCData npcStruct97 = {97, 80, 0, 3, 6};
    DanNPCData npcStruct98 = {98, 65, 0, 4, 6};
    DanNPCData npcStruct99 = {99, 35, 0, 5, 8};
    DanNPCData npcStruct100 = {100, 40, 0, 3, 6};
    DanNPCData npcStruct101 = {101, 75, 0, 3, 5};
    DanNPCData npcStruct102 = {102, 35, 0, 3, 5};
    DanNPCData npcStruct103 = {103, 50, 0, 2, 4};
    DanNPCData npcStruct105 = {105, 80, 0, 1, 3};
    DanNPCData npcStruct110 = {110, 75, 0, 3, 6};
    DanNPCData npcStruct112 = {112, 70, 0, 2, 6};
    DanNPCData npcStruct114 = {114, 65, 0, 3, 6};
    DanNPCData npcStruct116 = {116, 69, 0, 3, 5};
    DanNPCData npcStruct118 = {118, 7, 0, 1, 2};
    DanNPCData npcStruct123 = {123, 50, 0, 1, 3};
    DanNPCData npcStruct124 = {124, 30, 0, 1, 3};
    DanNPCData npcStruct125 = {125, 10, 0, 1, 3};
    DanNPCData npcStruct126 = {126, 60, 0, 3, 6};
    DanNPCData npcStruct127 = {127, 61, 0, 3, 6};
    DanNPCData npcStruct129 = {129, 90, 0, 3, 6};
    DanNPCData npcStruct131 = {131, 80, 0, 2, 6};
    DanNPCData npcStruct135 = {135, 55, 0, 3, 5};
    DanNPCData npcStruct137 = {137, 55, 0, 2, 4};
    DanNPCData npcStruct139 = {139, 55, 0, 3, 5};
    DanNPCData npcStruct143 = {143, 50, 0, 2, 4};
    DanNPCData npcStruct145 = {145, 60, 0, 3, 4};
    DanNPCData npcStruct147 = {147, 80, 0, 2, 4};
    DanNPCData npcStruct151 = {151, 40, 0, 1, 3};
    DanNPCData npcStruct153 = {153, 20, 1, 1, 2};
    DanNPCData npcStruct157 = {157, 10, 0, 2, 4};
    DanNPCData npcStruct158 = {158, 20, 0, 1, 2};
    DanNPCData npcStruct160 = {160, 45, 0, 1, 2};
    DanNPCData npcStruct161 = {161, 40, 0, 1, 2};
    DanNPCData npcStruct162 = {162, 70, 0, 1, 2};
    DanNPCData npcStruct164 = {164, 40, 0, 2, 4};
    DanNPCData npcStruct165 = {165, 40, 0, 2, 4};
    DanNPCData npcStruct166 = {166, 40, 0, 2, 4};
    DanNPCData npcStruct167 = {167, 40, 0, 2, 4};
    DanNPCData npcStruct168 = {168, 74, 0, 2, 4};
    DanNPCData npcStruct169 = {169, 50, 1, 1, 3};
    DanNPCData npcStruct170 = {170, 20, 1, 1, 3};
    DanNPCData npcStruct171 = {171, 20, 1, 1, 2};
    DanNPCData npcStruct172 = {172, 20, 1, 1, 2};
    DanNPCData npcStruct174 = {174, 55, 0, 3, 5};
    DanNPCData npcStruct178 = {178, 10, 0, 2, 5};
    DanNPCData npcStruct179 = {179, 40, 0, 2, 5};
    DanNPCData npcStruct180 = {180, 30, 0, 2, 4};
    DanNPCData npcStruct185 = {185, 50, 0, 2, 3};
    DanNPCData npcStruct189 = {189, 80, 0, 3, 6};
    DanNPCData npcStruct190 = {190, 50, 0, 2, 5};
    DanNPCData npcStruct193 = {193, 60, 0, 3, 5};
    DanNPCData npcStruct201 = {201, 64, 0, 3, 5};
    DanNPCData npcStruct202 = {202, 40, 0, 2, 4};
    DanNPCData npcStruct206 = {206, 30, 0, 3, 5};
    DanNPCData npcStruct208 = {208, 30, 0, 3, 5};
    DanNPCData npcStruct210 = {210, 30, 0, 3, 5};
    DanNPCData npcStruct212 = {212, 40, 0, 2, 4};
    DanNPCData npcStruct215 = {215, 40, 0, 2, 4};
    DanNPCData npcStruct218 = {218, 40, 0, 2, 4};
    DanNPCData npcStruct221 = {221, 40, 0, 2, 4};
    DanNPCData npcStruct225 = {225, 50, 0, 3, 6};
    DanNPCData npcStruct226 = {226, 40, 0, 4, 6};
    DanNPCData npcStruct228 = {228, 50, 0, 2, 4};
    DanNPCData npcStruct229 = {229, 40, 0, 2, 5};
    DanNPCData npcStruct231 = {231, 30, 0, 3, 5};
    DanNPCData npcStruct234 = {234, 30, 0, 1, 4};
    DanNPCData npcStruct238 = {238, 40, 0, 2, 5};
    DanNPCData npcStruct240 = {240, 40, 0, 2, 5};
    DanNPCData npcStruct242 = {242, 40, 0, 2, 5};
    DanNPCData npcStruct246 = {246, 60, 0, 2, 5};
    DanNPCData npcStruct247 = {247, 4, 0, 1, 4};
    DanNPCData npcStruct248 = {248, 30, 0, 2, 4};
    DanNPCData npcStruct249 = {249, 25, 0, 2, 4};
    DanNPCData npcStruct250 = {250, 35, 0, 2, 4};
    DanNPCData npcStruct256 = {256, 10, 0, 1, 2};
    DanNPCData npcStruct258 = {258, 10, 1, 1, 1};
    DanNPCData npcStruct259 = {259, 10, 0, 1, 2};
    DanNPCData npcStruct441 = {441, 65, 0, 2, 4};
    DanNPCData npcStruct443 = {443, 70, 0, 2, 5};
    DanNPCData npcStruct445 = {445, 75, 0, 2, 4};
    DanNPCData npcStruct447 = {447, 73, 0, 2, 5};
    DanNPCData npcStruct451 = {451, 60, 0, 2, 4};
    DanNPCData npcStruct466 = {466, 60, 0, 2, 5};
    DanNPCData npcStruct467 = {467, 60, 0, 2, 5};
    DanNPCData npcStruct471 = {471, 25, 0, 2, 5};
    DanNPCData npcStruct472 = {472, 30, 0, 3, 5};
    DanNPCData npcStruct474 = {474, 20, 0, 2, 4};
    DanNPCData npcStruct475 = {475, 20, 0, 2, 4};
    DanNPCData npcStruct477 = {477, 20, 0, 2, 4};
    DanNPCData npcStruct478 = {478, 20, 0, 2, 4};
    DanNPCData npcStruct480 = {480, 20, 0, 2, 4};
    DanNPCData npcStruct481 = {481, 20, 0, 2, 4};
    DanNPCData npcStruct496 = {496, 40, 0, 2, 6};
    DanNPCData npcStruct497 = {497, 45, 0, 2, 5};
    DanNPCData npcStruct505 = {505, 55, 0, 2, 4};
    DanNPCData npcStruct507 = {507, 16, 0, 1, 2};
    DanNPCData npcStruct531 = {531, 25, 0, 1, 3}; // Green Shy Guy
    DanNPCData npcStruct532 = {532, 30, 0, 2, 4}; // Blue Shy Guy
    DanNPCData npcStruct533 = {533, 40, 0, 2, 5}; // Red Shy Guy
    DanNPCData npcStruct535 = {535, 15, 0, 1, 2}; // Dark Lakitu

    DanNPCData *DanNPC[] = {
        // Enemy tribe as [arg] aligns with "name"
        &npcStruct1,   // 1,
        &npcStruct2,   // 2,
        &npcStruct3,   // 3,
        &npcStruct4,   // 4,
        &npcStruct5,   // 5,
        &npcStruct6,   // 6,
        &npcStruct7,   // 7,
        &npcStruct8,   // 8,
        &npcStruct9,   // 9,
        &npcStruct10,  // 10,
        &npcStruct11,  // 11,
        &npcStruct12,  // 12,
        &npcStruct13,  // 13,
        nullptr,       // 14,
        &npcStruct15,  // 15,
        &npcStruct16,  // 16,
        &npcStruct17,  // 17,
        nullptr,       // 18,
        &npcStruct19,  // 19,
        &npcStruct20,  // 20,
        &npcStruct21,  // 21,
        &npcStruct22,  // 22,
        &npcStruct23,  // 23,
        &npcStruct24,  // 24,
        &npcStruct25,  // 25,
        &npcStruct26,  // 26,
        nullptr,       // 27,
        &npcStruct28,  // 28,
        nullptr,       // 29,
        &npcStruct30,  // 30,
        &npcStruct31,  // 31,
        &npcStruct32,  // 32,
        &npcStruct33,  // 33,
        nullptr,       // 34,
        &npcStruct35,  // 35,
        &npcStruct36,  // 36,
        &npcStruct37,  // 37,
        &npcStruct38,  // 38,
        &npcStruct39,  // 39,
        &npcStruct40,  // 40,
        nullptr,       // 41,
        &npcStruct42,  // 42,
        nullptr,       // 43,
        &npcStruct44,  // 44,
        nullptr,       // 45,
        &npcStruct46,  // 46,
        nullptr,       // 47,
        &npcStruct48,  // 48,
        nullptr,       // 49,
        &npcStruct50,  // 50,
        nullptr,       // 51,
        &npcStruct52,  // 52,
        nullptr,       // 53,
        &npcStruct54,  // 54,
        nullptr,       // 55,
        &npcStruct56,  // 56,
        nullptr,       // 57,
        &npcStruct58,  // 58,
        nullptr,       // 59,
        &npcStruct60,  // 60,
        nullptr,       // 61,
        &npcStruct62,  // 62,
        nullptr,       // 63,
        &npcStruct64,  // 64,
        &npcStruct65,  // 65,
        nullptr,       // 66,
        &npcStruct67,  // 67,
        &npcStruct68,  // 68,
        nullptr,       // 69,
        &npcStruct70,  // 70,
        nullptr,       // 71,
        nullptr,       // 72,
        nullptr,       // 73,
        &npcStruct74,  // 74,
        nullptr,       // 75,
        &npcStruct76,  // 76,
        nullptr,       // 77,
        nullptr,       // 78,
        nullptr,       // 79,
        nullptr,       // 80,
        nullptr,       // 81,
        nullptr,       // 82,
        nullptr,       // 83,
        &npcStruct84,  // 84,
        &npcStruct85,  // 85,
        &npcStruct86,  // 86,
        &npcStruct87,  // 87,
        nullptr,       // 88,
        &npcStruct89,  // 89,
        &npcStruct90,  // 90,
        &npcStruct91,  // 91,
        nullptr,       // 92,
        nullptr,       // 93,
        nullptr,       // 94,
        &npcStruct95,  // 95,
        &npcStruct96,  // 96,
        &npcStruct97,  // 97,
        &npcStruct98,  // 98,
        &npcStruct99,  // 99,
        &npcStruct100, // 100,
        &npcStruct101, // 101,
        &npcStruct102, // 102,
        &npcStruct103, // 103,
        nullptr,       // 104,
        &npcStruct105, // 105,
        nullptr,       // 106,
        nullptr,       // 107,
        nullptr,       // 108,
        nullptr,       // 109,
        &npcStruct110, // 110,
        nullptr,       // 111,
        &npcStruct112, // 112,
        nullptr,       // 113,
        &npcStruct114, // 114,
        nullptr,       // 115,
        &npcStruct116, // 116,
        nullptr,       // 117,
        &npcStruct118, // 118,
        nullptr,       // 119,
        nullptr,       // 120,
        nullptr,       // 121,
        nullptr,       // 122,
        &npcStruct123, // 123,
        &npcStruct124, // 124,
        &npcStruct125, // 125,
        &npcStruct126, // 126,
        &npcStruct127, // 127,
        nullptr,       // 128,
        &npcStruct129, // 129,
        nullptr,       // 130,
        &npcStruct131, // 131,
        nullptr,       // 132,
        nullptr,       // 133,
        nullptr,       // 134,
        &npcStruct135, // 135,
        nullptr,       // 136,
        &npcStruct137, // 137,
        nullptr,       // 138,
        &npcStruct139, // 139,
        nullptr,       // 140,
        nullptr,       // 141,
        nullptr,       // 142,
        &npcStruct143, // 143,
        nullptr,       // 144,
        &npcStruct145, // 145,
        nullptr,       // 146,
        &npcStruct147, // 147,
        nullptr,       // 148,
        nullptr,       // 149,
        nullptr,       // 150,
        &npcStruct151, // 151,
        nullptr,       // 152,
        &npcStruct153, // 153,
        nullptr,       // 154,
        nullptr,       // 155,
        nullptr,       // 156,
        &npcStruct157, // 157,
        &npcStruct158, // 158,
        nullptr,       // 159,
        &npcStruct160, // 160,
        &npcStruct161, // 161,
        &npcStruct162, // 162,
        nullptr,       // 163,
        &npcStruct164, // 164,
        &npcStruct165, // 165,
        &npcStruct166, // 166,
        &npcStruct167, // 167,
        &npcStruct168, // 168,
        &npcStruct169, // 169,
        &npcStruct170, // 170,
        &npcStruct171, // 171,
        &npcStruct172, // 172,
        nullptr,       // 173,
        &npcStruct174, // 174,
        nullptr,       // 175,
        nullptr,       // 176,
        nullptr,       // 177,
        &npcStruct178, // 178,
        &npcStruct179, // 179,
        &npcStruct180, // 180,
        nullptr,       // 181,
        nullptr,       // 182,
        nullptr,       // 183,
        nullptr,       // 184,
        &npcStruct185, // 185,
        nullptr,       // 186,
        nullptr,       // 187,
        nullptr,       // 188,
        &npcStruct189, // 189,
        &npcStruct190, // 190,
        nullptr,       // 191,
        nullptr,       // 192,
        &npcStruct193, // 193,
        nullptr,       // 194,
        nullptr,       // 195,
        nullptr,       // 196,
        nullptr,       // 197,
        nullptr,       // 198,
        nullptr,       // 199,
        nullptr,       // 200,
        &npcStruct201, // 201,
        &npcStruct202, // 202,
        nullptr,       // 203,
        nullptr,       // 204,
        nullptr,       // 205,
        &npcStruct206, // 206,
        nullptr,       // 207,
        &npcStruct208, // 208,
        nullptr,       // 209,
        &npcStruct210, // 210,
        nullptr,       // 211,
        &npcStruct212, // 212,
        nullptr,       // 213,
        nullptr,       // 214,
        &npcStruct215, // 215,
        nullptr,       // 216,
        nullptr,       // 217,
        &npcStruct218, // 218,
        nullptr,       // 219,
        nullptr,       // 220,
        &npcStruct221, // 221,
        nullptr,       // 222,
        nullptr,       // 223,
        nullptr,       // 224,
        &npcStruct225, // 225,
        &npcStruct226, // 226,
        nullptr,       // 227,
        &npcStruct228, // 228,
        &npcStruct229, // 229,
        nullptr,       // 230,
        &npcStruct231, // 231,
        nullptr,       // 232,
        nullptr,       // 233,
        &npcStruct234, // 234,
        nullptr,       // 235,
        nullptr,       // 236,
        nullptr,       // 237,
        &npcStruct238, // 238,
        nullptr,       // 239,
        &npcStruct240, // 240,
        nullptr,       // 241,
        &npcStruct242, // 242,
        nullptr,       // 243,
        nullptr,       // 244,
        nullptr,       // 245,
        &npcStruct246, // 246,
        &npcStruct247, // 247,
        &npcStruct248, // 248,
        &npcStruct249, // 249,
        &npcStruct250, // 250,
        nullptr,       // 251,
        nullptr,       // 252,
        nullptr,       // 253,
        nullptr,       // 254,
        nullptr,       // 255,
        &npcStruct256, // 256,
        nullptr,       // 257,
        &npcStruct258, // 258,
        &npcStruct259, // 259,
        nullptr,       // 260,
        nullptr,       // 261,
        nullptr,       // 262,
        nullptr,       // 263,
        nullptr,       // 264,
        nullptr,       // 265,
        nullptr,       // 266,
        nullptr,       // 267,
        nullptr,       // 268,
        nullptr,       // 269,
        nullptr,       // 270,
        nullptr,       // 271,
        nullptr,       // 272,
        nullptr,       // 273,
        nullptr,       // 274,
        nullptr,       // 275,
        nullptr,       // 276,
        nullptr,       // 277,
        nullptr,       // 278,
        nullptr,       // 279,
        nullptr,       // 280,
        nullptr,       // 281,
        nullptr,       // 282,
        nullptr,       // 283,
        nullptr,       // 284,
        nullptr,       // 285,
        nullptr,       // 286,
        nullptr,       // 287,
        nullptr,       // 288,
        nullptr,       // 289,
        nullptr,       // 290,
        nullptr,       // 291,
        nullptr,       // 292,
        nullptr,       // 293,
        nullptr,       // 294,
        nullptr,       // 295,
        nullptr,       // 296,
        nullptr,       // 297,
        nullptr,       // 298,
        nullptr,       // 299,
        nullptr,       // 300,
        nullptr,       // 301,
        nullptr,       // 302,
        nullptr,       // 303,
        nullptr,       // 304,
        nullptr,       // 305,
        nullptr,       // 306,
        nullptr,       // 307,
        nullptr,       // 308,
        nullptr,       // 309,
        nullptr,       // 310,
        nullptr,       // 311,
        nullptr,       // 312,
        nullptr,       // 313,
        nullptr,       // 314,
        nullptr,       // 315,
        nullptr,       // 316,
        nullptr,       // 317,
        nullptr,       // 318,
        nullptr,       // 319,
        nullptr,       // 320,
        nullptr,       // 321,
        nullptr,       // 322,
        nullptr,       // 323,
        nullptr,       // 324,
        nullptr,       // 325,
        nullptr,       // 326,
        nullptr,       // 327,
        nullptr,       // 328,
        nullptr,       // 329,
        nullptr,       // 330,
        nullptr,       // 331,
        nullptr,       // 332,
        nullptr,       // 333,
        nullptr,       // 334,
        nullptr,       // 335,
        nullptr,       // 336,
        nullptr,       // 337,
        nullptr,       // 338,
        nullptr,       // 339,
        nullptr,       // 340,
        nullptr,       // 341,
        nullptr,       // 342,
        nullptr,       // 343,
        nullptr,       // 344,
        nullptr,       // 345,
        nullptr,       // 346,
        nullptr,       // 347,
        nullptr,       // 348,
        nullptr,       // 349,
        nullptr,       // 350,
        nullptr,       // 351,
        nullptr,       // 352,
        nullptr,       // 353,
        nullptr,       // 354,
        nullptr,       // 355,
        nullptr,       // 356,
        nullptr,       // 357,
        nullptr,       // 358,
        nullptr,       // 359,
        nullptr,       // 360,
        nullptr,       // 361,
        nullptr,       // 362,
        nullptr,       // 363,
        nullptr,       // 364,
        nullptr,       // 365,
        nullptr,       // 366,
        nullptr,       // 367,
        nullptr,       // 368,
        nullptr,       // 369,
        nullptr,       // 370,
        nullptr,       // 371,
        nullptr,       // 372,
        nullptr,       // 373,
        nullptr,       // 374,
        nullptr,       // 375,
        nullptr,       // 376,
        nullptr,       // 377,
        nullptr,       // 378,
        nullptr,       // 379,
        nullptr,       // 380,
        nullptr,       // 381,
        nullptr,       // 382,
        nullptr,       // 383,
        nullptr,       // 384,
        nullptr,       // 385,
        nullptr,       // 386,
        nullptr,       // 387,
        nullptr,       // 388,
        nullptr,       // 389,
        nullptr,       // 390,
        nullptr,       // 391,
        nullptr,       // 392,
        nullptr,       // 393,
        nullptr,       // 394,
        nullptr,       // 395,
        nullptr,       // 396,
        nullptr,       // 397,
        nullptr,       // 398,
        nullptr,       // 399,
        nullptr,       // 400,
        nullptr,       // 401,
        nullptr,       // 402,
        nullptr,       // 403,
        nullptr,       // 404,
        nullptr,       // 405,
        nullptr,       // 406,
        nullptr,       // 407,
        nullptr,       // 408,
        nullptr,       // 409,
        nullptr,       // 410,
        nullptr,       // 411,
        nullptr,       // 412,
        nullptr,       // 413,
        nullptr,       // 414,
        nullptr,       // 415,
        nullptr,       // 416,
        nullptr,       // 417,
        nullptr,       // 418,
        nullptr,       // 419,
        nullptr,       // 420,
        nullptr,       // 421,
        nullptr,       // 422,
        nullptr,       // 423,
        nullptr,       // 424,
        nullptr,       // 425,
        nullptr,       // 426,
        nullptr,       // 427,
        nullptr,       // 428,
        nullptr,       // 429,
        nullptr,       // 430,
        nullptr,       // 431,
        nullptr,       // 432,
        nullptr,       // 433,
        nullptr,       // 434,
        nullptr,       // 435,
        nullptr,       // 436,
        nullptr,       // 437,
        nullptr,       // 438,
        nullptr,       // 439,
        nullptr,       // 440,
        &npcStruct441, // 441,
        nullptr,       // 442,
        &npcStruct443, // 443,
        nullptr,       // 444,
        &npcStruct445, // 445,
        nullptr,       // 446,
        &npcStruct447, // 447,
        nullptr,       // 448,
        nullptr,       // 449,
        nullptr,       // 450,
        &npcStruct451, // 451,
        nullptr,       // 452,
        nullptr,       // 453,
        nullptr,       // 454,
        nullptr,       // 455,
        nullptr,       // 456,
        nullptr,       // 457,
        nullptr,       // 458,
        nullptr,       // 459,
        nullptr,       // 460,
        nullptr,       // 461,
        nullptr,       // 462,
        nullptr,       // 463,
        nullptr,       // 464,
        nullptr,       // 465,
        &npcStruct466, // 466,
        &npcStruct467, // 467,
        nullptr,       // 468,
        nullptr,       // 469,
        nullptr,       // 470,
        &npcStruct471, // 471,
        &npcStruct472, // 472,
        nullptr,       // 473,
        &npcStruct474, // 474,
        &npcStruct475, // 475,
        nullptr,       // 476,
        &npcStruct477, // 477,
        &npcStruct478, // 478,
        nullptr,       // 479,
        &npcStruct480, // 480,
        &npcStruct481, // 481,
        nullptr,       // 482,
        nullptr,       // 483,
        nullptr,       // 484,
        nullptr,       // 485,
        nullptr,       // 486,
        nullptr,       // 487,
        nullptr,       // 488,
        nullptr,       // 489,
        nullptr,       // 490,
        nullptr,       // 491,
        nullptr,       // 492,
        nullptr,       // 493,
        nullptr,       // 494,
        nullptr,       // 495,
        &npcStruct496, // 496,
        &npcStruct497, // 497,
        nullptr,       // 498,
        nullptr,       // 499,
        nullptr,       // 500,
        nullptr,       // 501,
        nullptr,       // 502,
        nullptr,       // 503,
        nullptr,       // 504,
        &npcStruct505, // 505,
        nullptr,       // 506,
        &npcStruct507, // 507
        nullptr,       // 508
        nullptr,       // 509
        nullptr,       // 510
        nullptr,       // 511
        nullptr,       // 512
        nullptr,       // 513
        nullptr,       // 514
        nullptr,       // 515
        nullptr,       // 516
        nullptr,       // 517
        nullptr,       // 518
        nullptr,       // 519
        nullptr,       // 520
        nullptr,       // 521
        nullptr,       // 522
        nullptr,       // 523
        nullptr,       // 524
        nullptr,       // 525
        nullptr,       // 526
        nullptr,       // 527
        nullptr,       // 528
        nullptr,       // 529
        nullptr,       // 530
        &npcStruct531, // 531
        &npcStruct532, // 532
        &npcStruct533, // 533
        nullptr,       // 534
        &npcStruct535  // 535
    };

    DanNPCData **danNpcGetPtr()
    {
        return (DanNPCData **)DanNPC;
    }

    //
    struct DanLevelData
    {
        s32 lv1Limiter;
        s32 lv2Limiter;
        s32 lv3Limiter;
        s32 bonusMin;
        s32 bonusMax;
    };

    DanLevelData level1 = {45, 85, 100, 0, 0};
    DanLevelData level2 = {25, 40, 80, 0, 1};
    DanLevelData level3 = {10, 25, 60, 0, 2};
    DanLevelData level4 = {0, 15, 40, 1, 3};

    DanLevelData *danLevelDataPtr(s32 level)
    {
        switch (level)
        {
        case 1:
            return &level1;
            break;
        case 2:
            return &level2;
            break;
        case 3:
            return &level3;
            break;
        case 4:
            return &level4;
            break;
        }
    }
}