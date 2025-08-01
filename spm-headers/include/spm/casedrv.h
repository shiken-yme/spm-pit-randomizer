/*
    casedrv runs scripts when the player interacts with certain HitObjs
*/

#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/hitdrv.h>

CPP_WRAPPER(spm::casedrv)

USING(spm::evtmgr::EvtScriptCode)
USING(spm::hitdrv::HitObj)

enum CaseType
{
    CASE_STAND_ON = 0,           /* standing on obj */
    CASE_RUN_UNTIL_STAND_ON = 1, /* unused; always runs unless standing on target hitobj */
    CASE_TALK = 2,               /* strict "Up" interaction i.e. talking to NPCs */
    CASE_DOWN = 3,               /* i.e. going down pipes */
    CASE_STAND_BY = 4,           /* activates upon touching side of obj while grounded */
    CASE_UP = 5,                 /* i.e. entering 2D door or reading sign */
    CASE_HIT_BOTTOM = 6,         /* i.e. mirror hall blocks upside-down pipes */
    CASE_HIT_BOTTOM_HOLD_UP = 7, /* unused; like case 6 but must hold Up */
    CASE_CUDGE_WALL = 8,         /* unused; hammering a vertical plane like a wall or door */
    CASE_CUDGE_FLOOR = 9,        /* hammering a horizontal plane like 7-3 floor to get apples */
    CASE_HOLD_SIDE = 10,         /* i.e. entering side pipes and 3D doors */
    CASE_BOOMER = 11,            /* i.e. cracks in walls */
    CASE_FIRE = 12,              /* i.e. torches in 7-2 8-1 */
    CASE_THUDLEY = 13,           /* i.e. yellow posts */
    CASE_SPACE_SWIM_TO = 14,     /* i.e. entering black hole */
    CASE_SPACE_HIT = 15,         /* i.e. hit by hedron in 4-3 */
    CASE_SPACE_SHOOT_64 = 16,    /* i.e. shooting a space rock 64 times */
    CASE_SPACE_SHOOT_256 = 17,   /* i.e. shooting a hedron 256 times */
    CASE_MEGA_STAR = 18,         /* i.e. breaking pipes while in mega star state */
    CASE_FLIP_2D = 19,           /* flipping to 2D */
    CASE_FLIP_3D = 20            /* flipping to 3D */
};

typedef struct
{
    /* 0x00 */ u16 flags;
    /* 0x02 */ u8 unknown_0x2[0x4 - 0x2];
    // Touching a HitObj with either name triggers the script
    /* 0x04 */ const char *name;
    /* 0x08 */ const char *name2;
    /* 0x0C */ u8 unknown_0xc[0x10 - 0xc];
    /* 0x10 */ s32 lw[16]; // copied into script's LW on run
    /* 0x50 */ u8 unknown_0x50[0x54 - 0x50];
    /* 0x54 */ EvtScriptCode *script;
    /* 0x58 */ s32 scriptPriority;
} CaseEntDef;
SIZE_ASSERT(CaseEntDef, 0x5c)

// Script running this frame
#define CASE_FLAG_RUN 0x10
// Entry allocated for a case
#define CASE_FLAG_IN_USE 1

typedef struct
{
    /* 0x00 */ u16 flags; // CASE_FLAG defines
    /* 0x02 */ u8 unknown_0x2[0x4 - 0x2];
    /* 0x04 */ s32 type; // controls the action & type of HitObj required
    /* 0x08 */ char name[64];
    /* 0x48 */ char name2[64];
    /* 0x88 */ s32 id;
    /* 0x8C */ u32 def_0xc; // 0xc-f from CaseEntDef
    /* 0x90 */ s32 lw[16];
    /* 0xD0 */ s32 evtId;
    /* 0xD4 */ EvtScriptCode *script;
    /* 0xD8 */ s32 scriptPriority;
} CaseEntry;
SIZE_ASSERT(CaseEntry, 0xdc)

typedef struct
{
    /* 0x0 */ s32 n;
    /* 0x4 */ CaseEntry *entries; // array of n length
    /* 0x8 */ u8 unknown_0x8[0x10 - 0x8];
} CaseWork;
SIZE_ASSERT(CaseWork, 0x10)

DECOMP_STATIC(CaseWork casedrv_work)
DECOMP_STATIC(CaseWork *casedrv_wp)

/*
    Allocates entries
*/
void caseInit();

/*
    Clears entries
*/
void caseReInit();

/*
    Creates an entry from a CaseEntDef and returns its id
*/
s32 caseEntry(CaseEntDef *def);

/*
    Deletes an entry by id
*/
void caseDelete(s32 id);

UNKNOWN_FUNCTION(func_80059268)

/*
    Updates all entries
*/
void caseMain();

/*
    Returns an entry by its HitObj
*/
CaseEntry *caseCheckHitObj(HitObj *hitObj);
CaseEntry *caseCheckHitObj2(HitObj *hitObj);

/*
    Returns an entry by id
*/
CaseEntry *caseIdToPtr(s32 id);

UNKNOWN_FUNCTION(func_8005adec)
UNKNOWN_FUNCTION(func_8005ae08)
UNKNOWN_FUNCTION(func_8005ae24)
UNKNOWN_FUNCTION(func_8005ae64)

CPP_WRAPPER_END()
