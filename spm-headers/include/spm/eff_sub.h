#pragma once

#include <common.h>
#include <spm/filemgr.h>
#include <wii/mtx.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::eff_sub)

USING(spm::filemgr::FileEntry)
USING(wii::gx::GXColor)
USING(wii::mtx::Mtx34)

typedef struct
{
/* 0x00 */ u8 unknown_0x00[0x40 - 0x00];
} EffdataHeader;
SIZE_ASSERT(EffdataHeader, 0x40)

typedef struct
{
/* 0x000 */ FileEntry * activeTplFile;
/* 0x004 */ FileEntry * activeDatFile;
/* 0x008 */ void * datBody;
/* 0x00C */ FileEntry * effdataTplFile;
/* 0x010 */ FileEntry * effdataDatFile;
/* 0x014 */ u8 unknown_0x014[0x78 - 0x14];
/* 0x078 */ f32 dispBoundaryOriginX;
/* 0x07C */ f32 dispBoundaryOriginZ;
/* 0x080 */ f32 dispBoundaryWidthX;
/* 0x084 */ f32 dispBoundaryWidthZ;
/* 0x088 */ u8 unknown_0x088[0xe0 - 0x88];
/* 0x0E0 */ bool overrideEffColor;
/* 0x0E1 */ GXColor overrideEffColMask1;
/* 0x0E5 */ GXColor overrideEffColMask2;
/* 0x0E9 */ u8 unknown_0x0e9[0x100 - 0xe9];
} EffSubWork;
SIZE_ASSERT(EffSubWork, 0x100)

DECOMP_STATIC(EffSubWork * effsub_wp)

void effSubMain();
void func_8005c47c(f32 p1, EffdataHeader * p2, s32 p3, Mtx34 p4, u8 p5, s32 p6);
void effSubDisp(u32 handle, Mtx34 mtx, double time);
void effSubSetColorOverride(bool enable, GXColor * col1, GXColor * col2);

CPP_WRAPPER_END()
