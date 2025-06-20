#pragma once

#include <nw4r/snd.hpp>
#include <spm/spmario_snd.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os/OSError.h>
#include <nw4r/snd.hpp>
#include "evt_cmd.h"

namespace mod::sndpatch
{
  EVT_DECLARE_USER_FUNC(evt_sndpatch_bgm_align, 2)
  EVT_DECLARE_USER_FUNC(evt_sndpatch_get_current_buffer, 2)

#define SNDPATCH_BGM_FADEIN(setPlayer, name, getPlayer, fade)                 \
  USER_FUNC(mod::sndpatch::evt_sndpatch_get_current_buffer, getPlayer, LW(0)) \
  DO(0)                                                                       \
  USER_FUNC(mod::sndpatch::evt_sndpatch_get_current_buffer, getPlayer, LW(1)) \
  IF_NOT_EQUAL(LW(0), LW(1))                                                  \
  DO_BREAK()                                                                  \
  END_IF()                                                                    \
  WAIT_FRM(1)                                                                 \
  WHILE()                                                                     \
  USER_FUNC(spm::evt_snd::evt_snd_bgmon, setPlayer, name)                     \
  USER_FUNC(spm::evt_snd::evt_snd_bgm_set_vol, setPlayer, 0, 0)               \
  WAIT_FRM(3)                                                                 \
  USER_FUNC(mod::sndpatch::evt_sndpatch_bgm_align, setPlayer, getPlayer)      \ 
  WAIT_FRM(40)                                                                \
      USER_FUNC(spm::evt_snd::evt_snd_bgm_fadein, setPlayer, fade)

}