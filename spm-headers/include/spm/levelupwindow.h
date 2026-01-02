#pragma once

#include <common.h>
#include <spm/effdrv.h>
#include <spm/camdrv.h>
#include <spm/pausewin.h>
#include <spm/dispdrv.h>

CPP_WRAPPER(spm::levelupwindow)

USING(spm::effdrv::EffEntry)
USING(spm::camdrv::CameraId)
USING(spm::pausewin::PausewinEntry)
USING(spm::dispdrv::DispCallback)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x4 - 0x0]; // Likely unused u16s
/* 0x04 */ EffEntry * eff;
/* 0x08 */ s32 state;
/* 0x0C */ s32 pausewinId;
/* 0x10 */ s32 maxHpIncrease;
/* 0x14 */ s32 attackIncrease;
/* 0x18 */ s32 backdropAlpha;
/* 0x1C */ s32 sfxId;
} LevelUpWindowWork;
SIZE_ASSERT(LevelUpWindowWork, 0x20)

DECOMP_STATIC(LevelUpWindowWork * levelupwindow_wp)
DECOMP_STATIC(LevelUpWindowWork levelUpWindowWork)

// Controls how long levelupwindow_wp->state is at 2 before either switching to state 3 or the window closing
DECOMP_STATIC(s32 levelUpWindowState2Timer)

void levelUpWindowInit();
void levelUpDispMaxHpIncrease(PausewinEntry * win);
void levelUpDispAttackIncrease(PausewinEntry * win);
DispCallback levelUpDispBackdrop;
void levelUpWindowMain();
bool levelUpWindowCheckIfCanDisplay();
f32 levelUpGetBackdropAlpha();

CPP_WRAPPER_END()
