#pragma once
#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>

namespace mod::msgpatch
{
    struct MsgPatchPtrs
    {
        const char *identifier;
        const char *newText;
    };

    void msgpatchAddEntry(const char *msgName, const char *newText, bool allowDuplicate);
    void msgpatchDelEntry(const char *msgName);

    EVT_DECLARE_USER_FUNC(evt_msgpatch_add_entry, 3)
    EVT_DECLARE_USER_FUNC(evt_msgpatch_del_entry, 1)

    extern MsgPatchPtrs msgpatchEntries[512];

    void msgpatchMain();
}