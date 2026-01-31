#pragma once

#include <common.h>
#include <spm/effdrv.h>
#include <wii/mtx.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::eff_damage_star)

USING(spm::effdrv::EffEntry)

EffEntry * effDamageStarEntry(f32 x, f32 y, f32 z, s32 variant, s32 damage);

CPP_WRAPPER_END()
