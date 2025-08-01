#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_fade)

//evt_fade_entry(s32 transitionType, s32 lengthMsec, char red, char blue, char green, char alpha)
EVT_DECLARE_USER_FUNC(evt_fade_entry, 6)

// evt_fade_end_wait(s32)
EVT_DECLARE_USER_FUNC(evt_fade_end_wait, 1)

EVT_DECLARE_USER_FUNC(evt_set_transition, 2)
EVT_DECLARE_USER_FUNC(evt_fade_set_center_pos, 3)
EVT_DECLARE_USER_FUNC(evt_fade_callfade, 1)
UNKNOWN_FUNCTION(func_800e720c)
EVT_DECLARE_USER_FUNC(evt_fade_set_transition_lengths, 2)

CPP_WRAPPER_END()
