#pragma once
#include "patch.h"
#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>

namespace mod::yme
{
    EVT_DECLARE(ymetoolsConfigMenu)
    EVT_DECLARE(delayClearHighlight)
    EVT_DECLARE(ymetoolsMobjMenu_Main)

    void ymeMain();
}