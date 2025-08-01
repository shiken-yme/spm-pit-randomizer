#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::mi4)

void mi4MimiHolographicEffect(wii::gx::GXTexObj * tex, spm::evtmgr::EvtEntry * evtEntry);
// more

CPP_WRAPPER_END()