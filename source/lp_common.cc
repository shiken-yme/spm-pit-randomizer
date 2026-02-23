#include <common.h>
#include <spm/item_data.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <wii/os.h>

namespace mod
{
    using namespace spm;

    // Input needs to be positive
    s32 round(f32 in)
    {
        s32 left = (s32)in;
        in -= left;
        if (in >= 0.5f)
            in = 1.0f;
        else
            in = 0.0f;
        s32 out = left + in;
        return out;
    }

    const char *npcTribeToName(s32 tribeId)
    {
        return msgdrv::msgSearch(item_data::itemDataTable[npcdrv::npcTribes[tribeId].catchCardItemId].nameMsg);
    }

}
