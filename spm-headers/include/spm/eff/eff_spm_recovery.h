#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_spm_recovery)

USING(spm::effdrv::EffEntry)

EffEntry * effSpmRecoveryEntry(f32, f32, f32, s32);

void func_80091794(EffEntry * eff, s32 evtId, const char * npcName);

CPP_WRAPPER_END()
