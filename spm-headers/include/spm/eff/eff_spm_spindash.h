#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_spm_spindash)

USING(spm::effdrv::EffEntry)

EffEntry * effSpmSpindashEntry(f32 x, f32 y, f32 z, f32 scale, s32 unused_p5);

void effSpmSpindashSetTargetMario(EffEntry * eff);

CPP_WRAPPER_END()
