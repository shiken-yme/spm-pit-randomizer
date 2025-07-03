#pragma once

#include <common.h>
#include <wii/os.h>
#include <wii/mtx.h>
#include <wii/gx.h>
#include <spm/filemgr.h>

CPP_WRAPPER(spm::animdrv)

USING(wii::os::OSTime)
USING(wii::mtx::Mtx34)
USING(wii::gx::GXColor)
USING(spm::filemgr::FileEntry)

typedef void (AnimPoseDisplayCb)(void * param, s32 animGroupIdx, s32 param_3);

typedef struct 
{
/* 0x00 */ char name[60];
/* 0x3C */ void * data;
} AnimationModelFileAnimTableEntry;
SIZE_ASSERT(AnimationModelFileAnimTableEntry, 0x40)

typedef struct 
{
/* 0x000 */ u8 unknown_0x0[0x148 - 0x0];
/* 0x148 */ u32 animCount;
/* 0x14C */ u8 unknown_0x14c[0x1ac - 0x14c];
/* 0x1AC */ AnimationModelFileAnimTableEntry * anims;
} AnimationModelFileHeader;
SIZE_ASSERT(AnimationModelFileHeader, 0x1b0)

typedef struct
{
/* 0x00 */ s32 inUse;
/* 0x04 */ s32 refCnt;
/* 0x08 */ FileEntry * file;
} TextureGroup;
SIZE_ASSERT(TextureGroup, 0xc)

typedef struct
{
/* 0x00 */ s32 inUse;
/* 0x04 */ s32 refCnt;
/* 0x08 */ FileEntry * file;
/* 0x0C */ s32 textureGroupId;
} AnimGroup;
SIZE_ASSERT(AnimGroup, 0x10)

typedef struct
{
/* 0x000 */ AnimGroup * animGroups;
/* 0x004 */ s32 animGroupNum;
/* 0x008 */ TextureGroup * textureGroups;
/* 0x00C */ s32 textureGroupNum;
/* 0x008 */ u8 unknown_0x8[0x110 - 0x010];
} AnimWork; // Uncertain size
SIZE_ASSERT(AnimWork, 0x110)

DECOMP_STATIC(AnimWork * animdrv_wp)

typedef struct 
{
/* 0x000 */ u32 flag;
/* 0x004 */ s32 paperPoseFlag; // typeFlags
/* 0x008 */ Unk paperPoseSharedCnt; // refCount
/* 0x00C */ s32 releaseType;
/* 0x010 */ s32 animGroupId;
/* 0x014 */ Unk curAnimIndex;
/* 0x018 */ u8 unknown_0x18[0x020 - 0x018];
/* 0x020 */ f32 frmCounter; // unconfirmed
/* 0x024 */ u8 unknown_0x24[0x188 - 0x024];
} AnimPose;
SIZE_ASSERT(AnimPose, 0x188)

AnimWork * animGetPtr();
OSTime animTimeGetTime();
DECOMP_STATIC(void * animdrv_testAlloc(u32 size))
void animInit();
void animMain();
UNKNOWN_FUNCTION(textureGroupEntry)
UNKNOWN_FUNCTION(animGroupEntry)
UNKNOWN_FUNCTION(animPoseRefresh)
s32 animPoseEntry(const char * filename, s32 releaseType);
UNKNOWN_FUNCTION(animPaperPoseEntry)
UNKNOWN_FUNCTION(func_80042ec8)
UNKNOWN_FUNCTION(animPosePeraOff)
UNKNOWN_FUNCTION(animPoseSetLocalTimeRate)
UNKNOWN_FUNCTION(animPoseSetLocalTime)
UNKNOWN_FUNCTION(animPoseSetStartTime)
void animPoseSetAnim(s32 id, const char * animName, bool forceReset);
UNKNOWN_FUNCTION(animPaperPoseGetId)
UNKNOWN_FUNCTION(animPoseSetPaperAnimGroup)
UNKNOWN_FUNCTION(animPoseSetPaperAnim)
UNKNOWN_FUNCTION(func_80043b90)
UNKNOWN_FUNCTION(func_80043ca4)
void animPoseSetEffect(s32 animPoseId, char *name, u32 effect);
UNKNOWN_FUNCTION(animPoseSetEffectAnim)
UNKNOWN_FUNCTION(animPoseSetGXFunc)
f32 animPoseGetLoopTimes(s32 animPoseId);
UNKNOWN_FUNCTION(animPoseSetFlagF0On)
UNKNOWN_FUNCTION(animPoseSetFlagF0Off)
UNKNOWN_FUNCTION(animPoseSetFlagF4On)
UNKNOWN_FUNCTION(animPoseSetFlagF4Off)
UNKNOWN_FUNCTION(animPoseSetMaterialLightFlagOn)
UNKNOWN_FUNCTION(animPoseSetMaterialLightFlagOff)

