#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_spm_hit)

USING(spm::effdrv::EffEntry)

EffEntry * effSpmHitEntry(float x, float y, float z, s32 damage);

CPP_WRAPPER_END()
