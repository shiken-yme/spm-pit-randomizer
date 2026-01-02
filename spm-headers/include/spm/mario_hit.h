#pragma once

#include <common.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::mario_hit)

USING(wii::mtx::Vec3)

void clear_hitobj_ride();
f32 marioGetWidth();
f32 marioGetHeight();
void func_80133dbc();
void marioJumpNpc(f32 bounceEjection, f32 jumpEjection, Vec3 * p3, s32 p4, s32 p5);

// more

CPP_WRAPPER_END()