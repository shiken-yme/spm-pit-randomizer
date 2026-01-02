#pragma once
#include <common.h>
#include <wii/gx.h>
#include <spm/effdrv.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>

namespace mod::effpatch
{
    using namespace spm;

    struct _EffPatchColorMask;
    typedef void (EffpatchColorMaskUpdateFunc)(struct _EffPatchColorMask *entry);

    typedef struct _EffPatchColorMask
    {
        spm::effdrv::EffEntry *eff;
        wii::gx::GXColor col1;
        wii::gx::GXColor col2;
        EffpatchColorMaskUpdateFunc *updateFunc;
        s32 frmCtr; // can be used by the update func to tell time relative to when it started
    } EffPatchColorMask;

    extern EffPatchColorMask effpatchColorMaskEntries[32];
    extern s32 effpatchColorMaskEntryCount;

    void effpatchColorMaskEntry(effdrv::EffEntry *eff, wii::gx::GXColor col1, wii::gx::GXColor col2, EffpatchColorMaskUpdateFunc *updateFunc);
    u8 effpatchChgColor(u8 color, u8 num, bool positive);
    void effpatchInit();
}