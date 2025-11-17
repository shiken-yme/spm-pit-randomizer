#include <common.h>
#include <evt_cmd.h>
#include <gen.h>
#include <util.h>
#include <cutscene_helpers.h>
#include <evtpatch.h>
#include <tplpatch.h>
#include <sndpatch.h>
#include <lunatic/localize.h>
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
    char cardNameBuffer[64];
    s32 _shadooHealth = 0;
    s32 *shadooHealth = &_shadooHealth;

    const char *msgSearchTribeToTattle(spm::npcdrv::NPCEntry *npc, s32 tribeId, Tribe2Tattle_Types type)
    {
        spm::npcdrv::NPCTribe *tribe = spm::npcdrv::npcGetTribe(tribeId);
        char tattleBuffer[512];
        char cardDescBuffer[512];
        if (npc != nullptr || npc != 0)
        {
            if (msl::string::strstr(npc->name, "rebear") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, tattle_merluna);
            }
            if (msl::string::strstr(npc->name, "dan_card") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, spm::msgdrv::msgSearch("mac_19_card"));
            }
            if (msl::string::strstr(npc->name, "mover") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, tattle_mover);
            }
            if (msl::string::strstr(npc->name, "dan_koburon") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, tattle_whacka);
            }
            if (msl::string::strstr(npc->name, "jimbo") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, tattle_jimbo);
            }
        }
        switch (tribeId)
        {
        case 2:
            msl::stdio::sprintf(tattleBuffer, tattle_hyper_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_hyper_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Hyper Goomba");
            break;
        case 5:
            msl::stdio::sprintf(tattleBuffer, tattle_spiked_gloomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spiked_gloomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spiked Gloomba");
            break;
        case 6:
            msl::stdio::sprintf(tattleBuffer, tattle_spiked_hyper_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spiked_hyper_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spiked Hyper Goomba");
            break;
        case 8:
            msl::stdio::sprintf(tattleBuffer, tattle_hyper_paragoomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_hyper_paragoomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Hyper Paragoomba");
            break;
        case 10:
            msl::stdio::sprintf(tattleBuffer, tattle_kamikaze_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_kamikaze_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Kamikaze Goomba");
            break;
        case 16:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Koopa");
            break;
        case 19:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_koopatrol, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_koopatrol, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Koopatrol");
            break;
        case 24:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_paratroopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_paratroopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Paratroopa");
            break;
        case 29:
            msl::stdio::sprintf(tattleBuffer, tattle_red_spike_top, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_red_spike_top, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Red Spike Top");
            break;
        case 38:
            msl::stdio::sprintf(tattleBuffer, tattle_sky_blue_spiny, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_sky_blue_spiny, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Sky-Blue Spiny");
            break;
        case 43:
        case 44:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_bones, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_bones, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Bones");
            break;
        case 49:
        case 50:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_hammer_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_hammer_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Hammer Bro");
            break;
        case 55:
        case 56:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_boomerang_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_boomerang_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Boomerang Bro");
            break;
        case 61:
        case 62:
            msl::stdio::sprintf(tattleBuffer, tattle_ice_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_ice_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Ice Bro");
            break;
        case 66:
        case 67:
        case 68:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Magikoopa");
            break;
        case 75:
        case 76:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_striker, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_striker, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Striker");
            break;
        case 86:
            msl::stdio::sprintf(tattleBuffer, tattle_bomb_boo, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_bomb_boo, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Bomb Boo");
            break;
        case 95:
            msl::stdio::sprintf(tattleBuffer, tattle_white_clubba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_white_clubba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "White Clubba");
            break;
        case 98:
            msl::stdio::sprintf(tattleBuffer, tattle_green_fuzzy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_green_fuzzy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Green Fuzzy");
            break;
        case 101:
            msl::stdio::sprintf(tattleBuffer, tattle_hyper_cleft, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_hyper_cleft, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Hyper Cleft");
            break;
        case 111:
        case 112:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_puff, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_puff, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Puff");
            break;
            case 124:
            msl::stdio::sprintf(tattleBuffer, tattle_gold_chomp, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_gold_chomp, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Gold Chomp");
            break;
        case 167:
            msl::stdio::sprintf(tattleBuffer, tattle_tileoid_pu, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_tileoid_pu, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Tileoid PU");
            break;
        case 171:
            msl::stdio::sprintf(tattleBuffer, tattle_bawbus, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_bawbus, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Bawbus");
            break;
        case 220:
        case 221:
        case 222:
            msl::stdio::sprintf(tattleBuffer, tattle_ninjeremiah, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_ninjeremiah, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Ninjeremiah");
            break;
        case 225:
            msl::stdio::sprintf(tattleBuffer, tattle_skellobyte, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_skellobyte, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Skellobyte");
            break;
        case 228:
            msl::stdio::sprintf(tattleBuffer, tattle_spiky_skellobyte, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spiky_skellobyte, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spiky Skellobyte");
            break;
        case 330:
            msl::stdio::sprintf(tattleBuffer, tattle_phantom_mario, *shadooHealth, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_phantom_mario, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Phantom Mario");
            break;
        case 332:
            msl::stdio::sprintf(tattleBuffer, tattle_phantom_peach, *shadooHealth, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_phantom_peach, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Phantom Peach");
            break;
        case 333:
            msl::stdio::sprintf(tattleBuffer, tattle_phantom_bowser, *shadooHealth, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_phantom_bowser, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Phantom Bowser");
            break;
        case 331:
            msl::stdio::sprintf(tattleBuffer, tattle_phantom_luigi, *shadooHealth, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_phantom_luigi, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Phantom Luigi");
            break;
        case 446:
            msl::stdio::sprintf(tattleBuffer, tattle_ash_cherbil, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_ash_cherbil, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Ash Cherbil");
            break;
        case 465:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Koopa");
            break;
        case 466:
            msl::stdio::sprintf(tattleBuffer, tattle_flip_shady_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_flip_shady_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Koopa");
            break;
        case 470:
            msl::stdio::sprintf(tattleBuffer, tattle_flip_buzzy_beetle, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_flip_buzzy_beetle, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Buzzy Beetle");
            break;
        case 471:
            msl::stdio::sprintf(tattleBuffer, tattle_flip_spike_top, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_flip_spike_top, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spike Top");
            break;
        case 473:
        case 474:
        case 475:
            msl::stdio::sprintf(tattleBuffer, tattle_green_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_green_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Green Magikoopa");
            break;
        case 476:
        case 477:
        case 478:
            msl::stdio::sprintf(tattleBuffer, tattle_white_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_white_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "White Magikoopa");
            break;
        case 479:
        case 480:
        case 481:
            msl::stdio::sprintf(tattleBuffer, tattle_red_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_red_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Red Magikoopa");
            break;
        case 495:
            msl::stdio::sprintf(tattleBuffer, tattle_spinia, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spinia, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spinia");
            break;
        case 496:
            msl::stdio::sprintf(tattleBuffer, tattle_spunia, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spunia, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spunia");
            break;
        case 504:
        case 505:
            msl::stdio::sprintf(tattleBuffer, tattle_bleepboxer, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_bleepboxer, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Bleepboxer");
            break;
        case 506:
            msl::stdio::sprintf(tattleBuffer, tattle_kilo_muth, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_kilo_muth, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Kilo Muth");
            break;
        case 529:
            msl::stdio::sprintf(tattleBuffer, tattle_yellow_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_yellow_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shy Guy");
            break;
        case 530:
            msl::stdio::sprintf(tattleBuffer, tattle_green_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_green_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shy Guy");
            break;
        case 531:
            msl::stdio::sprintf(tattleBuffer, tattle_blue_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_blue_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shy Guy");
            break;
        case 532:
            msl::stdio::sprintf(tattleBuffer, tattle_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shy Guy");
            break;
        case 534:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_lakitu, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_lakitu, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Lakitu");
            break;
        }
        if (type == TATTLE)
        {
            msl::stdio::sprintf(spm::search::search_wp->msgBuf, "<gsearch>\n%s", tattleBuffer);
            const char *tattleMsg = spm::search::search_wp->msgBuf;
            return tattleMsg;
        }
        else if (type == CARD_NAME)
        {
            const char *nameMsg = cardNameBuffer;
            return nameMsg;
        }
        else
        {
            msl::stdio::sprintf(spm::pausewin::pausewinCardDescBuf, cardDescBuffer);
            const char *descMsg = spm::pausewin::pausewinCardDescBuf;
            return descMsg;
        }
    }

}