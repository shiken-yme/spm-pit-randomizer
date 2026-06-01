#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/acdrv.h>

namespace mod::acpatch {
    using namespace spm;
    using namespace spm::acdrv;

#define ACPATCH_DEF_MAX 50

    extern AcDef * acDefs;

    void acpatchInit();
}
