#pragma once

#include <common.h>
#include <spm/npcdrv.h>

CPP_WRAPPER(spm::search)

typedef struct
{
/* 0x000 */ u8 unknown_0x000[0x034 - 0x000];
/* 0x034 */ char * message;
/* 0x038 */ char msgBuf[32];
/* 0x058 */ u8 unknown_0x058[0x4a8 - 0x058];
} SearchWork;
SIZE_ASSERT(SearchWork, 0x4a8)

DECOMP_STATIC(SearchWork * search_wp)

const char *searchGetNpcMsg(spm::npcdrv::NPCEntry *npc);

CPP_WRAPPER_END()