#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_3d_switch)

USING(spm::effdrv::EffEntry)

EffEntry * eff3dSwitchEntry(f32 x, f32 y, f32 z, u32 variant);

CPP_WRAPPER_END()