void animPoseSetMaterialFlagOn(s32 animPoseId, u32 flag);

void animPoseSetMaterialFlagOff(s32 animPoseId, u32 flag);

UNKNOWN_FUNCTION(animPoseSetMaterialAnmColor)

GXColor animPoseGetMaterialEvtColor(s32 animPoseId);

u32 animPoseGetMaterialFlag(s32 animPoseId);

UNKNOWN_FUNCTION(animPoseGetFlagF4)

void animPoseSetMaterialEvtColor(s32 animPoseId, GXColor color);

void animPoseSetDispCallback2(s32 id, void * func, void * evt);

UNKNOWN_FUNCTION(func_800451c4)
void animPoseMain(s32 id);
UNKNOWN_FUNCTION(pushGXModelMtx_TransformNode__)
UNKNOWN_FUNCTION(pushGXModelMtx_JointNode__)
UNKNOWN_FUNCTION(func_80047170)
UNKNOWN_FUNCTION(animSetMaterial_Texture)
UNKNOWN_FUNCTION(animSetMaterial_ChangeTexture)
UNKNOWN_FUNCTION(materialProc)
UNKNOWN_FUNCTION(renderProc)
UNKNOWN_FUNCTION(dispProc)
UNKNOWN_FUNCTION(animPoseDraw)
UNKNOWN_FUNCTION(_animPoseDrawMtx)
void animPoseDrawMtx(s32 id, Mtx34 * mtx, s32 xluStage, f32 rotY, f32 scale);
UNKNOWN_FUNCTION(animSetPaperTexObj)
s32 animPoseRelease(s32 id);
UNKNOWN_FUNCTION(animPaperPoseRelease)
void animPoseAutoRelease(s32 releaseType);
UNKNOWN_FUNCTION(animPaperPoseDisp)
UNKNOWN_FUNCTION(animPaperPoseDispSub)
UNKNOWN_FUNCTION(animPoseDisp_MakeExtTexture)
UNKNOWN_FUNCTION(animSetPaperTexMtx)
u32 animGroupBaseAsync(const char * animPoseName, s32 param_2, void * readDoneCb);
AnimPose * animPoseGetAnimPosePtr(s32 animPoseId);
UNKNOWN_FUNCTION(animPoseGetAnimDataPtr)
AnimationModelFileHeader * animPoseGetAnimBaseDataPtr(s32 id);
UNKNOWN_FUNCTION(animPoseGetCurrentAnim)
UNKNOWN_FUNCTION(func_8004ca08)
UNKNOWN_FUNCTION(func_8004caec)
UNKNOWN_FUNCTION(func_8004cd30)
UNKNOWN_FUNCTION(func_8004cfa8)
UNKNOWN_FUNCTION(evalProc)
UNKNOWN_FUNCTION(animPoseWorldPositionEvalOn)
UNKNOWN_FUNCTION(animPoseWorldMatrixEvalOn)
void animPoseSetDispCallback(s32 animPoseId, AnimPoseDisplayCb * cb, void * param);
UNKNOWN_FUNCTION(animPoseDrawShape)
UNKNOWN_FUNCTION(func_8004d96c)
UNKNOWN_FUNCTION(animPoseGetShapeIdx)
s32 animPoseGetGroupIdx(s32 id, const char * name);
UNKNOWN_FUNCTION(animPoseGetGroupIdxSubname)

CPP_WRAPPER_END()