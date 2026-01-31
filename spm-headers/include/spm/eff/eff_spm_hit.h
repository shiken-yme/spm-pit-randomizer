#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_spm_hit)

USING(spm::effdrv::EffEntry)

EffEntry * effSpmHitEntry(f32 x, f32 y, f32 z, s16 param_4);

CPP_WRAPPER_END()
