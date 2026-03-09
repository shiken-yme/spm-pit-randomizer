#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/spmario_snd.h>

namespace mod::sndpatch
{
    using namespace spm;
    using namespace spm::spmario_snd;

    #define SNDPATCH_BGM_MAX 200

    void sndpatchInit();

    void sndpatchAddBGMEntry(BgmEntry *entry);
    void sndpatchAddBGMEntryDirect(const char *name, s16 brsarIdx, u8 volume, u8 pan, u8 flags, u8 otoyaId);
}
