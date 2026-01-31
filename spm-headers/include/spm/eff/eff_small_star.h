#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_small_star)

USING(spm::effdrv::EffEntry)

EffEntry * effSmallStarEntry(f32 x, f32 y, f32 z, f32 p4, f32 p5, f32 p6, s32 p7, s32 p8);

CPP_WRAPPER_END()
