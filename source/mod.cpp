#include "mod.h"
#include "patch.h"

#include <common.h>
#include <util.h>
#include <cutscene_helpers.h>
#include <evtpatch.h>
#include <evt_cmd.h>
#include <spm/animdrv.h>
#include <spm/bgdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/eff_small_star.h>
#include <spm/eff_spm_confetti.h>
#include <spm/eff_zunbaba.h>
#include <spm/evt_cam.h>
#include <spm/evt_eff.h>
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
#include <spm/evt_sub.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_door.h>
#include <spm/fontmgr.h>
#include <spm/hitdrv.h>
#include <spm/hud.h>
#include <spm/itemdrv.h>
#include <spm/item_data.h>
#include <spm/lz_embedded.h>
#include <spm/map_data.h>
#include <spm/mapdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/mobjdrv.h>
#include <spm/memory.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <spm/parse.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <spm/seq_title.h>
#include <spm/spmario.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <spm/rel/dan.h>
#include <spm/rel/machi.h>
#include <wii/os/OSError.h>
#include <wii/cx.h>
#include <wii/gx.h>
#include <msl/math.h>
#include <msl/stdio.h>
#include <msl/string.h>
#include <cstdio>

namespace mod
{
    bool youSuck = false;

    // New pit entrance message
    const char *D100_entrance_01 =
        "<system>\n"
        "<col cfffffff>This is the entrance to a\n"
        "dangerous dungeon called the\n"
        "Pit of 100 Trials.\n"
        "<k>\n<p>\n"
        "Once you enter, it isn't easy\n"
        "to leave. Let me give you \n"
        "a few pointers.\n"
        "<k>\n<p>\n"
        "So first, you've got the\n"
        "<col c00000ff>Goombas.<col cfffffff><wait 2000> They're in there.\n"
        "<k>\n<p>\n"
        "Then you've got the... <wait 1000><col c00000ff>Koopas!!!\n"
        "<col cfffffff><wait 500>I've spent my whole life stompin'\n"
        "'em. They're really tough!\n"
        "<k>\n<p>\n"
        "Oh, then there's <col c00000ff>Shlurps.\n"
        "<col cfffffff><wait 250>They're really hard.<wait 250> Don't even\n"
        "try defeating those guys.\n"
        "<k>\n<p>\n"
        "<wait 250>... Unless you have some kind\n"
        "of <col c00000ff>bomb.<col cfffffff><wait 250> If it swallowed one,\n"
        "it'd probably die.\n"
        "<k>\n<p>\n"
        "But...\n"
        "<wait 500>what kind of cheater would\n"
        "bring <dynamic 3>bombs</dynamic> to the Pit?\n"
        "<k>\n<p>\n"
        "<shake>You know, back in my day,\n"
        "we didn't <wait 250><dynamic 3>HAVE</dynamic> any bombs.\n"
        "Got by fine without 'em.\n"
        "</shake><k>\n<p>\n"
        "<dynamic 3>Y'know what we called it?\n"
        "<wait 500><shake><col c00000ff>\"Boomerless Pit.\"\n"
        "<col cfffffff></shake></dynamic><k>\n<p>\n"
        "<wait 250>... Well, I don't know\n"
        "what a boomer is, but it\n"
        "sounds evil.<wait 250> And smelly.\n"
        "<k>\n<p>\n"
        "It also sounds a bit\n"
        "like 'bomb.'<wait 250> And I don't\n"
        "like that.<wait 250> Bombs are <shake>cheating.\n"
        "</shake><k>\n<p>\n"
        "<wait 250>Where was I again?\n"
        "<wait 500>Right. This generation always\n"
        "taking the easy way out.\n"
        "<k>\n<p>\n"
        "<wait 250>Excuse me for a second.\n"
        "<wait 1000><scale 0.67><shake>Ahem...\n"
        "</shake></scale><k>\n<p>\n"
        "<dynamic 3><scale 1.8><wait 500>SHAME<wait 500> ON<wait 500> YOU!!!\n"
        "<wait 1000></scale>\"Classic Pit\" runner!\n"
        "</dynamic>\n<k>\n<p>\n"
        "<wait 1000>So...<wait 500> do you still want\n"
        "to go in?\n"
        "<o>";

    const char *mac_kanban_003 =
        "<kanban>\n"
        "<center><col c00000ff>Flipside Pit of 100 Trials</center>\n"
        "</col><col c000fff0><center>Randomizer</center>\n"
        "</col><center>Discord: @tartt</center>\n"
        "<k>";

    const char *mac_kanban_004 =
        "<kanban>\n"
        "<center>Flopside's #1 Useless Pipe</center>\n"
        "<col c00000ff><center>IT'S USELESS! Try it out!</center>\n"
        "</col><center>-Flopside City Hall-</center>\n"
        "<k>";

    const char *D100_entrance_03 =
        "<system>\n"
        "I can't promise this will\n"
        "work correctly, but hey,\n"
        "whatever floats your boat.\n"
        "<o>";

    const char *modoridokan_D100_01 =
        "<system>\n"
        "Oh, it's you again. How's\n"
        "the Pit been treating you?\n"
        "<k>\n<p>\n"
        "Oh, what's that you're\n"
        "saying? \"Mr. Pipe, will you\n"
        "let me out, pleeeeeease?\"\n"
        "<k>\n<p>\n"
        "You know what you are?\n"
        "I know what you are. You're\n"
        "just a<dynamic 2> QUITTER.\n"
        "</dynamic><k>\n<p>\n"
        "You know what we used to\n"
        "do back in the day?\n"
        "<k>\n<p>\n"
        "Me and my buddies, we'd\n"
        "go to the Flipside Pit...\n"
        "EVERY day after school.\n"
        "<k>\n<p>\n"
        "Well, that part's a lie,\n"
        "I've never been to a school.\n"
        "But the point is...\n"
        "<k>\n<p>\n"
        "...we'd go to the Pit,\n"
        "and we'd exit from the\n"
        "100th Trial. Every time.\n"
        "<k>\n<p>\n"
        "And you know what?\n"
        "We did it without any of\n"
        "your darned Pixls.\n"
        "<k>\n<p>\n"
        "None of those \"items\" or all\n"
        "that other hullabaloo. Just\n"
        "us and our Ultra Hammers.\n"
        "<k>\n<p>\n"
        "You youngsters just don't\n"
        "know when to quit or when\n"
        "to keep going...\n"
        "<k>\n<p>\n"
        "You do know that if you leave\n"
        "now, I won't just put you\n"
        "back here, right?\n"
        "<k>\n<p>\n"
        "Yeah, that's right. You leave\n"
        "now, and you get to start\n"
        "ALL over again.\n"
        "<k>\n<p>\n"
        "<dynamic 1>\"Oh, but that's so cruel!\n"
        "What kind of questionable\n"
        "game design is this?\"\n"
        "</dynamic><k>\n<p>\n"
        "I'll tell you what kind of\n"
        "\"game design\" it is.\n"
        "<shake>Good and old-fashioned.\n"
        "</shake><k>\n<p>\n"
        "You children think this is\n"
        "a game? You're supposed to\n"
        "be saving the world!\n"
        "<k>\n<p>\n"
        "Yeah, I went there.\n"
        "I can see the headlines\n"
        "already...\n"
        "<k>\n<p>\n"
        "<dynamic 3>\"Local Hero of the Light\n"
        "Prognosticus: Abandoning All\n"
        "Worlds, Playing in a Pit?!\"\n"
        "</dynamic><k>\n<p>\n"
        "I mean, here I was, trying\n"
        "my best not to judge you.\n"
        "But I can't help myself!\n"
        "<k>\n<p>\n"
        "You're the Hero, and you\n"
        "can't even handle 100\n"
        "simulated battles?\n"
        "<k>\n<p>\n"
        "I thought I'd gotten this\n"
        "through to you the first\n"
        "time, but...<wait 1000><scale 0.67><shake>  Ahem...\n"
        "</shake></scale><k>\n<p>\n"
        "<dynamic 3><scale 1.8><wait 500>SHAME<wait 500> ON<wait 500> YOU!!!\n"
        "<wait 1000></scale>\nUseless quitter!\n"
        "</dynamic>\n<k>\n<p>\n"
        "... Well, I suppose it's fine to\n"
        "let you go and save all the\n"
        "worlds now.\n"
        "<k>\n<p>\n"
        "Do you want to leave the\n"
        "Flipside Pit of 100 Trials?\n"
        // "<wait 5000>Quitter!!!!!!!!\n"
        "<o>";

    static double boobies = 0;

    // Create arrays for enemy generation, separated by difficulty class

