#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_kemuri_test)

USING(spm::effdrv::EffEntry)

EffEntry * effKemuriTestEntry(f32, f32, f32, f32, u32);

CPP_WRAPPER_END()
