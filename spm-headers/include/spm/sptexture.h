#pragma once

#include <common.h>
#include <wii/gx.h>
#include <spm/filemgr.h>

CPP_WRAPPER(spm::sptexture)

USING(wii::gx::GXTexObj)
USING(spm::filemgr::FileEntry)

typedef struct
{
/* 0x0 */ bool loaded;
/* 0x1 */ u8 unknown_0x1[0x8 - 0x1];
/* 0x8 */ FileEntry * tpl;
/* 0xC */ u8 unknown_0xc[0x10 - 0xc];
} SptextureWork;
SIZE_ASSERT(SptextureWork, 0x10)

extern SptextureWork * sptexture_wp;
extern SptextureWork sptexture_work;

extern void * sptexture_image;

void sptextureInit();
UNKNOWN_FUNCTION(sptextureSetUnusedBool)
void sptextureMain();
void sptextureGet(u32 id, GXTexObj *dest);
UNKNOWN_FUNCTION(sptextureIsLoaded)

CPP_WRAPPER_END()
