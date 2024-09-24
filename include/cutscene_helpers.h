#pragma once
#include <spm/evt_npc.h>

namespace mod::cutscene_helpers
{

#define SET_CHAR_ANIMS(instanceName, tribeAnimDef) \
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR(instanceName), spm::npcdrv::NPCProperty::ANIMS, PTR(tribeAnimDef))

typedef enum {
    ANIMS = 0xe
} NPCProperty;

#define SPAWN_CHARACTER(instanceName, modelName, initialAnimations) \
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR(instanceName), PTR(modelName), 0) \
    SET_CHAR_ANIMS(instanceName, initialAnimations)

#define NPC_SET_ANIM_WAIT(npcName, animId) \
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(npcName), animId, 1) \
    USER_FUNC(spm::evt_npc::evt_wait_anim_end, PTR(npcName), 1)

#define NPC_SET_ANIMDEF_AND_ANIM(npcName, animDef, animId) \
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR(npcName), NPC_PROPERTY_ANIM_DEF, PTR(animDef)) \
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(npcName), animId, 1)

#define NPC_SET_ANIMDEF_AND_ANIM_WAIT(npcName, animDef, animId) \
    NPC_SET_ANIMDEF_AND_ANIM(npcName, animDef, animId) \
    USER_FUNC(spm::evt_npc::evt_wait_anim_end, PTR(npcName), 1)

#define NPC_SET_ANIMDEF_AND_ANIM_LOOP(npcName, animDef, animInId, animLoopId) \
    NPC_SET_ANIMDEF_AND_ANIM_WAIT(npcName, animDef, animInId) \
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR(npcName), animLoopId, 1)

#define NPC_FACEDIR_LEFT -1
#define NPC_FACEDIR_RIGHT 1
#define NPC_FACE_DIR(npcName, moveDir) \
    USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR(npcName), moveDir) \
    USER_FUNC(spm::evt_npc::evt_npc_wait_axis_movement_unit_end, PTR(npcName))


#define REACTION_EXCLAMATION 0
#define REACTION_QUESTION 1

#define MARIO_SPAWN_EXCLAMATION_MARK() \
    USER_FUNC(spm::evt_eff::evt_eff_exclamation_question, 0, 0, 0, REACTION_EXCLAMATION, 0, 0, 0, 0, 0, 0, 0)
#define MARIO_SPAWN_QUESTION_MARK() \
    USER_FUNC(spm::evt_eff::evt_eff_exclamation_question, 0, 0, 0, REACTION_QUESTION, 0, 0, 0, 0, 0, 0, 0)

#define NPC_SPAWN_EXCLAMATION_OR_QUESTION_MARK(npcName, exclamationOrQuestion, sfxName) \
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR(npcName), LW(0), LW(1), LW(2)) \
    ADD(LW(1), 80) \
    ADD(LW(2), 10) \
    USER_FUNC(spm::evt_eff::evt_eff_exclamation_question, 2, 0, 0, exclamationOrQuestion, 0, LW(0), LW(1), LW(2), 0, 0, 0) \
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR(sfxName), PTR(npcName))

#define NPC_SPAWN_EXCLAMATION_MARK(npcName) \
    NPC_SPAWN_EXCLAMATION_OR_QUESTION_MARK(npcName, REACTION_EXCLAMATION, "SFX_SYS_BIKKURI1")
#define NPC_SPAWN_QUESTION_MARK(npcName) \
    NPC_SPAWN_EXCLAMATION_OR_QUESTION_MARK(npcName, REACTION_QUESTION, "SFX_SYS_HATENA1")

#define WAIT_FOR_EVT(lwId, lwRes) \
    DO(0) \
        CHK_EVT(lwId, lwRes) \
        IF_EQUAL(LW(1), 0) \
            DO_BREAK() \
        END_IF() \
        WAIT_FRM(1) \
    WHILE()

}