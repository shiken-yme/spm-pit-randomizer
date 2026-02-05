#pragma once

#include <common.h>
#include <spm/npcdrv.h>
#include <spm/evtmgr.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::npc_tile)

USING(spm::npcdrv::NPCEntry)

USING(wii::mtx::Vec3)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x14 - 0x0];
/* 0x14 */ Vec3 rotationLimiter;
/* 0x20 */ u8 unknown_0x20[0x5c - 0x20];
/* 0x5C */ f32 rotationSpeedScale;
/* 0x60 */ f32 rotationSpeed1;
/* 0x64 */ f32 rotationSpeed2;
/* 0x68 */ u8 unknown_0x68[0xf0 - 0x68];
} TileoidWork;
SIZE_ASSERT(TileoidWork, 0xf0)

void npc_tile_init_work(spm::evtmgr::EvtEntry * evtEntry);

s32 npc_tile_setup_work(spm::evtmgr::EvtEntry * evtEntry);

EVT_DECLARE(npc_tile_common_onspawn_evt)

CPP_WRAPPER_END()
