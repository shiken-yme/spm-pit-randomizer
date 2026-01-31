#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_spm_levelup)

USING(spm::effdrv::EffEntry)

EffEntry * effSpmLevelupEntry(f32 x, f32 y, s32 unused);

s32 func_800abfe4(EffEntry * eff);

CPP_WRAPPER_END()
