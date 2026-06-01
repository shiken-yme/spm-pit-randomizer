/*
    Handles Fracktail/Wracktail bossfight work
*/

#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/npcdrv.h>
#include <spm/evtmgr.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::npc_zunbaba)

USING(spm::npcdrv::NPCEntry)
USING(wii::mtx::Vec3)
USING(wii::mtx::Mtx34)

typedef struct
{
/* 0x0 */ const char * animPoseName;
/* 0x4 */ const char * animDefs[8];
} ZunbabaSegmentAnimNames;
SIZE_ASSERT(ZunbabaSegmentAnimNames, 0x24)

typedef struct
{
/* 0x000 */ u32 unused_0x0; // Might've been flags at one point
/* 0x004 */ s32 segmentId;
/* 0x008 */ s32 segmentType;
/* 0x00C */ s32 animPoseId;
/* 0x010 */ Unk unused_0x10;
/* 0x014 */ s32 antennaLightGroupIdx;
/* 0x010 */ u8 unknown_0x18[0x130 - 0x018];
} ZunbabaSegmentDef;
SIZE_ASSERT(ZunbabaSegmentDef, 0x130)

typedef struct
{
/* 0x00 */ u32 unused_0x0; // Might've been flags at one point
/* 0x04 */ s32 segmentCount;
/* 0x08 */ ZunbabaSegmentDef * segments;
/* 0x0C */ u8 unknown_0x0c[0x7c - 0x0c];
/* 0x7C */ NPCEntry * npcEntry;
/* 0x80 */ u8 unknown_0x80[0x84 - 0x80];
} ZunbabaWork;
SIZE_ASSERT(ZunbabaWork, 0x84)

DECOMP_STATIC(ZunbabaWork * zunbaba_wp)
DECOMP_STATIC(ZunbabaSegmentAnimNames npcZunbabaSegmentAnimNames[8]) // Fracktail
DECOMP_STATIC(ZunbabaSegmentAnimNames npcZanbabaSegmentAnimNames[8]) // Wracktail

s32 npcZunbabaSegmentDispCb(ZunbabaSegmentDef * segment, s32 grpIdx, Mtx34 mtx);
s32 npcZunbabaHeadDispCb(ZunbabaSegmentDef * segment, s32 grpIdx, Mtx34 mtx);
void func_801da938(s32 p1, NPCEntry * npcEntry);
void func_801e0678(s32 type);

EVT_DECLARE_USER_FUNC(npc_zunbaba_init_work, 0)

CPP_WRAPPER_END()
