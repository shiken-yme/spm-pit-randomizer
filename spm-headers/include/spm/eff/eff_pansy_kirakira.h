#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_pansy_kirakira)

USING(spm::effdrv::EffEntry)

EffEntry * effPansyKirakiraEntry(s32 variant);

CPP_WRAPPER_END()
