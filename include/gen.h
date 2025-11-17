#pragma once

#include <common.h>
#include <evt_cmd.h>

namespace mod {

  EVT_DECLARE_USER_FUNC(evt_dan_read_data_new, -1)
  EVT_DECLARE_USER_FUNC(evt_dan_get_enemy_info_new, -1)

  s32 * getRfcItems();

}
