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
        "</col><col c000fff0><center>Reworked</center>\n"
        "</col><center>Discord: @tartt</center>\n"
        "<k>";

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
    /*
    const char * m_noroi =
    "lmao skill issue\n"
    "<k>";
    */

    static double boobies = 0;

    // Create arrays for enemy generation, separated by difficulty class

    // Goomba, Flip Goomba, Paragoomba, Green Koopa, Glassesless Koopa, Red Koopa, Flip Red Koopa, Green Paratroopa, Glassesless Paratroopa, Buzzy Beetle, Stone Buzzy, Boo, Green Cheep Cheep,
    // Red Cheep Cheep, Flip Cheep Cheep, Bald Cleft, Squiglet, Squig, Sproing-Oing, Boing-Oing, Boomboxer, Growmeba, Jellien, Slow Cursya, Cherbil, Poison Cherbil, Frackle, Spinia, Flip Buzzy
    int lv1Names[] = {1, 2, 8, 12, 13, 15, 16, 21, 22, 26, 33, 85, 89, 90, 91, 100, 126, 127, 135, 137, 143, 160, 179, 246, 441, 443, 451, 496, 471};
    int lv1Odds[] = {75, 75, 70, 75, 65, 75, 65, 75, 65, 80, 85, 73, 60, 40, 25, 40, 80, 81, 85, 65, 80, 85, 70, 70, 75, 80, 50, 90, 85};
    int lv1SpR[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv1Min[] = {4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2};
    int lv1Max[] = {7, 6, 5, 5, 5, 5, 5, 4, 4, 5, 6, 6, 4, 4, 3, 6, 6, 6, 5, 4, 5, 3, 5, 5, 4, 5, 4, 6, 5};

    // Gloomba, Spiked Goomba, Non-falling Spiked Goomba, Red Paratroopa, Flip Red Paratroopa, Spike Top, Parabuzzy, Spiny, Flip Spiny, Hammer Bro, Magikoopa, Fuzzy, Pokey,
    // Spania, Chain Chomp, Squog, Beepboxer, Mister I, Shlurp, Tileoid G, Tileoid B, Jawbus, PatrolMeow, AirMeow, Longator, Barribad, Pigarithm, 3x Floro Sapiens, Ninjoe,
    // Tech Cursya, Heavy Cursya, Reversya Cursya, Ice Cherbil, Flip Spike Top
    int lv2Names[] = {4, 5, 6, 23, 24, 28, 31, 37, 38, 46, 64, 97, 103, 114, 123, 129, 145, 151, 157, 164, 165, 169, 174, 175, 189, 195, 201, 206, 208, 210, 212, 248, 249, 250, 445, 472};
    int lv2Odds[] = {85, 50, 40, 70, 70, 65, 75, 80, 70, 80, 70, 80, 80, 85, 70, 90, 75, 50, 20, 40, 40, 20, 85, 75, 80, 30, 64, 30, 30, 30, 40, 40, 35, 45, 75, 70};
    int lv2SpR[] = {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv2Min[] = {4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 2, 3, 1, 3, 3, 1, 2, 2, 2, 1, 3, 3, 3, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 3};
    int lv2Max[] = {6, 6, 5, 5, 4, 5, 4, 6, 6, 4, 5, 6, 4, 6, 3, 6, 5, 3, 4, 4, 4, 3, 5, 4, 6, 2, 5, 5, 5, 5, 4, 4, 4, 4, 4, 5};

    // Headbonk Goomba, Koopatrol, Spiky Parabuzzy, Dry Bones, Flip Hammer Bro, Boomerang Bro, Fire Bro, Broom Magikoopa, Koopa Striker, Bill Blaster, Dark Boo, Clubba, Pink Fuzzy,
    // Moon Cleft, Ruff Puff, Crazee Dayzee, Zoing-Oing, Blomeba, Tileoid R, Tileoid Y, Rawbus, BombMeow, Foton, Hooligon, Copta, Hogarithm, Ninjohn, Flip Skellobit, Flip Skellobomber,
    // Muth, Shady Koopa, Flip Shady Koopa, Spunia
    int lv3Names[] = {10, 19, 32, 40, 48, 52, 58, 65, 70, 84, 86, 95, 98, 101, 110, 116, 139, 161, 166, 167, 170, 178, 180, 185, 193, 202, 215, 225, 231, 258, 466, 467, 497};
    int lv3Odds[] = {85, 45, 70, 68, 70, 75, 75, 75, 85, 50, 70, 75, 65, 75, 80, 70, 85, 80, 40, 40, 20, 30, 40, 50, 60, 48, 40, 50, 50, 10, 70, 70, 85};
    int lv3SpR[] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    int lv3Min[] = {4, 2, 2, 3, 3, 3, 3, 3, 2, 2, 3, 3, 4, 3, 3, 3, 3, 1, 2, 2, 1, 2, 2, 2, 3, 2, 2, 3, 3, 1, 2, 2, 3};
    int lv3Max[] = {6, 5, 4, 5, 4, 4, 4, 4, 5, 4, 6, 5, 6, 5, 6, 5, 5, 3, 4, 4, 3, 5, 4, 3, 5, 4, 4, 6, 5, 2, 4, 4, 6};

    // Lakitu, Flip Lakitu, Dull Bones, Flip Boomerang Bro, Flip Fire Bro, Soopa Striker, Poison Pokey, Amazee Dayzee, Red Chomp, Squoinker, Blastboxer, Red I, Shlorp,
    // Chromeba, Gawbus, Longadile, Sobarribad, Ninjerry, Flip Spiky Skellobit, Flip Skellobomber w/ Skellobait, 3x Magiblots, Back Cursya, Gigabyte, Mega Muth
    int lv4Names[] = {35, 36, 42, 54, 60, 74, 105, 118, 124, 131, 147, 153, 158, 162, 171, 190, 197, 218, 228, 234, 238, 240, 242, 247, 256, 259};
    int lv4Odds[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int lv4SpR[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv4Min[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int lv4Max[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

    int SpR2Doors[] = {3, 6, 12, 13, 19, 22};
    int SpR1Doors[] = {27, 28, 29, 30};

    // Pit Flimm item pool to iterate from
    int rotenShopItemPool[] = {67, 68, 69, 70, 75, 81, 82, 84, 88, 89, 90, 99, 106, 118, 123, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
                               141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160, 161, 162, 163, 164, 166, 168, 169, 170,
                               171, 173, 174, 177, 178, 180, 181, 182, 183, 184, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 198, 199, 200,
                               201, 202, 203, 204, 205, 207, 208, 209, 211, 212, 214, 215, 63, 50, 288, 292, 298, 299, 301, 302, 305, 307, 308, 310, 312,
                               313, 321, 323, 328, 329, 332, 335, 341, 343, 346, 355, 358, 360, 363, 366, 372, 377, 383, 389, 393, 396, 398, 399, 400, 401, 409, 412, 415, 418,
                               424, 426, 428, 433, 432, 436, 437, 440, 444, 446, 447, 448};

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
        bool nipples = spm::swdrv::swGet(409);

        // Prepare pit text to be read
        u32 size = wii::cx::CXGetCompressionHeader(spm::lz_embedded::pitText).decompSize;
        char *decompPitText = (char *)spm::memory::__memAlloc(0, size);
        wii::cx::CXUncompressLZ(spm::lz_embedded::pitText, decompPitText);
        spm::parse::parseInit(decompPitText, size);
        s32 no = 0;
        s32 i = 0;

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
            wii::os::OSReport("Queueing 3 segment generations for this room.\n");
        }
        else if (segmentRNG >= 42 && segmentRNG < 45)
        {
            segmentCount = 1;
            wii::os::OSReport("Queueing 1 segment generation for this room.\n");
        }
        else if (segmentRNG >= 45 && segmentRNG < 109)
        {
            segmentCount = 4;
            wii::os::OSReport("Queueing 4 segment generations for this room.\n");
        }
        else if (segmentRNG >= 109 && segmentRNG < 122)
        {
            segmentCount = 2;
            wii::os::OSReport("Queueing 2 segment generations for this room.\n");
        }
        else if (segmentRNG >= 122 && segmentRNG < 126)
        {
            segmentCount = 0;
            wii::os::OSReport("Queueing 0 segment generations for this room.\n");
        }
        else
        {
            segmentCount = 5;
            wii::os::OSReport("Queueing 5 segment generations for this room.\n");
        }
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
            else if (structureMiscRNG < 33)
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
                    segment10000 = true;
                }
                else if (boobies < 50)
                {
                    segment800 = true;
                    segment400 = true;
                    segment10000 = true;
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
                    (segment10 && segment200 && segment800) ||
                    (segment10 && segment200 && segment400) ||
                    (segment10 && segment200 && segment2000) ||
                    (segment20 && segment200 && segment800) ||
                    (segment20 && segment200 && segment400) ||
                    (segment20 && segment200 && segment2000) ||
                    (segment100 && segment200 && segment800) ||
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

        // Doors
        int doorEntranceArrayVal = spm::system::rand() % activeDoorCount;
        int doorEntrance = doorOn[doorEntranceArrayVal];
        int doorExitArrayVal = spm::system::rand() % activeDoorCount;
        while (doorExitArrayVal == doorEntranceArrayVal)
        {
            doorExitArrayVal = spm::system::rand() % activeDoorCount;
        }
        int doorExit = doorOn[doorExitArrayVal];

        // Add all dungeon entries to work
        while (spm::parse::parsePush("<Dungeon>"))
        {
            // Read no (dungeon id)
            no = 0;
            i = 0;
            spm::parse::parseTagGet1("<no>", spm::parse::PARSE_VALUE_TYPE_INT, &no);
            assertf(no >= 0 && no < DUNGEON_MAX, "なんか番号がおかしい [%d]", no);

            // Read item id (chest contents in chest rooms, null & unused elsewhere)
            char itemName[64];
            spm::parse::parseTagGet1("<item>", spm::parse::PARSE_VALUE_TYPE_STRING, itemName);

            // Item Substitution Protocol
            double floorwiseMultiplier = 1;
            switch (no)
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
                floorwiseMultiplier = 0.4;
                break;
            case 49:
                floorwiseMultiplier = 0;
                break;
            case 59:
                floorwiseMultiplier = -0.4;
                break;
            case 69:
                floorwiseMultiplier = -0.7;
                break;
            case 79:
                floorwiseMultiplier = -0.85;
                break;
            case 89:
                floorwiseMultiplier = -1.0;
                break;
            }

            // Establishing reward weights
            double ShroomShake = msl::math::pow(400, floorwiseMultiplier);
            double FireBurst = msl::math::pow(350, floorwiseMultiplier);
            double StarMedal = msl::math::pow(314, floorwiseMultiplier);
            double BigEgg = msl::math::pow(280, floorwiseMultiplier);
            double ShellShock = msl::math::pow(240, floorwiseMultiplier);
            double FriedShroomPlate = msl::math::pow(210, floorwiseMultiplier);
            double HoneySuper = msl::math::pow(160, floorwiseMultiplier);
            double IceStorm = msl::math::pow(140, floorwiseMultiplier);
            double MysteryBox = msl::math::pow(101, floorwiseMultiplier);
            double BlockBlock = msl::math::pow(64, floorwiseMultiplier);
            double ThunderRage = msl::math::pow(55, floorwiseMultiplier);
            double MightyTonic = msl::math::pow(50, floorwiseMultiplier);
            double RoastedShroomDish = msl::math::pow(40, floorwiseMultiplier);
            double ShootingStar = msl::math::pow(30, floorwiseMultiplier);
            double LifeShroom = msl::math::pow(30, floorwiseMultiplier);
            double MeteorMeal = msl::math::pow(25, floorwiseMultiplier);
            double GoldMedal = msl::math::pow(20, floorwiseMultiplier);
            double UltraShroomShake = msl::math::pow(15, floorwiseMultiplier);
            double DyllisDeluxe = msl::math::pow(5, floorwiseMultiplier);
            double TrialStew = .1;
            double sumOfLoot = (ShroomShake + FireBurst + StarMedal + BigEgg + ShellShock + FriedShroomPlate + HoneySuper + IceStorm + MysteryBox + BlockBlock + ThunderRage + MightyTonic + RoastedShroomDish + ShootingStar + LifeShroom + MeteorMeal + GoldMedal + UltraShroomShake + DyllisDeluxe + TrialStew);

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
            double F_DDeluxe = (F_USShake - DDeluxe);

            // The rabbit is landing
            if (nipples == 0)
            {
                if (boobies >= F_TStew)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
                else if (boobies >= F_SShake)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HUNNY_KINOKO");
                else if (boobies >= F_FBurst)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_HONOO_SAKURETU");
                else if (boobies >= F_SMedal)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_STAR_MEDAL");
                else if (boobies >= F_BEgg)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_BIG_EGG");
                else if (boobies >= F_SShock)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KOURA_DE_PON");
                else if (boobies >= F_FSPlate)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_KINOKO_FRY");
                else if (boobies >= F_HSuper)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HUNNY_KINOKO_S");
                else if (boobies >= F_IStorm)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KOORI_NO_IBUKI");
                else if (boobies >= F_MBox)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_NANIGA_OKORUKANA");
                else if (boobies >= F_BBlock)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_BARIA_FRAME");
                else if (boobies >= F_TRage)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KAMINARI_DOKKAN");
                else if (boobies >= F_MTonic)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_TUYOTUYO_DRINK");
                else if (boobies >= F_RSDish)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_KINOKO_HOILE_FRY");
                else if (boobies >= F_SStar)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KIRAKIRA_OTOSHI");
                else if (boobies >= F_LShroom)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KINKYU_KINOKO");
                else if (boobies >= F_MMeal)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_DINNER");
                else if (boobies >= F_GMedal)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_GOLD_MEDAL");
                else if (boobies >= F_USShake)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_ULTRA_DRINK");
                else if (boobies >= F_DDeluxe)
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_DOROCY_DELUX");
                else
                    spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
            }
            else
            {
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
            }

            // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
            spm::parse::parseTagGet1("<map>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].map);
            spm::dan::dan_wp->dungeons[no].map = roomDecCode;

            // Read doors
            i = 0;
            while (spm::parse::parsePush("<door>"))
            {
                spm::parse::parseTagGet1("<enter>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].doors[i].enter);
                spm::parse::parseTagGet1("<exit>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].doors[i].exit);
                // Door substitution
                spm::dan::dan_wp->dungeons[no].doors[i].enter = doorEntrance;
                spm::dan::dan_wp->dungeons[no].doors[i].exit = doorExit;
                spm::parse::parsePopNext();
                i++;
            }
            spm::dan::dan_wp->dungeons[no].doorCount = i;

            // Enemy generation setup
            i = 0;
            int enemyTypes = 0;
            int arrayRNG = 0;
            int enemyAmt = 0;
            int enemyDifference = 0;
            int enemyMin = 0;
            int enemyMax = 0;
            int spDoorRNG = 0;
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
            // Enemy Substition Protocol
            while (i < enemyTypes)
            {
                // Lv1
                if (no <= 25)
                {
                    enemyGenRNG = spm::system::rand() % 100;
                    if (enemyGenRNG < 70)
                    {
                        arrayRNG = spm::system::rand() % 29;
                        vsOdds = spm::system::rand() % 100;
                        if (lv1Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv1Names[arrayRNG];
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv1SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv1SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                    else if (enemyGenRNG < 90)
                    {
                        arrayRNG = spm::system::rand() % 36;
                        vsOdds = spm::system::rand() % 100;
                        if (lv2Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv2Names[arrayRNG];
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
                            }
                            if (enemyTypes == 1)
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv2SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv2SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                            wii::os::OSReport("Room %d: Enemy %d gen'd %d times @ pos %d. i = %d.\n", no, spm::dan::dan_wp->dungeons[no].enemies[i].name, enemyAmt, spm::dan::dan_wp->dungeons[no].enemies[i].pos, i);
                        }
                    }
                    else
                    {
                        arrayRNG = spm::system::rand() % 33;
                        vsOdds = spm::system::rand() % 100;
                        if (lv3Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv3Names[arrayRNG];
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv3SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv3SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                            wii::os::OSReport("Room %d: Enemy %d gen'd %d times @ pos %d. i = %d.\n", no, spm::dan::dan_wp->dungeons[no].enemies[i].name, enemyAmt, spm::dan::dan_wp->dungeons[no].enemies[i].pos, i);
                        }
                    }
                }
                // Lv2
                else if (no <= 50)
                {
                    enemyGenRNG = spm::system::rand() % 100;
                    if (enemyGenRNG < 20)
                    {
                        arrayRNG = spm::system::rand() % 29;
                        vsOdds = spm::system::rand() % 100;
                        if (lv1Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv1Names[arrayRNG];
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv1SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv1SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                    else if (enemyGenRNG < 80)
                    {
                        arrayRNG = spm::system::rand() % 36;
                        vsOdds = spm::system::rand() % 100;
                        if (lv2Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv2Names[arrayRNG];
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
                            }
                            if (enemyTypes == 1)
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv2SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv2SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                    else
                    {
                        arrayRNG = spm::system::rand() % 33;
                        vsOdds = spm::system::rand() % 100;
                        if (lv3Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv3Names[arrayRNG];
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv3SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv3SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                }
                // Lv3
                else if (no <= 75)
                {
                    enemyGenRNG = spm::system::rand() % 100;
                    if (enemyGenRNG < 20)
                    {
                        arrayRNG = spm::system::rand() % 36;
                        vsOdds = spm::system::rand() % 100;
                        if (lv2Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv2Names[arrayRNG];
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
                            }
                            if (enemyTypes == 1)
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv2SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv2SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                    else if (enemyGenRNG < 80)
                    {
                        arrayRNG = spm::system::rand() % 33;
                        vsOdds = spm::system::rand() % 100;
                        if (lv3Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv3Names[arrayRNG];
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv3SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv3SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                    else
                    {
                        arrayRNG = spm::system::rand() % 26;
                        vsOdds = spm::system::rand() % 100;
                        if (lv4Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv4Names[arrayRNG];
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
                            }
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv4SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv4SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                }
                // Lv4
                else
                {
                    enemyGenRNG = spm::system::rand() % 100;
                    if (enemyGenRNG < 10)
                    {
                        arrayRNG = spm::system::rand() % 29;
                        vsOdds = spm::system::rand() % 100;
                        if (lv1Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv1Names[arrayRNG];
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv1SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv1SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                    else if (enemyGenRNG < 20)
                    {
                        arrayRNG = spm::system::rand() % 36;
                        vsOdds = spm::system::rand() % 100;
                        if (lv2Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv2Names[arrayRNG];
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
                            }
                            if (enemyTypes == 1)
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv2SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv2SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                    else if (enemyGenRNG < 80)
                    {
                        arrayRNG = spm::system::rand() % 33;
                        vsOdds = spm::system::rand() % 100;
                        if (lv3Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv3Names[arrayRNG];
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
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv3SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv3SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                    else
                    {
                        arrayRNG = spm::system::rand() % 26;
                        vsOdds = spm::system::rand() % 100;
                        if (lv4Odds[arrayRNG] > vsOdds)
                        {
                            ++i;
                            spm::dan::dan_wp->dungeons[no].enemies[i].name = lv4Names[arrayRNG];
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
                            }
                            spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                            if (lv4SpR[arrayRNG] == 0)
                            {
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                            }
                            else if (lv4SpR[arrayRNG] == 1)
                            {
                                spDoorRNG = spm::system::rand() % 4;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR1Doors[spDoorRNG];
                            }
                            else
                            {
                                spDoorRNG = spm::system::rand() % 6;
                                spm::dan::dan_wp->dungeons[no].enemies[i].pos = SpR2Doors[spDoorRNG];
                            }
                        }
                    }
                }
            }
            spm::dan::dan_wp->dungeons[100].enemies[0].name = 6;
            spm::dan::dan_wp->dungeons[100].enemies[0].num = 10;
            spm::dan::dan_wp->dungeons[100].enemies[1].name = 5;
            spm::dan::dan_wp->dungeons[100].enemies[1].num = 5;
            spm::dan::dan_wp->dungeons[no].enemyCount = i;

            // Replace Flimm chests
            if (no == 9 || no == 19 || no == 29 || no == 39 || no == 49 || no == 59 || no == 69 || no == 79 || no == 89)
            {
                int poolItem = 0;
                for (i = 0; i < 67; i = i + 3)
                {
                    poolItem = spm::system::rand() % 142;
                    newRotenShopItems[i] = rotenShopItemPool[poolItem];
                    wii::os::OSReport("Flimm array val %d set to %d. poolItem: %d.\n", i, rotenShopItemPool[poolItem], poolItem);
                }
                for (i = 0; i < 70; i++)
                {
                    spm::dan::dan_rotenShopItems[i] = newRotenShopItems[i];
                }
            }
            // Move to next dungeon
            spm::parse::parsePopNext();
        }

        // Free pit text
        spm::parse::parsePop();
        spm::memory::__memFree(0, decompPitText);

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
                                                // else if (msl::string::strcmp(msgName, "m_noroi") == 0)
                                                // back cursya
                                                // return m_noroi;
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
        s32 no = spm::evtmgr_cmd::evtGetValue(entry, args[0]);
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

    static spm::seqdef::SeqFunc *seq_titleMainReal;
    static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
    {
        wii::gx::GXColor notgreen = {230, 116, 216, 255};
        f32 scale = 0.8f;
        const char *msg = "Project SPM Pit Rework Name Pending alpha 1.12";
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
        spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &youSuckDisplay;
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

    static void evtNpcPatchAllEnemies() {
        patch::hookFunction(spm::evt_npc::evt_npc_set_rgba, [](spm::evtmgr::EvtEntry* entry, bool isFirstCall) {
            (void)entry;
            (void)isFirstCall;
            return 2;
        });
    }
    static void danPatchPitEnemies() {
        spm::npcdrv::npcTribes[5].animPoseName = "e_kuribo_redhat"; // need the file
        spm::npcdrv::npcTribes[465].maxHp = 20;
    }

    spm::npcdrv::NPCTribeAnimDef nastasiaAnims[] = {
        {0, "S_1"}, // Standing (idle)
        {3, "T_1"}, // Talking
        {-1, nullptr}
    };

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
    RETURN_FROM_CALL()

    static void addNastasiaToTopOfFlipsideTowerForFun() {
        // Initialize the patches to the EVT interpreter to add custom opcodes
        evtpatch::evtmgrExtensionInit();

        // Get Flipside Tower's init EVT script
        spm::evtmgr_cmd::EvtScriptCode* flipsideInitEvt = spm::map_data::mapDataPtr("mac_02")->initScript;

        // Add a hook at the beginning of Flipside's init EVT script that will play our custom script first
        evtpatch::hookEvt(flipsideInitEvt, 1, flipside_nastasia_funny);
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
        addNastasiaToTopOfFlipsideTowerForFun();
    }
}
