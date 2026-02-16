#pragma once

#include <common.h>
#include <wii/mtx.h>

// TODO: probably called statuswindow

CPP_WRAPPER(spm::hud)

USING(wii::mtx::Vec2)

typedef void (CountdownDoneCb)();

typedef struct
{
/* 0x000 */ u32 flags;
/* 0x004 */ u8 unknown_0x004[0x10 - 0x004];
/* 0x010 */ f32 hudMoveProgress;
/* 0x014 */ f32 hudMoveLastShownYPos;
/* 0x018 */ f32 hudMoveTarget;
/* 0x01C */ f32 unknown_0x1c;
/* 0x020 */ Vec2 basePos;
/* 0x004 */ u8 unknown_0x028[0x178 - 0x028];
/* 0x178 */ s32 countdownTimer;
/* 0x17C */ void * countdownCallback;
/* 0x180 */ u8 unknown_0x180[0x190 - 0x180];
} HudWork;
SIZE_ASSERT(HudWork, 0x190)

DECOMP_STATIC(HudWork * hud_wp)

void hudInit();
void hudReInit();
void hudLoadStats();
void hudMain();
void hudGetPos(f32 * x, f32 * y);
void hudUnhideAlt();
void hudHide();
void func_80199b0c();
void func_80199b5c();
void hudUnhide();
void hudUnblockDisp();
void hudBlockDisp();
void func_80199c9c();
void func_80199cb0();
void hudStartCountdown(u32 length, CountdownDoneCb * cb);
bool hudCheckStatsDesynced();
void hudUpdateStats();
void hudDisp();
void func_8019af88();
void hudTurnOffFlipTimeBox(s32 idx);
void func_8019b0dc();
void func_8019be84();
void func_8019bea8(f32 p1);

CPP_WRAPPER_END()
