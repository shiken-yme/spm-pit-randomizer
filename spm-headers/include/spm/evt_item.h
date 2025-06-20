#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_item)

EVT_DECLARE_USER_FUNC(evt_item_event_free_work, 0)

EVT_UNKNOWN_USER_FUNC(evt_item_entry)
UNKNOWN_FUNCTION(func_800ecd70)
EVT_UNKNOWN_USER_FUNC(func_800ecda0)
EVT_UNKNOWN_USER_FUNC(func_800ece50)
EVT_UNKNOWN_USER_FUNC(evt_item_flag_onoff)
UNKNOWN_FUNCTION(func_800ecf80)
UNKNOWN_FUNCTION(func_800ed020)
UNKNOWN_FUNCTION(func_800ed0bc)
EVT_DECLARE_USER_FUNC(evt_item_wait_collected, 1)
EVT_UNKNOWN_USER_FUNC(evt_item_spawn_thunder)
UNKNOWN_FUNCTION(func_800ed188)
UNKNOWN_FUNCTION(func_800ed1dc)
UNKNOWN_FUNCTION(func_800ed468)
UNKNOWN_FUNCTION(func_800ed66c)

CPP_WRAPPER_END()
