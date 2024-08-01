#include <common.h>

#include "evtdebug.h"
#include "patch.h"

#include <msl/string.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

namespace mod {

static s32 debugPutMsg(spm::evtmgr::EvtEntry * entry)
{
    const char * str = (char *) spm::evtmgr_cmd::evtGetValue(entry, entry->pCurData[0]);
    wii::os::OSReport("%s\n", str);
    return 2;
}

static s32 debugPutReg(s32 ret)
{
    // Write to OSReport
    wii::os::OSReport("%s\n", spm::evtmgr_cmd::evt_debug_put_reg_str);
    return ret;
}

void evtDebugPatch()
{
    // Replace debug_put_msg
    writeBranch(spm::evtmgr_cmd::evt_debug_put_msg, 0, debugPutMsg);

    // Hook on the blr of debug_put_reg
    writeBranch(spm::evtmgr_cmd::evt_debug_put_reg, 0x7b4, debugPutReg);
}

}
