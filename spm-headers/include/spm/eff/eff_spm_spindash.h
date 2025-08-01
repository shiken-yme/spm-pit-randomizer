#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_spm_spindash)

// ...

spm::effdrv::EffEntry * effSpmSpindashEntry(f32, f32, f32, f32, s32);

void effSpmSpindashUpdateUnkEff(spm::effdrv::EffEntry * eff);

// ...

CPP_WRAPPER_END()
