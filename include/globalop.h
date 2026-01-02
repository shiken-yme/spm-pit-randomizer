#pragma once

#include <common.h>
#include <evt_cmd.h>

namespace mod::globalop
{
    #define GLOBALOP_MAX 32

    typedef void (GlobalOpFunc)(void *param);

    struct GlobalOpEntry
    {
        GlobalOpFunc *func;
        void *param;
    };

    struct GlobalOperations
    {
        s32 entryCount;
        GlobalOpEntry Operations[GLOBALOP_MAX];
    };

    void GlobalOperationsInit();
    void globalopAddEntry(void *func, void *param);
}
