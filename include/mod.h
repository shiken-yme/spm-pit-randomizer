#pragma once

#include <common.h>
#include <evt_cmd.h>

namespace mod {

  enum BlessId
  {
    /* 0x0 */ MERLUNA_NULL_BLESS,
    /* 0x1 */ MERLUNA_SPECTRE,
    /* 0x2 */ MERLUNA_HOURAI,
    /* 0x3 */ MERLUNA_PARAMITA
  };

  enum CurseId
  {
    /* 0x0 */ MERLUNA_NULL_CURSE,
    /* 0x1 */ MERLUNA_SHION,
    /* 0x2 */ MERLUNA_HEX,
    /* 0x3 */ MERLUNA_MIGRAINE
  };

  enum DisorderId
  {
    /* 0x0 */ DISORDER_NULL,
    /* 0x1 */ DISORDER_RED,
    /* 0x2 */ DISORDER_ORANGE,
    /* 0x3 */ DISORDER_YELLOW,
    /* 0x4 */ DISORDER_GREEN,
    /* 0x5 */ DISORDER_CYAN,
    /* 0x6 */ DISORDER_BLUE,
    /* 0x7 */ DISORDER_PURPLE,
    /* 0x8 */ DISORDER_WHITE,
    /* 0x9 */ DISORDER_BLACK,
  };

  extern bool gIsDolphin;
  extern bool gIsRiivolution;
  extern bool gIsPatchedDisc;
  extern bool gIs4_3;

  EVT_DECLARE(fwd_mover_speech)
  EVT_DECLARE(custom_pit_music)

  EVT_DECLARE_USER_FUNC(rand100, 1)
  EVT_DECLARE_USER_FUNC(handleBlessingWearOff, 0)
  EVT_DECLARE_USER_FUNC(get_mover_rng, 1)

  s32 danClearMapParts();
  s32 returnMoverRng();
  void main();

}