    // Goomba, Flip Goomba, Paragoomba, Green Koopa, Glassesless Koopa, Red Koopa, Flip Red Koopa, Green Paratroopa, Glassesless Paratroopa, Buzzy Beetle, Stone Buzzy, Boo, Green Cheep Cheep,
    // Red Cheep Cheep, Flip Cheep Cheep, Bald Cleft, Squiglet, Squig, Sproing-Oing, Boing-Oing, Boomboxer, Growmeba, Jellien, Slow Cursya, Cherbil, Poison Cherbil, Frackle, Spinia, Flip Buzzy, Dark Puff, Jawbus
    int lv1Names[] = {1, 2, 8, 12, 13, 15, 16, 21, 22, 26, 33, 85, 89, 90, 91, 100, 126, 127, 135, 137, 143, 160, 179, 246, 441, 443, 451, 496, 471, 112, 169};
    int lv1Odds[] = {70, 65, 60, 35, 35, 45, 35, 35, 30, 20, 45, 73, 50, 40, 20, 40, 60, 61, 55, 55, 60, 65, 40, 60, 65, 70, 60, 40, 25, 70, 50};
    int lv1SpR[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    int lv1Min[] = {4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1};
    int lv1Max[] = {7, 6, 5, 5, 5, 5, 5, 4, 4, 5, 6, 6, 4, 4, 3, 6, 6, 6, 5, 4, 5, 3, 5, 5, 4, 5, 4, 6, 5, 6, 3};

    // Gloomba, Spiked Goomba, Spiked Gloomba, Red Paratroopa, Flip Red Paratroopa, Spike Top, Parabuzzy, Spiny, Flip Spiny, Hammer Bro, Magikoopa, Fuzzy, Pokey,
    // Spania, Chain Chomp, Squog, Beepboxer, Mister I, Shlurp, Tileoid G, Tileoid B, Rawbus, PatrolMeow, Longator, Barribad, Pigarithm, 3x Floro Sapiens, Ninjoe,
    // Tech Cursya, Heavy Cursya, Reversya Cursya, Ice Cherbil, Flip Spike Top, Moon Cleft, Kilo Muth, Bleepboxer
    int lv2Names[] = {4, 5, 6, 23, 24, 28, 31, 37, 38, 46, 64, 97, 103, 114, 123, 129, 145, 151, 157, 164, 165, 170, 174, 189, 195, 201, 206, 208, 210, 212, 248, 249, 250, 445, 472, 101, 507, 505};
    int lv2Odds[] = {85, 60, 50, 40, 40, 40, 25, 70, 70, 80, 70, 80, 80, 85, 70, 90, 75, 50, 20, 40, 40, 20, 85, 80, 15, 64, 30, 30, 30, 40, 30, 25, 35, 75, 30, 75, 32, 69};
    int lv2SpR[] = {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv2Min[] = {4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 2, 3, 1, 3, 3, 1, 2, 2, 2, 1, 3, 3, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 2};
    int lv2Max[] = {6, 6, 5, 5, 4, 5, 4, 6, 6, 4, 5, 6, 4, 6, 3, 6, 5, 3, 4, 4, 4, 3, 5, 6, 2, 5, 5, 5, 5, 4, 4, 4, 4, 4, 5, 5, 2, 5};

    // Headbonk Goomba, Koopatrol, Spiky Parabuzzy, Dry Bones, Flip Hammer Bro, Boomerang Bro, Fire Bro, Broom Magikoopa, Koopa Striker, Bill Blaster, Dark Boo, Clubba, Pink Fuzzy,
    // Hyper Cleft, Ruff Puff, Crazee Dayzee, Zoing-Oing, Blomeba, Tileoid R, Tileoid Y, BombMeow, Foton, Hooligon, Copta, Hogarithm, Ninjohn, Flip Skellobit, Flip Skellobomber,
    // Muth, Spunia, Hyper Goomba, Hyper Spiked Goomba, Hyper Paragoomba, Dark Koopa, Dark Paratroopa, Flip Boomerang Bro, Flip Fire Bro, Soopa Striker, Green Fuzzy, Ice Bro, Red Magikoopa,
    // Red Broom Magikoopa, White Magikoopa, White Broom Magikoopa, Green Magikoopa, Green Broom Magikoopa
    int lv3Names[] = {10, 19, 32, 40, 40, 40, 40, 65, 70, 84, 86, 95, 98, 102, 110, 116, 139, 161, 166, 167, 178, 180, 185, 193, 202, 215, 225, 231, 258, 497, 3, 7, 9, 17, 25, 54, 60, 74, 99, 62, 480, 481, 477, 478, 474, 475};
    int lv3Odds[] = {80, 35, 30, 55, 70, 75, 75, 55, 65, 30, 70, 75, 65, 35, 75, 69, 55, 60, 40, 40, 10, 30, 50, 60, 40, 40, 50, 30, 10, 75, 30, 30, 30, 20, 25, 30, 30, 50, 35, 50, 20, 20, 20, 20, 20, 20};
    int lv3SpR[] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv3Min[] = {4, 2, 2, 3, 3, 3, 3, 2, 2, 2, 3, 3, 4, 3, 3, 3, 3, 1, 2, 2, 2, 2, 2, 3, 2, 2, 3, 3, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2};
    int lv3Max[] = {6, 5, 4, 5, 4, 4, 4, 4, 5, 4, 6, 5, 6, 5, 6, 5, 5, 3, 4, 4, 5, 4, 3, 5, 4, 4, 6, 5, 1, 6, 6, 6, 6, 5, 5, 5, 5, 4, 8, 5, 4, 4, 4, 4, 4, 4};

    // Lakitu, Flip Lakitu, Dull Bones, Poison Pokey, Amazee Dayzee, Red Chomp, Squoinker, Blastboxer, Red I, Shlorp, Chromeba, Gawbus, Longadile, Sobarribad, Ninjerry,
    // Flip Spiky Skellobit, Flip Skellobomber w/ Skellobait, 3x Magiblots, Back Cursya, Gigabyte, Mega Muth, Shady Koopa, Flip Shady Koopa, Ash Cherbil, Sky-Blue Spiny,
    // Red Spike Top, Dark Bones, White Clubba, Shady Magikoopa, Shady Broom Magikoopa, Tileoid PU, Ninjeremiah, Dark Koopatrol, Kamikaze Goomba,
    // Skellobyte, Spiky Skellobyte, Shady Hammer Bro, Shady Boomerang Bro, Shady Striker
    int lv4Names[] = {35, 36, 42, 105, 118, 124, 131, 147, 153, 158, 162, 171, 190, 197, 218, 228, 234, 238, 240, 242, 247, 256, 259, 466, 467, 447, 39, 30, 44, 96, 67, 68, 168, 221, 20, 11, 226, 229, 50, 56, 76};
    int lv4Odds[] = {60, 50, 85, 80, 20, 30, 80, 80, 40, 20, 70, 30, 50, 20, 40, 50, 30, 40, 40, 40, 4, 10, 10, 60, 60, 73, 75, 70, 60, 70, 66, 44, 74, 40, 30, 70, 40, 40, 60, 60, 60};
    int lv4SpR[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv4Min[] = {1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 1, 2, 1, 1, 2, 2, 1, 3, 2, 2, 2, 2, 2};
    int lv4Max[] = {2, 2, 5, 5, 2, 3, 6, 5, 2, 2, 3, 2, 5, 1, 4, 4, 4, 5, 5, 5, 4, 2, 2, 5, 5, 5, 6, 6, 3, 5, 4, 3, 4, 4, 4, 5, 5, 5, 4, 4, 5};

    //   int SpR2Doors[] = {3, 6, 12, 13, 19, 22};
    //   int SpR1Doors[] = {27, 28, 29, 30};

    int enemyConfigArray[2001] = {169};

    // Pit Flimm item pool to iterate from
    int rotenShopItemPool[] = {67, 68, 69, 70, 75, 81, 82, 84, 88, 89, 90, 99, 106, 118, 123, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
                               141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160, 161, 162, 163, 164, 166, 168, 169, 170,
                               171, 173, 174, 177, 178, 180, 181, 182, 183, 184, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 198, 199, 200,
                               201, 202, 203, 204, 205, 207, 208, 209, 211, 212, 214, 215, 288, 292, 298, 299, 301, 302, 305, 307, 308, 310, 312,
                               313, 321, 323, 328, 329, 332, 335, 341, 343, 346, 355, 358, 360, 363, 366, 372, 377, 383, 389, 393, 396, 398, 399, 400, 401, 409, 412, 415, 418,
                               424, 426, 428, 433, 432, 436, 437, 440, 444, 446, 447, 448, 0x11c, 0x11e, 0x121, 0x122, 0x125, 0x126, 0x129, 0x12c, 0x132, 0x135, 0x13a, 0x13b, 0x13c,
                               0x13e, 0x142, 0x144, 0x14d, 0x156, 0x167, 0x16c, 0x184, 0x192, 0x1b2, 0x1b6, 0x1b7};

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

    s32 no = 0;
    s32 i = 0;
    int currentFloor = 0;
    int nextFloor = 0;
    int enemyArrayOffset = 0;
    int nextEnemyCount = 0;
    int nextName1 = 0;
    int nextNum1 = 0;
    int nextPos1 = 0;
    int nextName2 = 0;
    int nextNum2 = 0;
    int nextPos2 = 0;
    int nextName3 = 0;
    int nextNum3 = 0;
    int nextPos3 = 0;

    /*
    // Define enemy defenses
    spm::npcdrv::NPCDefense createDef(int type, int defense, int flags)
    {
        spm::npcdrv::NPCDefense def;
        def.type = type;
        def.defense = defense;
        def.flags = flags;
        return def;
    }
    spm::npcdrv::NPCDefense def1_1 = createDef(0x1, 0x1, 0x0);
    spm::npcdrv::NPCDefense def1_2 = createDef(0x1, 0x2, 0x0);
    spm::npcdrv::NPCDefense def1_3 = createDef(0x1, 0x3, 0x0);
    spm::npcdrv::NPCDefense def1_0 = createDef(0x1, 0x0, 0x0);
    */

    // Add/reduce damage to certain enemies; later, maybe set DEFs for enemies that could actually use it and don't override all defenses with Fucking damage reduction.
    // There are certainly a few in this array that can use DEFs, DR is just easier for me right now.
    int (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
    void patchMarioDamage()
    {
        marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy,
                                                     [](s32 damageType, s32 tribeId)
                                                     {
                                                         // Kilo Muth, DEF 3 -> 1
                                                         // Ice Bro, DEF 3 --> 1
                                                         // Dark Koopatrol, DEF 8 --> 6
                                                         // Red Spike Top, DEF 6 --> 4
                                                         // Sky-Blue Spiny, DEF 6 --> 4
                                                         if (tribeId == 506 || tribeId == 61 || tribeId == 19 || tribeId == 29 || tribeId == 38)
                                                         {
                                                             int damage = marioCalcDamageToEnemy(damageType, tribeId);
                                                             damage += 2;
                                                             if (damage < 0)
                                                             {
                                                                 damage = 0;
                                                             }
                                                             return damage;
                                                         }
                                                         // Spunia, DEF 0 --> 1
                                                         else if (tribeId == 496)
                                                         {
                                                             int damage = marioCalcDamageToEnemy(damageType, tribeId);
                                                             damage -= 1;
                                                             if (damage < 0)
                                                             {
                                                                 damage = 0;
                                                             }
                                                             return damage;
                                                         }
                                                         // Shady Bros, DEF 3 --> 2
                                                         if (tribeId == 61 || tribeId == 55 || tribeId == 49)
                                                         {
                                                             int damage = marioCalcDamageToEnemy(damageType, tribeId);
                                                             damage += 1;
                                                             if (damage < 0)
                                                             {
                                                                 damage = 0;
                                                             }
                                                             return damage;
                                                         }
                                                         // Green (Broom) Magikoopa, DEF 0 --> 3
                                                         // (Flip) Shady Koopa, DEF 0 --> 3
                                                         else if (tribeId == 473 || tribeId == 474 || tribeId == 465 || tribeId == 466)
                                                         {
                                                             int damage = marioCalcDamageToEnemy(damageType, tribeId);
                                                             damage -= 3;
                                                             if (damage < 0)
                                                             {
                                                                 damage = 0;
                                                             }
                                                             return damage;
                                                         }
                                                         return marioCalcDamageToEnemy(damageType, tribeId);
                                                     });
    }

    // Toggle below to "true" to enable on-screen displays for current enemies in a Pit room / enemies in the next room / GSW1. Will display in pit rooms 2-98.
    // This feature is not complete or stable. Leaving the pit early may cause the game to crash.
    bool displayDebugValues = false;

    s32 evt_dan_read_data_new(spm::evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)entry;

        // Allocate work on first run
        // (check is a relD leftover, this only runs once on retail anyway)
        if (isFirstCall)
        {
            spm::dan::dan_wp = (spm::dan::DanWork *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, sizeof(spm::dan::DanWork));
            msl::string::memset(spm::dan::dan_wp, 0, sizeof(*spm::dan::dan_wp));
            spm::dan::dan_wp->dungeons = (spm::dan::DanDungeon *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, sizeof(spm::dan::DanDungeon[DUNGEON_MAX]));
            msl::string::memset(spm::dan::dan_wp->dungeons, 0, sizeof(spm::dan::DanDungeon[DUNGEON_MAX]));
        }

        // Reset Pit chests
        spm::swdrv::swClear(433);
        spm::swdrv::swClear(434);
        spm::swdrv::swClear(435);
        spm::swdrv::swClear(436);
        spm::swdrv::swClear(437);
        spm::swdrv::swClear(438);
        spm::swdrv::swClear(439);
        spm::swdrv::swClear(440);
        spm::swdrv::swClear(441);
        spm::swdrv::swClear(442);
        spm::swdrv::swClear(443);
        spm::swdrv::swClear(444);
        spm::swdrv::swClear(445);
        spm::swdrv::swClear(446);
        spm::swdrv::swClear(447);
        spm::swdrv::swClear(448);
        spm::swdrv::swClear(449);
        spm::swdrv::swClear(450);
        bool nipples = spm::swdrv::swGet(409);

        // Prepare pit text to be read
        /*    u32 size = wii::cx::CXGetCompressionHeader(spm::lz_embedded::pitText).decompSize;
            char *decompPitText = (char *)spm::memory::__memAlloc(0, size);
            wii::cx::CXUncompressLZ(spm::lz_embedded::pitText, decompPitText);
            spm::parse::parseInit(decompPitText, size); */
        i = 0;
        currentFloor = spm::swdrv::swByteGet(1);
        int shadooEntries = spm::swdrv::swByteGet(24);

        // Global room generation RNG value (1-100, 5 decimal places)
        double thighs = spm::system::rand() % 99 + 1;
        double divisor = 10;
        double ass = (spm::system::rand() % 10) / divisor;
        divisor = 100;
        double booty = (spm::system::rand() % 10) / divisor;
        divisor = 1000;
        double butt = (spm::system::rand() % 10) / divisor;
        divisor = 10000;
        double bottom = (spm::system::rand() % 10) / divisor;
        divisor = 100000;
        double buttocks = (spm::system::rand() % 10) / divisor;
        boobies = (thighs + ass + booty + butt + bottom + buttocks);

        // Check if room 48, then shove you into Flopside Pit
        if (currentFloor == 48)
        {
            spm::swdrv::swByteSet(1, 148);
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
            spm::swdrv::swByteSet(24, 1);
        }

        // Start pit room randomization process
        // Establish default doors + other vars
        int activeDoorCount = 19; // Actually 20, but let's not complicate things
        int doorOn[31] = {2, 3, 6, 7, 9, 12, 13, 16, 18, 19, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32};
        int roomDecCode = 0;
        int segmentCount = 0;
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

        // Determine # of segments to generate for current room
        int segmentRNG = spm::system::rand() % 152;
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
        else if (segmentRNG >= 122 && segmentRNG < 126)
        {
            segmentCount = 0;
        }
        else
        {
            segmentCount = 5;
        }
        wii::os::OSReport("Queueing %d segment generations for this room.\n", segmentCount);
        i = 0;

        // Enclosed structure generation. If more than 2 segments are generated at any time, the remainder is subtracted from segmentCount
        int structureRNG = spm::system::rand() % 180;
        int structureMiscRNG = spm::system::rand() % 100;
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
            int segmentID = spm::system::rand() % 16;
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
                    (segment20 && segment8000) ||
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
            int pipeRNG = spm::system::rand() % 133;
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
            //    marioMaxHp = spm::mario_pouch::MarioPouchWork::maxHp;
        }

        // Doors
        int doorEntranceArrayVal = spm::system::rand() % activeDoorCount;
        int doorEntrance = doorOn[doorEntranceArrayVal];
        int doorExitArrayVal = spm::system::rand() % activeDoorCount;
        while (doorExitArrayVal == doorEntranceArrayVal)
        {
            doorExitArrayVal = spm::system::rand() % activeDoorCount;
        }
        int doorExit = doorOn[doorExitArrayVal];

        i = 0;

        // Item Substitution Protocol
        double floorwiseMultiplier = 1;
        switch (currentFloor)
        {
        case 9:
            floorwiseMultiplier = 1.0;
            break;
        case 19:
            floorwiseMultiplier = 0.85;
            break;
        case 29:
            floorwiseMultiplier = 0.7;
            break;
        case 39:
            floorwiseMultiplier = 0.5;
            break;
        case 149:
            floorwiseMultiplier = 0.25;
            break;
        case 159:
            floorwiseMultiplier = 0;
            break;
        case 169:
            floorwiseMultiplier = -0.3;
            break;
        case 179:
            floorwiseMultiplier = -0.5;
            break;
        case 189:
            floorwiseMultiplier = -0.75;
            break;
        }

        // Establishing reward weights
        double ShroomShake = msl::math::pow(400, floorwiseMultiplier);
        double FireBurst = msl::math::pow(350, floorwiseMultiplier);
        double StarMedal = msl::math::pow(314, floorwiseMultiplier);
        double BigEgg = msl::math::pow(280, floorwiseMultiplier);
        double ShellShock = msl::math::pow(240, floorwiseMultiplier);
        double FreshPastaBunch = msl::math::pow(220, floorwiseMultiplier);
        double FriedShroomPlate = msl::math::pow(210, floorwiseMultiplier);
        double HoneySuper = msl::math::pow(160, floorwiseMultiplier);
        double SapSyrup = msl::math::pow(150, floorwiseMultiplier);
        double IceStorm = msl::math::pow(140, floorwiseMultiplier);
        double CocoaBean = msl::math::pow(120, floorwiseMultiplier);
        double KeelMango = msl::math::pow(110, floorwiseMultiplier);
        double MysteryBox = msl::math::pow(101, floorwiseMultiplier);
        double EmergencyRation = msl::math::pow(90, floorwiseMultiplier);
        double HotDog = msl::math::pow(80, floorwiseMultiplier);
        double HerbTea = msl::math::pow(70, floorwiseMultiplier);
        double KoopaTea = msl::math::pow(70, floorwiseMultiplier);
        double BlockBlock = msl::math::pow(64, floorwiseMultiplier);
        double PowBlock = msl::math::pow(60, floorwiseMultiplier);
        double ThunderRage = msl::math::pow(55, floorwiseMultiplier);
        double MightyTonic = msl::math::pow(50, floorwiseMultiplier);
        double RoastedShroomDish = msl::math::pow(40, floorwiseMultiplier);
        double ShootingStar = msl::math::pow(30, floorwiseMultiplier);
        double LifeShroom = msl::math::pow(30, floorwiseMultiplier);
        double MeteorMeal = msl::math::pow(25, floorwiseMultiplier);
        double GoldMedal = msl::math::pow(20, floorwiseMultiplier);
        double EggBomb = msl::math::pow(16, floorwiseMultiplier);
        double UltraShroomShake = msl::math::pow(15, floorwiseMultiplier);
        double DyllisDeluxe = msl::math::pow(5, floorwiseMultiplier);
        double TrialStew = .1;
        double sumOfLoot = (ShroomShake + FireBurst + StarMedal + BigEgg + ShellShock + FriedShroomPlate + HoneySuper + IceStorm + MysteryBox + BlockBlock + ThunderRage + MightyTonic + RoastedShroomDish + ShootingStar + LifeShroom + MeteorMeal + GoldMedal + UltraShroomShake + DyllisDeluxe + FreshPastaBunch + SapSyrup + CocoaBean + KeelMango + EmergencyRation + HotDog + PowBlock + EggBomb + HerbTea + KoopaTea);

        // Adjusting weights against floor number and normalizing them to add up to 99.9
        double SShake = (ShroomShake / (sumOfLoot) * 99.9);
        double FBurst = (FireBurst / (sumOfLoot) * 99.9);
        double SMedal = (StarMedal / (sumOfLoot) * 99.9);
        double BEgg = (BigEgg / (sumOfLoot) * 99.9);
        double SShock = (ShellShock / (sumOfLoot) * 99.9);
        double FSPlate = (FriedShroomPlate / (sumOfLoot) * 99.9);
        double HSuper = (HoneySuper / (sumOfLoot) * 99.9);
        double IStorm = (IceStorm / (sumOfLoot) * 99.9);
        double MBox = (MysteryBox / (sumOfLoot) * 99.9);
        double BBlock = (BlockBlock / (sumOfLoot) * 99.9);
        double TRage = (ThunderRage / (sumOfLoot) * 99.9);
        double MTonic = (MightyTonic / (sumOfLoot) * 99.9);
        double RSDish = (RoastedShroomDish / (sumOfLoot) * 99.9);
        double SStar = (ShootingStar / (sumOfLoot) * 99.9);
        double LShroom = (LifeShroom / (sumOfLoot) * 99.9);
        double MMeal = (MeteorMeal / (sumOfLoot) * 99.9);
        double GMedal = (GoldMedal / (sumOfLoot) * 99.9);
        double USShake = (UltraShroomShake / (sumOfLoot) * 99.9);
        double EBomb = (EggBomb / (sumOfLoot) * 99.9);
        double PBlock = (PowBlock / (sumOfLoot) * 99.9);
        double HDog = (HotDog / (sumOfLoot) * 99.9);
        double ERation = (EmergencyRation / (sumOfLoot) * 99.9);
        double KMango = (KeelMango / (sumOfLoot) * 99.9);
        double CBean = (CocoaBean / (sumOfLoot) * 99.9);
        double SSyrup = (SapSyrup / (sumOfLoot) * 99.9);
        double FPBunch = (FreshPastaBunch / (sumOfLoot) * 99.9);
        double HTea = (HerbTea / (sumOfLoot) * 99.9);
        double KTea = (KoopaTea / (sumOfLoot) * 99.9);
        double DDeluxe = (DyllisDeluxe / (sumOfLoot) * 99.9);

        // Establishing selection ranges for each item -- these should be perfectly 1-100 like boobies.
        double F_TStew = (100 - TrialStew);
        double F_SShake = (F_TStew - SShake);
        double F_FBurst = (F_SShake - FBurst);
        double F_SMedal = (F_FBurst - SMedal);
        double F_BEgg = (F_SMedal - BEgg);
        double F_SShock = (F_BEgg - SShock);
        double F_FSPlate = (F_SShock - FSPlate);
        double F_HSuper = (F_FSPlate - HSuper);
        double F_IStorm = (F_HSuper - IStorm);
        double F_MBox = (F_IStorm - MBox);
        double F_BBlock = (F_MBox - BBlock);
        double F_TRage = (F_BBlock - TRage);
        double F_MTonic = (F_TRage - MTonic);
        double F_RSDish = (F_MTonic - RSDish);
        double F_SStar = (F_RSDish - SStar);
        double F_LShroom = (F_SStar - LShroom);
        double F_MMeal = (F_LShroom - MMeal);
        double F_GMedal = (F_MMeal - GMedal);
        double F_USShake = (F_GMedal - USShake);
        double F_EBomb = (F_USShake - EBomb);
        double F_PBlock = (F_EBomb - PBlock);
        double F_HDog = (F_PBlock - HDog);
        double F_ERation = (F_HDog - ERation);
        double F_KMango = (F_ERation - KMango);
        double F_CBean = (F_KMango - CBean);
        double F_SSyrup = (F_CBean - SSyrup);
        double F_FPBunch = (F_SSyrup - FPBunch);
        double F_HTea = (F_FPBunch - HTea);
        double F_KTea = (F_HTea - KTea);
        double F_DDeluxe = (F_USShake - DDeluxe);

        // The rabbit is landing
        if (nipples == 0)
        {
            if (boobies >= F_TStew)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
            else if (boobies >= F_SShake)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HUNNY_KINOKO");
            else if (boobies >= F_FBurst)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_HONOO_SAKURETU");
            else if (boobies >= F_SMedal)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_STAR_MEDAL");
            else if (boobies >= F_BEgg)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_BIG_EGG");
            else if (boobies >= F_SShock)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KOURA_DE_PON");
            else if (boobies >= F_FSPlate)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_KINOKO_FRY");
            else if (boobies >= F_HSuper)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HUNNY_KINOKO_S");
            else if (boobies >= F_IStorm)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KOORI_NO_IBUKI");
            else if (boobies >= F_MBox)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_NANIGA_OKORUKANA");
            else if (boobies >= F_BBlock)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_BARIA_FRAME");
            else if (boobies >= F_TRage)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KAMINARI_DOKKAN");
            else if (boobies >= F_MTonic)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_TUYOTUYO_DRINK");
            else if (boobies >= F_RSDish)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_KINOKO_HOILE_FRY");
            else if (boobies >= F_SStar)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KIRAKIRA_OTOSHI");
            else if (boobies >= F_LShroom)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KINKYU_KINOKO");
            else if (boobies >= F_MMeal)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_DINNER");
            else if (boobies >= F_GMedal)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_GOLD_MEDAL");
            else if (boobies >= F_USShake)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_ULTRA_DRINK");
            else if (boobies >= F_EBomb)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_BOMB_EGG");
            else if (boobies >= F_PBlock)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_POW_BLOCK");
            else if (boobies >= F_HDog)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HOTDOG");
            else if (boobies >= F_ERation)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_EMERGENCY_MEAL");
            else if (boobies >= F_KMango)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_TOROPICO_MANGO");
            else if (boobies >= F_CBean)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_MILD_CACAO");
            else if (boobies >= F_SSyrup)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HANAJIRU_SYRUP");
            else if (boobies >= F_FPBunch)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_ROW_PASTA");
            else if (boobies >= F_HTea)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HERB_TEA");
            else if (boobies >= F_KTea)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_KAME_TEA");
            else if (boobies >= F_DDeluxe)
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_DOROCY_DELUX");
            else
                spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
        }
        else
        {
            spm::dan::dan_wp->dungeons[currentFloor].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
        }

        // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
        spm::dan::dan_wp->dungeons[currentFloor].map = roomDecCode;

        i = 0;
        // Door substitution
        spm::dan::dan_wp->dungeons[currentFloor].doors[i].enter = doorEntrance;
        spm::dan::dan_wp->dungeons[currentFloor].doors[i].exit = doorExit;
        spm::dan::dan_wp->dungeons[currentFloor].doorCount = 1;

        if (currentFloor == 0)
        {
            int enemyArrayVal = 1;
            int funnyVal = 0;
            for (int roomGens = 0; roomGens <= 199; roomGens = roomGens + 1)
            {
                // Enemy generation setup
                i = 0;
                int enemyTypes = 0;
                int arrayRNG = 0;
                int enemyAmt = 0;
                int enemyDifference = 0;
                int enemyMin = 0;
                int enemyMax = 0;
                //  int spDoorRNG = 0;
                int vsOdds = 0;
                int enemyGenRNG = spm::system::rand() % 100;
                if (enemyGenRNG <= 60)
                {
                    enemyTypes = 2;
                }
                else if (enemyGenRNG <= 85)
                {
                    enemyTypes = 1;
                }
                else
                {
                    enemyTypes = 3;
                }
                enemyConfigArray[enemyArrayVal] = enemyTypes;
                enemyArrayVal = enemyArrayVal + 1;
                // Enemy Substition Protocol
                while (i < enemyTypes)
                {
                    // Lv1
                    if (roomGens <= 24)
                    {
                        enemyGenRNG = spm::system::rand() % 100;
                        if (enemyGenRNG < 75)
                        {
                            arrayRNG = spm::system::rand() % 31;
                            vsOdds = spm::system::rand() % 100;
                            if (lv1Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv1Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv1Max[arrayRNG] - lv1Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv1Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv1Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv1Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv1Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
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
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv1SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv1SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //    spDoorRNG = spm::system::rand() % 6;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 95)
                        {
                            arrayRNG = spm::system::rand() % 38;
                            vsOdds = spm::system::rand() % 100;
                            if (lv2Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv2Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv2Max[arrayRNG] - lv2Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv2Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv2Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv2Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv2Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                    if (lv2Names[arrayRNG] == 195 || lv2Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
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
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv2SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv2SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //    spDoorRNG = spm::system::rand() % 6;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else
                        {
                            arrayRNG = spm::system::rand() % 46;
                            vsOdds = spm::system::rand() % 100;
                            if (lv3Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv3Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv3Max[arrayRNG] - lv3Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv3Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv3Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv3Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv3Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
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
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv3SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv3SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //    spDoorRNG = spm::system::rand() % 6;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                    }
                    // Lv2
                    else if (roomGens <= 149)
                    {
                        enemyGenRNG = spm::system::rand() % 100;
                        if (enemyGenRNG < 20)
                        {
                            arrayRNG = spm::system::rand() % 31;
                            vsOdds = spm::system::rand() % 100;
                            if (lv1Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv1Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv1Max[arrayRNG] - lv1Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv1Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv1Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv1Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv1Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
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
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv1SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv1SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //    spDoorRNG = spm::system::rand() % 6;
                                    //   spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 85)
                        {
                            arrayRNG = spm::system::rand() % 38;
                            vsOdds = spm::system::rand() % 100;
                            if (lv2Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv2Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv2Max[arrayRNG] - lv2Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv2Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv2Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv2Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv2Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                    if (lv2Names[arrayRNG] == 195 || lv2Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
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
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv2SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv2SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //   spDoorRNG = spm::system::rand() % 6;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else
                        {
                            arrayRNG = spm::system::rand() % 46;
                            vsOdds = spm::system::rand() % 100;
                            if (lv3Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv3Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv3Max[arrayRNG] - lv3Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv3Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv3Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv3Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv3Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
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
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv3SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv3SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                    }
                    // Lv3
                    else if (roomGens <= 174)
                    {
                        enemyGenRNG = spm::system::rand() % 100;
                        if (enemyGenRNG < 15)
                        {
                            arrayRNG = spm::system::rand() % 38;
                            vsOdds = spm::system::rand() % 100;
                            if (lv2Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv2Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv2Max[arrayRNG] - lv2Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv2Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv2Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv2Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv2Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                    if (lv2Names[arrayRNG] == 195 || lv2Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv2SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv2SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 80)
                        {
                            arrayRNG = spm::system::rand() % 46;
                            vsOdds = spm::system::rand() % 100;
                            if (lv3Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv3Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv3Max[arrayRNG] - lv3Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv3Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv3Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv3Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv3Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv3SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv3SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else
                        {
                            arrayRNG = spm::system::rand() % 41;
                            vsOdds = spm::system::rand() % 100;
                            if (lv4Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv4Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv4Max[arrayRNG] - lv4Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv4Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv4Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv4Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv4Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv4Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv4Min[arrayRNG];
                                    if (lv4Names[arrayRNG] == 195 || lv4Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv4SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv4SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                    }
                    // Lv4
                    else
                    {
                        enemyGenRNG = spm::system::rand() % 100;
                        if (enemyGenRNG < 5)
                        {
                            arrayRNG = spm::system::rand() % 31;
                            vsOdds = spm::system::rand() % 100;
                            if (lv1Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv1Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv1Max[arrayRNG] - lv1Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv1Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv1Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv1Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv1Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv1SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv1SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 16)
                        {
                            arrayRNG = spm::system::rand() % 38;
                            vsOdds = spm::system::rand() % 100;
                            if (lv2Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv2Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv2Max[arrayRNG] - lv2Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv2Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv2Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv2Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv2Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                    if (lv2Names[arrayRNG] == 195 || lv2Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv2SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv2SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 50)
                        {
                            arrayRNG = spm::system::rand() % 46;
                            vsOdds = spm::system::rand() % 100;
                            if (lv3Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv3Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv3Max[arrayRNG] - lv3Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv3Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv3Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv3Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv3Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv3SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv3SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else
                        {
                            arrayRNG = spm::system::rand() % 41;
                            vsOdds = spm::system::rand() % 100;
                            if (lv4Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv4Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv4Max[arrayRNG] - lv4Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv4Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv4Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv4Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv4Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv4Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv4Min[arrayRNG];
                                    if (lv4Names[arrayRNG] == 195 || lv4Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv4SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv4SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                    }
                }
                // Fill empty slots if enemy types < 3
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
        enemyArrayOffset = (currentFloor * 10 + 1);
        spm::dan::dan_wp->dungeons[currentFloor].enemyCount = enemyConfigArray[enemyArrayOffset];
        enemyArrayOffset = enemyArrayOffset + 1;
        spm::dan::dan_wp->dungeons[currentFloor].enemies[1].name = enemyConfigArray[enemyArrayOffset];
        enemyArrayOffset = enemyArrayOffset + 1;
        spm::dan::dan_wp->dungeons[currentFloor].enemies[1].num = enemyConfigArray[enemyArrayOffset];
        enemyArrayOffset = enemyArrayOffset + 1;
        spm::dan::dan_wp->dungeons[currentFloor].enemies[1].pos = enemyConfigArray[enemyArrayOffset];
        enemyArrayOffset = enemyArrayOffset + 1;
        wii::os::OSReport("enemyCount for current floor #%d: %d.\n", currentFloor, spm::dan::dan_wp->dungeons[currentFloor].enemyCount);
        if (enemyConfigArray[enemyArrayOffset] != 0)
        {
            spm::dan::dan_wp->dungeons[currentFloor].enemies[2].name = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            spm::dan::dan_wp->dungeons[currentFloor].enemies[2].num = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            spm::dan::dan_wp->dungeons[currentFloor].enemies[2].pos = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            if (enemyConfigArray[enemyArrayOffset] != 0)
            {
                spm::dan::dan_wp->dungeons[currentFloor].enemies[3].name = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                spm::dan::dan_wp->dungeons[currentFloor].enemies[3].num = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                spm::dan::dan_wp->dungeons[currentFloor].enemies[3].pos = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
            }
        }

        if (currentFloor >= 100)
        {
            int currentFloor2 = (currentFloor - 100);
            enemyArrayOffset = (currentFloor * 10 + 1);
            spm::dan::dan_wp->dungeons[currentFloor2].enemyCount = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            spm::dan::dan_wp->dungeons[currentFloor2].enemies[1].name = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            spm::dan::dan_wp->dungeons[currentFloor2].enemies[1].num = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            spm::dan::dan_wp->dungeons[currentFloor2].enemies[1].pos = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            wii::os::OSReport("enemyCount for current floor #%d: %d.\n", currentFloor2, spm::dan::dan_wp->dungeons[currentFloor2].enemyCount);
            if (enemyConfigArray[enemyArrayOffset] != 0)
            {
                spm::dan::dan_wp->dungeons[currentFloor2].enemies[2].name = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                spm::dan::dan_wp->dungeons[currentFloor2].enemies[2].num = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                spm::dan::dan_wp->dungeons[currentFloor2].enemies[2].pos = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                if (enemyConfigArray[enemyArrayOffset] != 0)
                {
                    spm::dan::dan_wp->dungeons[currentFloor2].enemies[3].name = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    spm::dan::dan_wp->dungeons[currentFloor2].enemies[3].num = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    spm::dan::dan_wp->dungeons[currentFloor2].enemies[3].pos = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                }
            }
        }

        nextFloor = 0;
        nextEnemyCount = 0;
        nextName1 = 0;
        nextNum1 = 0;
        nextPos1 = 0;
        nextName2 = 0;
        nextNum2 = 0;
        nextPos2 = 0;
        nextName3 = 0;
        nextNum3 = 0;
        nextPos3 = 0;
        // Set up values for debug displays
        if (currentFloor != 199)
        {
            nextFloor = currentFloor + 1;
            enemyArrayOffset = ((nextFloor * 10) + 1);
            nextEnemyCount = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            nextName1 = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            nextNum1 = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            nextPos1 = enemyConfigArray[enemyArrayOffset];
            enemyArrayOffset = enemyArrayOffset + 1;
            if (enemyConfigArray[enemyArrayOffset] != 0)
            {
                nextName2 = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                nextNum2 = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                nextPos2 = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                if (enemyConfigArray[enemyArrayOffset] != 0)
                {
                    nextName3 = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    nextNum3 = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    nextPos3 = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                }
            }
        }

        // Replace Flimm inventory; this sets a number of random items from the custom rotenShopItemPool to be selectable across the entire Pit session.
        if (currentFloor == 0)
        {
            int poolItem = 0;
            for (i = 0; i < 67; i = i + 3)
            {
                poolItem = spm::system::rand() % 165;
                newRotenShopItems[i] = rotenShopItemPool[poolItem];
            }
            for (i = 0; i < 70; i++)
            {
                spm::dan::dan_rotenShopItems[i] = newRotenShopItems[i];
            }
        }

        // Free pit text
        spm::parse::parsePop();

        return EVT_RET_CONTINUE;
    }

    static const char *(*msgSearchReal)(const char *msgName);
    static void messagePatch()
    {
        msgSearchReal = patch::hookFunction(spm::msgdrv::msgSearch,
                                            [](const char *msgName)
                                            {
                                                if (msl::string::strcmp(msgName, "D100_entrance_01") == 0)
                                                    // flipside pit first entrance text
                                                    return D100_entrance_01;
                                                else if (msl::string::strcmp(msgName, "mac_kanban_003") == 0)
                                                    // flipside pit entrance sign
                                                    return mac_kanban_003;
                                                else if (msl::string::strcmp(msgName, "modoridokan_D100_01") == 0)
                                                    // flipside pit first exit text
                                                    return modoridokan_D100_01;
                                                else if (msl::string::strcmp(msgName, "D100_entrance_03") == 0)
                                                    // flipside pit first exit text
                                                    return D100_entrance_03;
                                                else if (msl::string::strcmp(msgName, "mac_kanban_004") == 0)
                                                    // flipside pit first exit text
                                                    return mac_kanban_004;
                                                else
                                                    return msgSearchReal(msgName);
                                            });
    }

    // I literally had to port this entire thing just to remove a condition that makes certain enemies force the Pit to fucking crash upon entry. Fuck you Intelligent Systems. 6
    s32 evt_dan_get_enemy_info_new(spm::evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)isFirstCall;

        // Get dungeon and enemy index
        spm::evtmgr::EvtScriptCode *args = entry->pCurData;
        //    s32 no = spm::evtmgr_cmd::evtGetValue(entry, args[0]);
        s32 enemyIdx = spm::evtmgr_cmd::evtGetValue(entry, args[1]);
        spm::dan::DanDungeon *dungeon = spm::dan::dan_wp->dungeons + no;

        if ((enemyIdx < 0) || (enemyIdx >= 16))
        {
            // Return 0 for invalid enemies
            spm::evtmgr_cmd::evtSetValue(entry, args[2], 0);
            spm::evtmgr_cmd::evtSetValue(entry, args[3], 0);

            return EVT_RET_CONTINUE;
        }
        else
        {
            // Get enemy
            spm::dan::DanEnemy *enemy = dungeon->enemies + enemyIdx;

            if (enemy->num > 0)
            {
                // Find template with correct tribe id
                s32 tribeId = enemy->name - 1;
                s32 i;
                spm::npcdrv::NPCEnemyTemplate *curTemplate = spm::npcdrv::npcEnemyTemplates;
                for (i = 0; i < NPCTEMPLATE_MAX; i++, curTemplate++)
                {
                    spm::npcdrv::npcEnemyTemplates[10].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[13].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[74].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[76].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[55].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[101].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[321].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[322].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[323].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[324].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[82].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[316].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[317].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[318].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[319].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[320].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[276].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[277].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[279].unknown_0x8 = 0;

                    if (((curTemplate->unknown_0x8 & 1) == 0) && (curTemplate->tribeId == tribeId))
                        break;
                }
                assertf(i < NPCTEMPLATE_MAX, "みつかりませんでした[%d]", tribeId);

                // Return template id and num
                spm::evtmgr_cmd::evtSetValue(entry, args[2], i);
                spm::evtmgr_cmd::evtSetValue(entry, args[3], enemy->num);
            }
            else
            {
                // Return 0 for empty enemy slots
                spm::evtmgr_cmd::evtSetValue(entry, args[2], 0);
                spm::evtmgr_cmd::evtSetValue(entry, args[3], 0);
            }

            return EVT_RET_CONTINUE;
        }
    }

    static spm::seqdef::SeqFunc *seq_gameMainReal;

    void youSuckDisplay(spm::seqdrv::SeqWork *wp)
    {
        if (youSuck)
        {
            wii::gx::GXColor funnyColor = {255, 255, 255, 255};
            f32 scale = 2.8f;
            char buffer[50];
            const char *youSuckText = "YOU SUCK";
            msl::stdio::sprintf(buffer, "%s", youSuckText);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&funnyColor);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColor();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x, 200.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void new_dan_gameover()
    {
        youSuck = true;
    }

    void debugDisplay_no(spm::seqdrv::SeqWork *wp)
    {
        if (currentFloor >= 1 && currentFloor < 198 && displayDebugValues)
        {
            char buffer[50];
            wii::gx::GXColor reddish = {
                255,
                0,
                127,
                255};
            f32 scale = 0.5f;
            sprintf(buffer, "GSW1: %d", currentFloor);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&reddish);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x + 340, 78.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void debugDisplay_name_one(spm::seqdrv::SeqWork *wp)
    {
        if (currentFloor >= 1 && currentFloor < 198 && displayDebugValues)
        {
            char buffer[50];
            wii::gx::GXColor reddish = {
                200,
                27,
                127,
                127};
            f32 scale = 0.5f;
            sprintf(buffer, "Enemy 1: %d (%d)", spm::dan::dan_wp->dungeons[currentFloor].enemies[1].name, spm::dan::dan_wp->dungeons[currentFloor].enemies[1].num);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&reddish);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x + 314, 64.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void debugDisplay_name_two(spm::seqdrv::SeqWork *wp)
    {
        if (currentFloor >= 1 && currentFloor < 198 && displayDebugValues)
        {
            char buffer[50];
            wii::gx::GXColor reddish = {
                200,
                27,
                127,
                127};
            f32 scale = 0.5f;
            sprintf(buffer, "Enemy 2: %d (%d)", spm::dan::dan_wp->dungeons[currentFloor].enemies[2].name, spm::dan::dan_wp->dungeons[currentFloor].enemies[2].num);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&reddish);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x + 312, 50.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void debugDisplay_name_three(spm::seqdrv::SeqWork *wp)
    {
        if (currentFloor >= 1 && currentFloor < 198 && displayDebugValues)
        {
            char buffer[50];
            wii::gx::GXColor reddish = {
                200,
                27,
                127,
                127};
            f32 scale = 0.5f;
            sprintf(buffer, "Enemy 3: %d (%d)", spm::dan::dan_wp->dungeons[currentFloor].enemies[3].name, spm::dan::dan_wp->dungeons[currentFloor].enemies[3].num);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&reddish);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x + 312, 36.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void debugDisplay_next_name_one(spm::seqdrv::SeqWork *wp)
    {
        if (currentFloor >= 1 && currentFloor < 198 && displayDebugValues)
        {
            char buffer[50];
            wii::gx::GXColor reddish = {
                127,
                50,
                200,
                127};
            f32 scale = 0.5f;
            sprintf(buffer, "Next Enemy 1: %d (%d)", nextName1, nextNum1);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&reddish);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x + 300, 8.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void debugDisplay_next_name_two(spm::seqdrv::SeqWork *wp)
    {
        if (currentFloor >= 1 && currentFloor < 198 && displayDebugValues)
        {
            char buffer[50];
            wii::gx::GXColor reddish = {
                127,
                50,
                200,
                127};
            f32 scale = 0.5f;
            sprintf(buffer, "Next Enemy 2: %d (%d)", nextName2, nextNum2);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&reddish);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x + 300, -6.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void debugDisplay_next_name_three(spm::seqdrv::SeqWork *wp)
    {
        if (currentFloor >= 1 && currentFloor < 198 && displayDebugValues)
        {
            char buffer[50];
            wii::gx::GXColor reddish = {
                127,
                50,
                200,
                127};
            f32 scale = 0.5f;
            sprintf(buffer, "Next Enemy 3: %d (%d)", nextName3, nextNum3);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&reddish);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x + 300, -20.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void textDisplay(spm::seqdrv::SeqWork *wp)
    {
        debugDisplay_no(wp);
        debugDisplay_name_one(wp);
        debugDisplay_name_two(wp);
        debugDisplay_name_three(wp);
        debugDisplay_next_name_one(wp);
        debugDisplay_next_name_two(wp);
        debugDisplay_next_name_three(wp);
        youSuckDisplay(wp);
    }

    static spm::seqdef::SeqFunc *seq_titleMainReal;
    static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
    {
        wii::gx::GXColor notgreen = {230, 116, 216, 255};
        f32 scale = 0.8f;
        const char *msg = "Project SPM Pit Rework Name Pending alpha 1.3.1";
        spm::fontmgr::FontDrawStart();
        spm::fontmgr::FontDrawEdge();
        spm::fontmgr::FontDrawColor(&notgreen);
        spm::fontmgr::FontDrawScale(scale);
        spm::fontmgr::FontDrawNoiseOff();
        spm::fontmgr::FontDrawRainbowColorOff();
        f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
        spm::fontmgr::FontDrawString(x, 200.0f, msg);
        seq_titleMainReal(wp);
    }
    static void titleScreenCustomTextPatch()
    {
        seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
        spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
        seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
        spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &textDisplay;
    }

    static void danOverwrite()
    {
        patch::hookFunction(spm::dan::evt_dan_read_data, evt_dan_read_data_new);
    }

    static void danYouSuck()
    {
        patch::hookFunction(spm::dan::danCountdownDone, new_dan_gameover);
    }

    static void danDontFuckingCrash()
    {
        patch::hookFunction(spm::dan::evt_dan_get_enemy_info, evt_dan_get_enemy_info_new);
    }

    static void evtNpcPatchAllEnemies()
    {
        patch::hookFunction(spm::evt_npc::evt_npc_set_rgba, [](spm::evtmgr::EvtEntry *entry, bool isFirstCall)
                            {
            (void)entry;
            (void)isFirstCall;
            return 2; });

        patch::hookFunction(spm::evt_npc::evt_npc_set_rgbacopy, [](spm::evtmgr::EvtEntry *entry, bool isFirstCall)
                            {
            (void)entry;
            (void)isFirstCall;
            return 2; });
        patch::hookFunction(spm::evt_npc::evt_npc_set_rgbacopytwo, [](spm::evtmgr::EvtEntry *entry, bool isFirstCall)
                            {
            (void)entry;
            (void)isFirstCall;
            return 2; });
    }

    static void danPatchPitEnemies()
    {
        // Define default animposes for new enemies
        spm::npcdrv::npcTribes[5].animPoseName = "e_kuribo_y";     // Unused Spiked Goomba --> Spiked Gloomba
        spm::npcdrv::npcTribes[2].animPoseName = "e_kuribo_h";     // Dark Goomba --> Hyper Goomba
        spm::npcdrv::npcTribes[6].animPoseName = "e_kuribo_h";     // Dark Spiked Goomba --> Hyper Spiked Goomba
        spm::npcdrv::npcTribes[8].animPoseName = "e_kuribo_h";     // Dark Paragoomba --> Hyper Paragoomba
        spm::npcdrv::npcTribes[16].animPoseName = "e_nokoteki_d";  // Dark Koopa --> Dark Koopa
        spm::npcdrv::npcTribes[24].animPoseName = "e_nokoteki_d";  // Dark Paratroopa --> Dark Paratroopa
        spm::npcdrv::npcTribes[446].animPoseName = "e_cheririn_a"; // Dark Cherbil --> Ash Cherbil
        spm::npcdrv::npcTribes[38].animPoseName = "e_togezb";      // Dark Spiny --> Sky-Blue Spiny
        spm::npcdrv::npcTribes[29].animPoseName = "e_mer";         // Dark Spike Top --> Red Spike Top
        spm::npcdrv::npcTribes[98].animPoseName = "e_chorobon_g";  // Dark Fuzzy --> Green Fuzzy
        spm::npcdrv::npcTribes[43].animPoseName = "e_karon_d";     // Dark Dull Bones --> Dark Bones
        spm::npcdrv::npcTribes[44].animPoseName = "e_karon_d";     // Dark Bones projectile
        spm::npcdrv::npcTribes[111].animPoseName = "e_kmond";      // Dark Ruff Puff --> Dark Puff
        spm::npcdrv::npcTribes[95].animPoseName = "e_gabow";       // Dark Clubba --> White Clubba
        spm::npcdrv::npcTribes[66].animPoseName = "e_kames";       // Dark Magikoopa --> Shady Magikoopa
        spm::npcdrv::npcTribes[67].animPoseName = "e_kames";       // Dark Broom Magikoopa --> Shady Broom Magikoopa
                                                                   /*  spm::npcdrv::npcTribes[124].animPoseName = "e_wanwan_g";   // Dark Chomp --> Gold Chomp
                                                                       spm::npcdrv::npcTribes[124].partsList[1].animPoseName = "e_wanwan_g";
                                                                       spm::npcdrv::npcTribes[124].partsList[2].animPoseName = "e_wanwan_g";
                                                                       spm::npcdrv::npcTribes[124].partsList[3].animPoseName = "e_wanwan_g";
                                                                       spm::npcdrv::npcTribes[124].partsList[4].animPoseName = "e_wanwan_g";
                                                                       spm::npcdrv::npcTribes[124].partsList[5].animPoseName = "e_wanwan_g";
                                                                       spm::npcdrv::npcTribes[124].partsList[6].animPoseName = "e_wanwan_g"; */
        spm::npcdrv::npcTribes[167].animPoseName = "e_ntl_p";      // Dark Tileoid --> Tileoid PU
        spm::npcdrv::npcTribes[220].animPoseName = "e_nin_d";      // Dark Ninjoe --> Ninjeremiah
        spm::npcdrv::npcTribes[19].animPoseName = "e_togenokd";    // Dark Koopatrol --> Dark Koopatrol
        spm::npcdrv::npcTribes[171].animPoseName = "e_jyama_b";    // Dark Jawbus --> Bawbus
        spm::npcdrv::npcTribes[10].animPoseName = "e_k_kuribo";    // Dark Headbonk Goomba --> Kamikaze Goomba
        spm::npcdrv::npcTribes[225].animPoseName = "e_tesita_bt";  // Dark Skellobit --> Skellobyte
        spm::npcdrv::npcTribes[228].animPoseName = "e_tesita_sb";  // Dark Spiky Skellobit --> Spiky Skellobyte
        spm::npcdrv::npcTribes[49].animPoseName = "e_buross_h";    // Dark Hammer Bro --> Shady Hammer Bro
        spm::npcdrv::npcTribes[55].animPoseName = "e_buross_b";    // Dark Boomerang Bro --> Shady Boomerang Bro
        spm::npcdrv::npcTribes[56].animPoseName = "e_buross_b";    // Shady Boomerang Bro projectile
        spm::npcdrv::npcTribes[61].animPoseName = "e_burosu_i";    // Dark Fire Bro --> Ice Bro
        spm::npcdrv::npcTribes[62].animPoseName = "e_burosu_i";    // Ice Bro projectile
        spm::npcdrv::npcTribes[101].animPoseName = "e_sinemoh";    // Dark Cleft --> Hyper Cleft
        spm::npcdrv::npcTribes[75].animPoseName = "e_k_shoote4";   // Dark Striker --> Shady Striker

        // Adjust new NPC stats
        // Spinia
        spm::npcdrv::npcTribes[495].catchCardItemId = 344;
        spm::npcdrv::npcTribes[495].catchCardDefense = 5;
        spm::npcdrv::npcTribes[495].maxHp = 4;
        spm::npcdrv::npcTribes[495].killXp = 200;
        spm::npcdrv::npcTribes[495].coinDropChance = 100;
        spm::npcdrv::npcTribes[495].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[495].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[495].coinDropExtraMax = 1;
        spm::npcdrv::npcTribes[495].attackStrength = 1;

        // Spunia
        spm::npcdrv::npcTribes[496].catchCardItemId = 350;
        spm::npcdrv::npcTribes[496].catchCardDefense = 20;
        spm::npcdrv::npcTribes[496].maxHp = 10;
        spm::npcdrv::npcTribes[496].killXp = 600;
        spm::npcdrv::npcTribes[496].coinDropChance = 100;
        spm::npcdrv::npcTribes[496].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[496].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[496].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[496].attackStrength = 3;

        // Shady Koopa, ATK 5 DEF 3
        spm::npcdrv::npcTribes[465].catchCardItemId = 304;
        spm::npcdrv::npcTribes[465].catchCardDefense = 30;
        spm::npcdrv::npcTribes[465].maxHp = 15;
        spm::npcdrv::npcTribes[465].killXp = 300;
        spm::npcdrv::npcTribes[465].coinDropChance = 100;
        spm::npcdrv::npcTribes[465].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[465].coinDropExtraChance = 75;
        spm::npcdrv::npcTribes[465].coinDropExtraMax = 5;
        spm::npcdrv::npcTribes[465].attackStrength = 5;

        // Flip Shady Koopa, ATK 5 DEF 3
        spm::npcdrv::npcTribes[466].catchCardItemId = 304;
        spm::npcdrv::npcTribes[466].catchCardDefense = 30;
        spm::npcdrv::npcTribes[466].maxHp = 15;
        spm::npcdrv::npcTribes[466].killXp = 300;
        spm::npcdrv::npcTribes[466].coinDropChance = 100;
        spm::npcdrv::npcTribes[466].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[466].coinDropExtraChance = 75;
        spm::npcdrv::npcTribes[466].coinDropExtraMax = 5;
        spm::npcdrv::npcTribes[466].attackStrength = 5;

        // Spiked Gloomba, ATK 3
        spm::npcdrv::npcTribes[5].catchCardItemId = 336;
        spm::npcdrv::npcTribes[5].catchCardDefense = 6;
        spm::npcdrv::npcTribes[5].maxHp = 2;
        spm::npcdrv::npcTribes[5].killXp = 200;
        spm::npcdrv::npcTribes[5].coinDropChance = 100;
        spm::npcdrv::npcTribes[5].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[5].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[5].coinDropExtraMax = 1;
        spm::npcdrv::npcTribes[5].attackStrength = 3;

        // Green Magikoopa, DEF 2
        spm::npcdrv::npcTribes[473].catchCardItemId = 353;
        spm::npcdrv::npcTribes[473].catchCardDefense = 20;
        spm::npcdrv::npcTribes[473].maxHp = 6;
        spm::npcdrv::npcTribes[473].killXp = 600;
        spm::npcdrv::npcTribes[473].coinDropChance = 100;
        spm::npcdrv::npcTribes[473].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[473].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[473].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[473].attackStrength = 3;

        // Green Broom Magikoopa, DEF 2
        spm::npcdrv::npcTribes[474].catchCardItemId = 353;
        spm::npcdrv::npcTribes[474].catchCardDefense = 20;
        spm::npcdrv::npcTribes[474].maxHp = 6;
        spm::npcdrv::npcTribes[474].killXp = 600;
        spm::npcdrv::npcTribes[474].coinDropChance = 100;
        spm::npcdrv::npcTribes[474].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[474].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[474].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[474].attackStrength = 3;

        // Green Magi Projectile
        spm::npcdrv::npcTribes[475].catchCardItemId = 353;

        // White Magikoopa
        spm::npcdrv::npcTribes[476].catchCardItemId = 352;
        spm::npcdrv::npcTribes[476].catchCardDefense = 20;
        spm::npcdrv::npcTribes[476].maxHp = 18;
        spm::npcdrv::npcTribes[476].killXp = 600;
        spm::npcdrv::npcTribes[476].coinDropChance = 100;
        spm::npcdrv::npcTribes[476].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[476].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[476].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[476].attackStrength = 3;

        // White Broom Magikoopa
        spm::npcdrv::npcTribes[477].catchCardItemId = 352;
        spm::npcdrv::npcTribes[477].catchCardDefense = 20;
        spm::npcdrv::npcTribes[477].maxHp = 18;
        spm::npcdrv::npcTribes[477].killXp = 600;
        spm::npcdrv::npcTribes[477].coinDropChance = 100;
        spm::npcdrv::npcTribes[477].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[477].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[477].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[477].attackStrength = 3;

        // White Magi Projectile
        spm::npcdrv::npcTribes[478].catchCardItemId = 352;

        // Red Magikoopa
        spm::npcdrv::npcTribes[479].catchCardItemId = 351;
        spm::npcdrv::npcTribes[479].catchCardDefense = 20;
        spm::npcdrv::npcTribes[479].maxHp = 6;
        spm::npcdrv::npcTribes[479].killXp = 600;
        spm::npcdrv::npcTribes[479].coinDropChance = 100;
        spm::npcdrv::npcTribes[479].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[479].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[479].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[479].attackStrength = 6;

        // Red Broom Magikoopa
        spm::npcdrv::npcTribes[480].catchCardItemId = 351;
        spm::npcdrv::npcTribes[480].catchCardDefense = 20;
        spm::npcdrv::npcTribes[480].maxHp = 5;
        spm::npcdrv::npcTribes[480].killXp = 600;
        spm::npcdrv::npcTribes[480].coinDropChance = 100;
        spm::npcdrv::npcTribes[480].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[480].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[480].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[480].attackStrength = 5;

        // Red Magi Projectile, ATK 6
        spm::npcdrv::npcTribes[481].catchCardItemId = 351;

        // Bleepboxer, ATK 3
        spm::npcdrv::npcTribes[504].catchCardItemId = 384;
        spm::npcdrv::npcTribes[504].catchCardDefense = 15;
        spm::npcdrv::npcTribes[504].maxHp = 12;
        spm::npcdrv::npcTribes[504].killXp = 500;
        spm::npcdrv::npcTribes[504].coinDropChance = 100;
        spm::npcdrv::npcTribes[504].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[504].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[504].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[504].attackStrength = 3;

        // Kilo Muth, ATK 2
        spm::npcdrv::npcTribes[506].catchCardItemId = 427;
        spm::npcdrv::npcTribes[506].catchCardDefense = 15;
        spm::npcdrv::npcTribes[506].maxHp = 25;
        spm::npcdrv::npcTribes[506].killXp = 700;
        spm::npcdrv::npcTribes[506].attackStrength = 2;

        // Mega Muth and Muth HP drop
        spm::npcdrv::npcTribes[257].maxHp = 50;
        spm::npcdrv::npcTribes[258].maxHp = 75;

        // Ice Bro nerf, ATK 3 DEF 2
        spm::npcdrv::npcTribes[61].catchCardDefense = 16;
        spm::npcdrv::npcTribes[61].maxHp = 10;
        spm::npcdrv::npcTribes[61].killXp = 800;
        spm::npcdrv::npcTribes[61].coinDropChance = 100;
        spm::npcdrv::npcTribes[61].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[61].coinDropExtraChance = 50;
        spm::npcdrv::npcTribes[61].coinDropExtraMax = 5;
        spm::npcdrv::npcTribes[61].attackStrength = 3;

        // Dark Puff nerf
        spm::npcdrv::npcTribes[111].catchCardDefense = 8;
        spm::npcdrv::npcTribes[111].maxHp = 6;
        spm::npcdrv::npcTribes[111].killXp = 300;
        spm::npcdrv::npcTribes[111].coinDropChance = 100;
        spm::npcdrv::npcTribes[111].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[111].coinDropExtraChance = 40;
        spm::npcdrv::npcTribes[111].coinDropExtraMax = 2;
        spm::npcdrv::npcTribes[111].attackStrength = 2;

        // Flip Buzzy Beetle/Flip Spike Top patch
        spm::npcdrv::npcTribes[470].catchCardItemId = 298;
        spm::npcdrv::npcTribes[470].catchCardDefense = 2;
        spm::npcdrv::npcTribes[470].maxHp = 1;
        spm::npcdrv::npcTribes[470].killXp = 200;
        spm::npcdrv::npcTribes[470].coinDropChance = 100;
        spm::npcdrv::npcTribes[470].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[470].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[470].coinDropExtraMax = 2;
        spm::npcdrv::npcTribes[470].attackStrength = 1;

        spm::npcdrv::npcTribes[471].catchCardItemId = 299;
        spm::npcdrv::npcTribes[471].catchCardDefense = 10;
        spm::npcdrv::npcTribes[471].maxHp = 1;
        spm::npcdrv::npcTribes[471].killXp = 300;
        spm::npcdrv::npcTribes[471].coinDropChance = 100;
        spm::npcdrv::npcTribes[471].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[471].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[471].coinDropExtraMax = 2;
        spm::npcdrv::npcTribes[471].attackStrength = 2;

        // Green Fuzzy nerf
        spm::npcdrv::npcTribes[531].catchCardDefense = 16;
        spm::npcdrv::npcTribes[531].maxHp = 10;
        spm::npcdrv::npcTribes[531].killXp = 700;
        spm::npcdrv::npcTribes[531].coinDropChance = 100;
        spm::npcdrv::npcTribes[531].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[531].coinDropExtraChance = 40;
        spm::npcdrv::npcTribes[531].coinDropExtraMax = 2;
        spm::npcdrv::npcTribes[531].attackStrength = 2;

        // Tileoid PU stats
        spm::npcdrv::npcTribes[167].catchCardDefense = 12;
        spm::npcdrv::npcTribes[167].maxHp = 1;
        spm::npcdrv::npcTribes[167].killXp = 600;
        spm::npcdrv::npcTribes[167].coinDropChance = 100;
        spm::npcdrv::npcTribes[167].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[167].coinDropExtraChance = 40;
        spm::npcdrv::npcTribes[167].coinDropExtraMax = 6;
        spm::npcdrv::npcTribes[167].attackStrength = 5;

        // White Clubba nerf
        spm::npcdrv::npcTribes[95].maxHp = 30;
        spm::npcdrv::npcTribes[95].attackStrength = 5;

        // Shadoo nerf
        spm::npcdrv::npcTribes[332].maxHp = 50; // Dark Peach
        spm::npcdrv::npcTribes[333].maxHp = 50; // Dark Bowser
        spm::npcdrv::npcTribes[330].maxHp = 50; // Dark Mario
        spm::npcdrv::npcTribes[331].maxHp = 50; // Dark Luigi
    }

    static spm::evt_door::DokanDesc new_dan_70_dokan_desc = {
        0, 0, 0, "dokan", "dan_70", "A2D_dokan_1", "A3D_dokan_1", "mac_05", "dokan_1"};

    /*  static spm::evt_door::DokanDesc temp_mac_04_2_dokan_desc = {
          0, 0, 0, "dokan", "mac_04", "A2D_dokan_1", "A3D_dokan_1", "dan_70", "dokan_1"}; */

    /*    spm::npcdrv::NPCTribeAnimDef nastasiaAnims[] = {
            {0, "S_1"}, // Standing (idle)
            {3, "T_1"}, // Talking
            {-1, nullptr}};

        EVT_BEGIN(nastasia_speech)
        USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
        USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("Hey,<wait 250> nice to see you here.\n<k><p>I came here to Flipside to\ncheck out the renovations\nthey've done to the Pit.\n<k><p>Apparently,<wait 250> they completely\nremade the room layouts,\n<k><p>And I think there were some\nnew enemies too.\n<k><p>You should check it out,<wait 250> it\nseems like the kind of thing\nthat would interest you.<k>"), 0, PTR("me"))
        USER_FUNC(spm::evt_mario::evt_mario_key_on)
        RETURN()
        EVT_END()

        EVT_BEGIN(fwd_nastasia_speech)
        RUN_EVT(nastasia_speech)
        RETURN()
        EVT_END()

        EVT_BEGIN(flipside_nastasia_funny)
        USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("nastasiaFunny"), PTR("n_nasta"), 0)
        USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("nastasiaFunny"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(nastasiaAnims))
        USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("nastasiaFunny"), 0, true)
        USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("nastasiaFunny"), -75, 1500, -150)
        USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("nastasiaFunny"), mod::cutscene_helpers::NPCProperty::INTERACT, PTR(fwd_nastasia_speech))
        RETURN_FROM_CALL() */

    EVT_BEGIN(overwrite_dark_mario_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 523, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_peach_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 458, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_dark_peach_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 521, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_bowser_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 476, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_dark_bowser_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 536, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_luigi_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 512, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_dark_luigi_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 231, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(disable_flopside_pit_entrance)
    USER_FUNC(spm::evt_door::evt_door_enable_disable_dokan_desc, 0, PTR("dokan_1"))
    USER_FUNC(spm::evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("dokan_1"), 1)
    USER_FUNC(spm::evt_hit::evt_hitobj_onoff, PTR("A2d_dokan_1"), 1, 0)
    USER_FUNC(spm::evt_hit::evt_hitobj_onoff, PTR("A3d_dokan_1"), 1, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(patch_pit_exit)
    USER_FUNC(spm::evt_door::evt_door_set_dokan_descs, PTR(&new_dan_70_dokan_desc), 1)
    RETURN_FROM_CALL()

    // TEMP
    /*  EVT_BEGIN(temp_pipe)
      USER_FUNC(spm::evt_door::evt_door_set_dokan_descs, PTR(&temp_mac_04_2_dokan_desc), 1)
      RETURN_FROM_CALL() */

    // Dark Puff contact ATK patch
    EVT_BEGIN(d_puff_dir_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 1)
    RETURN_FROM_CALL()

    // Dark Puff projectile ATK patch
    EVT_BEGIN(d_puff_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 2)
    RETURN_FROM_CALL()

    // Ice Bro projectile ATK patch
    EVT_BEGIN(i_bro_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Ice Bro contact ATK patch
    EVT_BEGIN(i_bro_dir_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    RETURN_FROM_CALL()

    // Hyper Goomba contact ATK patch
    EVT_BEGIN(h_goomba_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Hyper Paragoomba contact ATK patch
    EVT_BEGIN(h_pgoomba_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Red Magikoopa projectile ATK patch
    EVT_BEGIN(r_magi_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Tileoid PU ATK patch
    EVT_BEGIN(p_tile_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Kilo Muth ATK/DEF patch
    EVT_BEGIN(k_muth_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    RETURN_FROM_CALL()

    // Spiked Gloomba ATK patch
    EVT_BEGIN(s_gloomba_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 3)
    RETURN_FROM_CALL()

    // Shady Koopa/Flip Shady Koopa ATK patch
    EVT_BEGIN(uranoko_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 4)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 2, 4)
    RETURN_FROM_CALL()

    // Bleepboxer projectile ATK patch
    EVT_BEGIN(bleep_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 3)
    RETURN_FROM_CALL()

    // White Clubba ATK patch
    EVT_BEGIN(w_clubba_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 5)
    RETURN_FROM_CALL()

    static void evtPatches()
    {
        // Initialize the patches to the EVT interpreter to add custom opcodes
        evtpatch::evtmgrExtensionInit();

        /* // Get Flipside Tower's init EVT script
         spm::evtmgr_cmd::EvtScriptCode *flipsideInitEvt = spm::map_data::mapDataPtr("mac_02")->initScript;

         // Add a hook at the beginning of Flipside's init EVT script that will play our custom script first
         evtpatch::hookEvt(flipsideInitEvt, 1, flipside_nastasia_funny); */

        // Render Flopside Pit pipe useless and patch Pit exit pipe
        spm::evtmgr_cmd::EvtScriptCode *disableFlopsidePitEntrance = spm::map_data::mapDataPtr("mac_15")->initScript;
        evtpatch::hookEvt(disableFlopsidePitEntrance, 11, disable_flopside_pit_entrance);
        evtpatch::hookEvt(spm::dan::dan_70_init_evt, 8, patch_pit_exit);

        // Get enemy onSpawnScripts from templates
        spm::evtmgr_cmd::EvtScriptCode *dPuffDirAtk = spm::npcdrv::npcEnemyTemplates[357].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *dPuffAtk = spm::npcdrv::npcEnemyTemplates[358].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *iBroDirAtk = spm::npcdrv::npcEnemyTemplates[341].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *iBroAtk = spm::npcdrv::npcEnemyTemplates[344].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *hGoombaAtk = spm::npcdrv::npcEnemyTemplates[329].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *hpGoombaAtk = spm::npcdrv::npcEnemyTemplates[328].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *rMagiAtk = spm::npcdrv::npcEnemyTemplates[324].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *pTileAtk = spm::npcdrv::npcEnemyTemplates[379].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *kMuthAtk = spm::npcdrv::npcEnemyTemplates[279].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *sGloombaAtk = spm::npcdrv::npcEnemyTemplates[6].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *uranokoAtk = spm::npcdrv::npcEnemyTemplates[10].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *fUranokoAtk = spm::npcdrv::npcEnemyTemplates[13].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *bleepAtk = spm::npcdrv::npcEnemyTemplates[277].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *wClubbaAtk = spm::npcdrv::npcEnemyTemplates[350].onSpawnScript;

        // ATK overwrite
        evtpatch::hookEvtReplace(dPuffDirAtk, 2, d_puff_dir_atk);
        evtpatch::hookEvtReplace(dPuffAtk, 11, d_puff_atk);
        evtpatch::hookEvtReplace(iBroDirAtk, 1, i_bro_dir_atk);
        evtpatch::hookEvtReplace(iBroAtk, 1, i_bro_atk);
        evtpatch::hookEvtReplace(hGoombaAtk, 2, h_goomba_atk);
        evtpatch::hookEvtReplace(hpGoombaAtk, 2, h_pgoomba_atk);
        evtpatch::hookEvtReplace(rMagiAtk, 1, r_magi_atk);
        evtpatch::hookEvtReplace(pTileAtk, 1, p_tile_atk);
        evtpatch::hookEvtReplace(kMuthAtk, 1, k_muth_atk);
        evtpatch::hookEvtReplace(sGloombaAtk, 2, s_gloomba_atk);
        evtpatch::hookEvtReplace(uranokoAtk, 5, uranoko_atk);
        evtpatch::hookEvtReplace(fUranokoAtk, 2, uranoko_atk);
        evtpatch::hookEvtReplace(bleepAtk, 1, bleep_atk);
        evtpatch::hookEvtReplace(wClubbaAtk, 1, w_clubba_atk);

        // Post-Shadoo chest reward overwrites
        evtpatch::hookEvtReplace(spm::dan::dan_70_dark_mario_chest_open_evt, 3, overwrite_dark_mario_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_peach_chest_open_evt, 3, overwrite_peach_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_dark_peach_chest_open_evt, 3, overwrite_dark_peach_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_bowser_chest_open_evt, 3, overwrite_bowser_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_dark_bowser_chest_open_evt, 3, overwrite_dark_bowser_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_luigi_chest_open_evt, 3, overwrite_luigi_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_dark_luigi_chest_open_evt, 3, overwrite_dark_luigi_card_chest);
    }

    void main()
    {
        wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
        titleScreenCustomTextPatch();
        messagePatch();
        danOverwrite();
        danYouSuck();
        danDontFuckingCrash();
        evtNpcPatchAllEnemies();
        danPatchPitEnemies();
        evtPatches();
        patchMarioDamage();
    }
}