#include "patch.h"
#include "globalop.h"

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/system.h>
#include <spm/mario.h>
#include <spm/memory.h>
#include <wii/os.h>
#include <msl/string.h>

namespace mod::globalop
{
    GlobalOperations *GlobalOp = nullptr;

    void globalopAddEntry(void *func, void *param)
    {
        GlobalOp->Operations[GlobalOp->entryCount].func = (GlobalOpFunc *)func;
        GlobalOp->Operations[GlobalOp->entryCount].param = param;
        GlobalOp->entryCount += 1;
        return;
    }

    void globalopMain()
    {
        if (GlobalOp->entryCount == 0)
            return;
        for (s32 i = 0; i < GlobalOp->entryCount; i += 1)
        {
            if (GlobalOp->Operations[i].param == nullptr)
            {
                (GlobalOp->Operations[i].func)(nullptr);
            }
            else
                (GlobalOp->Operations[i].func)(GlobalOp->Operations[i].param);
        }
        spm::mario::marioHandleSquash(); // Call the function the branchlink overwrites
    }

    void GlobalOperationsInit()
    {
        GlobalOp = (GlobalOperations *)spm::memory::__memAlloc(0, sizeof(GlobalOperations));
        msl::string::memset(GlobalOp, 0, sizeof(GlobalOperations));
        writeBranchLink(spm::mario::marioMain, 0x48C, globalopMain);
    }
}
